﻿using OpenCvSharp.Extensions;
using OpenCvSharp;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using BeeCplus;
using System.Runtime.InteropServices;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace BeeForm
{
    public partial class Main : Form
    {
        [DllImport(@".\BeeCplus.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        unsafe public static extern IntPtr GetResultImage(ref int h, ref int w, ref int type);
        public Main()
        {
            InitializeComponent();
        }
        Yolo Yolo = new Yolo();
        CCD CCD = new CCD();
        private void Main_Load(object sender, EventArgs e)
        {
            String result = Yolo.IniGIL();
            lbStatus.Text = result;
            List<String> list = CCD.ScanBasler();
            if (list.Count() > 0)
            {
                String nameCam = CCD.ConnectBasler("CAM1");
                lbStatus.Text = "CAM1";
            }
            else
            {
                lbStatus.Text = "No Camera";

            }
            try
            {
                btnTrigger.PerformClick();
            }
            catch
            {

            }
        }

        private void btnTrigger_Click(object sender, EventArgs e)
        {
            CCD.GrabBasler();
            String rs =Yolo.CheckYolo(Convert.ToSingle((float)tbarScore.Value / 10));
            string[] numbers = rs.Split(',');
            if (numbers.Length > 2)
            {
                lbWires.Text = numbers[0];
                lbCounter.Text = numbers[1];
                lbCycle.Text = numbers[2];
            }
            int counter = Convert.ToInt32(lbCounter.Text);
            int value = Convert.ToInt32(lbValue.Text);

            int progressValue = (int)((float)counter / value * 100);
            progressValue = Clamp(progressValue, pbar.Minimum, pbar.Maximum);

            pbar.Value = progressValue;

            lbPT.Text = progressValue.ToString();
            int height = 0, width = 0, type = 0;
            IntPtr intPtr = GetResultImage(ref height, ref width, ref type);

            unsafe
            {
                Mat raw = new Mat(height, width, type, intPtr);
                view.Image = OpenCvSharp.Extensions.BitmapConverter.ToBitmap(raw);
            }
        }
        private DateTime startTime;
        private TimeSpan elapsedTime;

        private void BtnRun_Click(object sender, EventArgs e)
        {
            IsLive = !IsLive;

            if (IsLive)
            {
                previousCounter = 0;
                startTime = DateTime.Now; 
                tmRun.Enabled = true;

                BtnRun.Text = "Stop";
                BtnRun.ForeColor = Color.Red;
                lbCount.Text = "Counting";
            }
            else
            {
                previousCounter = 0;
                tmRun.Enabled = false;

                BtnRun.Text = "Run";
                BtnRun.ForeColor = Color.Green;
                lbCount.Text = "Waiting";

                elapsedTime = DateTime.Now - startTime; 

                lbTotaltime.Text = $"{elapsedTime.TotalSeconds:F2} seconds"; 
            }
        }

        bool IsLive;
        private int previousCounter = 0;
        int newCounter;
        private void tmRun_Tick(object sender, EventArgs e)
        {
            CCD.GrabBasler();
            int value = Convert.ToInt32(lbValue.Text);
            String rs = Yolo.CheckYolo(Convert.ToSingle((float)tbarScore.Value / 10));
            string[] numbers = rs.Split(',');
            
            if (numbers.Length > 2)
            {
                lbWires.Text = numbers[0];

                newCounter = Convert.ToInt32(numbers[1]);
                if (newCounter >= previousCounter)
                {
                    lbCounter.Text = newCounter.ToString();
                    previousCounter = newCounter; 
                }

                lbCycle.Text = numbers[2];
            }
            if (newCounter > value)
            {
                BtnRun.PerformClick();
                tmRun.Enabled = false;
                IsLive = false;
                lbCount.Text = "Error";
                MessageBox.Show("Error: The counted quantity is greater than the norm ");
                return;
            }
            int counter = Convert.ToInt32(lbCounter.Text);
            

            int progressValue = (int)((float)counter / value * 100);
            progressValue = Clamp(progressValue, pbar.Minimum, pbar.Maximum);

            pbar.Value = progressValue;

            lbPT.Text = progressValue.ToString();
            int height = 0, width = 0, type = 0;
            IntPtr intPtr = GetResultImage(ref height, ref width, ref type);
            if (newCounter >= previousCounter)
            {
                unsafe
                {
                    Mat raw = new Mat(height, width, type, intPtr);
                    view.Image = OpenCvSharp.Extensions.BitmapConverter.ToBitmap(raw);
                }
            }
            if (counter == value)
            {
                BtnRun.PerformClick();
                tmRun.Enabled = false;
                IsLive = false;
                qty++;
                lbQty.Text = qty.ToString();
                lbCount.Text = "Done";
            }
            if (IsLive)
            {
                tmRun.Enabled = true;
            }
            else
            {
                tmRun.Enabled = false;
            }
        }
        public static int Clamp(int value, int min, int max)
        {
            if (value < min) return min;
            if (value > max) return max;
            return value;
        }
        private void Main_FormClosing(object sender, FormClosingEventArgs e)
        {
            CCD.DisconnectBasler();
            Yolo.FinalizeGIL();
        }

        private void trackBar1_ValueChanged(object sender, EventArgs e)
        {
            lbValue.Text = trackBar1.Value.ToString();
            lbVValue.Text = trackBar1.Value.ToString();
        }
        int qty = 0;
        private void lbCounter_TextChanged_1(object sender, EventArgs e)
        {
            try
            {

                if (lbCount.Text.Trim() == lbValue.Text.Trim())
                {
                    BtnRun.PerformClick();
                    lbCount.Text = "0";
                    qty++;
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error: {ex.Message}", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            finally
            {
                lbQty.Text = qty.ToString();
            }
        }
        private void tbarScore_ValueChanged(object sender, EventArgs e)
        {
            lbVScore.Text = Convert.ToSingle((float)tbarScore.Value / 10).ToString();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            qty = 0;
            lbQty.Text = "0";
            lbTotaltime.Text = "0";
            lbWires.Text = "0";
            lbCycle.Text = "0";
            lbCounter.Text= "0";
            pbar.Value = pbar.Minimum;
           
        }
    }
}
