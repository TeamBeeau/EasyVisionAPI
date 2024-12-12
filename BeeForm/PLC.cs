using System;
using System.IO.Ports;
using System.Threading;
using System.Windows.Forms;

namespace BeeForm
{
    public class PLC
    {
        private SerialPort sp = null;

        private int DELAY = 10;

        public PLC(string portName, int baudrate, int dataBit, StopBits stopBits, Parity parity)
        {
            try
            {
                sp = new SerialPort(portName, baudrate, parity, dataBit, stopBits);
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        public bool Connect()
        {
            try
            {
                if (sp.IsOpen)
                {
                    sp.Close();
                }
                sp.Open();
                return true;
            }
            catch (Exception ex)
            {
                throw ex;
                return false;
            }
        }

        public void Disconnect()
        {
            try
            {
                if (sp.IsOpen)
                {
                    sp.Close();
                }
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }
        public void TestSerial()
        {
            string testCommand = "30 B1 30 30 30 30 B8 03 35 C3";
            byte[] commandBytes = Array.ConvertAll(testCommand.Split(' '), s => Convert.ToByte(s, 16));

            Console.WriteLine("Sending command: " + BitConverter.ToString(commandBytes));

            sp.Write(commandBytes, 0, commandBytes.Length);
            Thread.Sleep(100);

            if (sp.BytesToRead > 0)
            {
                byte[] response = new byte[sp.BytesToRead];
                sp.Read(response, 0, response.Length);
                Console.WriteLine("Received response: " + BitConverter.ToString(response));
            }
            else
            {
                Console.WriteLine("No data received.");
            }
        }

        public int[] Read()
        {
            int[] array = new int[20];
            string text = "30 B1 30 30 30 30 B8 03 35 C3";
            byte[] array2 = new byte[11];
            string[] array3 = text.Split(' ');
            byte b = 0;
            string[] array4 = array3;
            foreach (string value in array4)
            {
                array2[b + 1] = Convert.ToByte(value, 16);
                b++;
            }
            array2[0] = Convert.ToByte("82", 16);
            sp.Write(array2, 0, array2.Length);
            Thread.Sleep(DELAY);
            try
            {
                Thread.Sleep(20);
                if (sp.BytesToRead >= 1)
                {
                    byte[] array5 = new byte[sp.BytesToRead];
                    sp.Read(array5, 0, sp.BytesToRead);
                    sp.DiscardOutBuffer();
                    byte[] array6 = new byte[array5.Length - 3];
                    Array.Copy(array5, 1, array6, 0, array5.Length - 3);
                    string text2 = "";
                    if (array6.Length != 17)
                    {
                        return null;
                    }
                    byte[] array7 = array6;
                    foreach (int num in array7)
                    {
                        string text3 = $"{(byte)num:x}".ToUpper();
                        if (text3 == "30")
                        {
                            text3 = "0";
                        }
                        if (text3 == "B1" || text3 == "31")
                        {
                            text3 = "1";
                        }
                        if (text3 == "B2" || text3 == "32")
                        {
                            text3 = "2";
                        }
                        if (text3 == "33")
                        {
                            text3 = "3";
                        }
                        if (text3 == "B4" || text3 == "34")
                        {
                            text3 = "4";
                        }
                        if (text3 == "35")
                        {
                            text3 = "5";
                        }
                        if (text3 == "36")
                        {
                            text3 = "6";
                        }
                        if (text3 == "B7" || text3 == "37")
                        {
                            text3 = "7";
                        }
                        if (text3 == "B8" || text3 == "38")
                        {
                            text3 = "8";
                        }
                        if (text3 == "39" || text3 == "B9")
                        {
                            text3 = "9";
                        }
                        if (text3 == "41")
                        {
                            text3 = "A";
                        }
                        if (text3 == "42")
                        {
                            text3 = "B";
                        }
                        if (text3 == "C3" || text3 == "43")
                        {
                            text3 = "C";
                        }
                        if (text3 == "44")
                        {
                            text3 = "D";
                        }
                        if (text3 == "45")
                        {
                            text3 = "E";
                        }
                        if (text3 == "C6" || text3 == "46")
                        {
                            text3 = "F";
                        }
                        text2 += text3;
                    }
                    for (int k = 0; k < text2.Length - 1; k += 4)
                    {
                        string value2 = text2.Substring(k + 2, 2) + text2.Substring(k, 2);
                        array[k / 4] = Convert.ToInt16(value2, 16);
                    }
                }
            }
            catch
            {
            }
            return array;
        }

        public void Write(int[] write)
        {
            string[] array = new string[17]
            {
                "82",
                "31",
                "31",
                "30",
                "30",
                "30",
                "30",
                "38",
                ConWrite(write[0]),
                ConWrite(write[1]),
                ConWrite(write[2]),
                ConWrite(write[3]),
                "03",
                null,
                null,
                null,
                null
            };
            string text = "";
            for (byte b = 1; b < array.Length; b++)
            {
                text = text + " " + array[b];
            }
            string text2 = Lrc(text.Trim());
            array[13] = text2.Remove(2);
            array[14] = text2.Substring(3);
            string text3 = text.Trim() + " " + array[13] + " " + array[14];
            byte[] array2 = new byte[27];
            string[] array3 = text3.Trim().Split(' ');
            byte b2 = 0;
            string[] array4 = array3;
            foreach (string value in array4)
            {
                array2[b2 + 1] = Convert.ToByte(value, 16);
                b2++;
            }
            array2[0] = Convert.ToByte(array[0], 16);
            sp.Write(array2, 0, array2.Length);
        }

        private string ConWrite(int nhap)
        {
            string text = nhap.ToString("X4");
            string[] array = new string[4]
            {
                ChartoHEX(text.Substring(2).Remove(1, 1)),
                ChartoHEX(text.Substring(3)),
                ChartoHEX(text.Remove(1, 3)),
                ChartoHEX(text.Substring(1).Remove(1, 2))
            };
            return array[0] + " " + array[1] + " " + array[2] + " " + array[3];
        }

        private string Lrc(string nhap)
        {
            byte b = 0;
            char[] separator = new char[1] { ' ' };
            string[] array = nhap.Split(separator);
            foreach (string value in array)
            {
                byte b2 = Convert.ToByte(value, 16);
                b += b2;
            }
            string text = $"{b:x2}".ToUpper();
            string text2 = ChartoHEX(text.Substring(1));
            return ChartoHEX(text.Remove(1, 1)) + " " + text2;
        }

        private string ChartoHEX(string nhap)
        {
            string result = "";
            if (nhap == "0")
            {
                result = "30";
            }
            if (nhap == "1")
            {
                result = "31";
            }
            if (nhap == "2")
            {
                result = "32";
            }
            if (nhap == "3")
            {
                result = "33";
            }
            if (nhap == "4")
            {
                result = "34";
            }
            if (nhap == "5")
            {
                result = "35";
            }
            if (nhap == "6")
            {
                result = "36";
            }
            if (nhap == "7")
            {
                result = "37";
            }
            if (nhap == "8")
            {
                result = "38";
            }
            if (nhap == "9")
            {
                result = "39";
            }
            if (nhap == "A")
            {
                result = "41";
            }
            if (nhap == "B")
            {
                result = "42";
            }
            if (nhap == "C")
            {
                result = "43";
            }
            if (nhap == "D")
            {
                result = "44";
            }
            if (nhap == "E")
            {
                result = "45";
            }
            if (nhap == "F")
            {
                result = "46";
            }
            return result;
        }
    }
}
