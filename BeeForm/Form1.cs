using BeeCplus;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using OpenCvSharp;
using System.Runtime.InteropServices;
namespace BeeForm
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }
        Yolo Yolo = new Yolo();
        CCD CCD = new CCD();
        private void button1_Click(object sender, EventArgs e)
        {
            String result = Yolo.IniGIL();
            lbStatus.Text = result;
        }

        private void button2_Click(object sender, EventArgs e)
        {
           List< String> list = CCD.ScanBasler();
            if (list .Count()>0)
             {
           String result=  CCD.ConnectBasler("CAM1");
                lbStatus.Text = result;
            }
           else
            {
               lbStatus.Text="No Camera";

            }    
        }

        private void button3_Click(object sender, EventArgs e)
        {
            CCD.GrabBasler();
            Yolo.CheckYolo(Convert.ToSingle(numericUpDown1.Value));
            int height = 0, width = 0, type = 0;
            IntPtr intPtr = GetResultImage(ref height, ref width, ref type);

            unsafe
            {
                Mat raw = new Mat(height, width, type, intPtr);
                view.Image = OpenCvSharp.Extensions.BitmapConverter.ToBitmap(raw);
            }

        }
        [DllImport(@".\BeeCplus.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        unsafe public static extern IntPtr GetResultImage(ref int h, ref int w, ref int type );
        private void work_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
           
               
        }
        bool IsLive = false;
        private void btnLive_Click(object sender, EventArgs e)
        {
            IsLive = !IsLive;
            if (IsLive)
            {
                btnLive.BackColor = Color.Gold;
                tmRun.Enabled = true;
            }
            else
            { btnLive.BackColor = Color.White; tmRun.Enabled = false; }
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            CCD.DisconnectBasler();
            Yolo.FinalizeGIL();
        }

        private void tmRun_Tick(object sender, EventArgs e)
        {
            CCD.GrabBasler();
         String rs=   Yolo.CheckYolo(Convert.ToSingle(numericUpDown1.Value));
            lbStatus.Text = rs;
            int height = 0, width = 0, type = 0;
            IntPtr intPtr = GetResultImage(ref height, ref width, ref type);

            unsafe
            {
                Mat raw = new Mat(height, width, type, intPtr);
                view.Image = OpenCvSharp.Extensions.BitmapConverter.ToBitmap(raw);
            }


            if (IsLive)
            {
               
                tmRun.Enabled = true;
            }
            else
                tmRun.Enabled = false;


        }
    }
}
