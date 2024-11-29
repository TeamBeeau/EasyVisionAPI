using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;

using System.Threading;
using IndustrialNetworks.ModbusCore.ASCII;

namespace BeeAPI
{
    [Serializable()]
    public class PLC
    {
        public String StringConnect = "192.168.1:100";
        private List<Tuple<String, String, float>> addRead = new List<Tuple<string, string, float>>(); // List Vùng Nhớ Đọc PLC Funtion ,Address,Value
        private List<Tuple<String, String, float>> addWrite = new List<Tuple<string, string, float>>(); // List Vùng Nhớ Đọc PLC Funtion ,Address,Value
        private bool isConnect = false;
        private bool isBusy = false;
        private Thread plc;
        private ModbusASCIIMaster master;

        public List<Tuple<string, string, float>> AddRead { get => addRead; set => addRead = value; }
        public List<Tuple<string, string, float>> AddWrite { get => addWrite; set => addWrite = value; }
        public bool IsConnect { get => isConnect; set => isConnect = value; }
        public bool IsBusy { get => isBusy; set => isBusy = value; }
        public ModbusASCIIMaster Master { get => master; set => master = value; }

        public PLC() { }
        public List<String> ScanPLC()
        {
            string[] ports = SerialPort.GetPortNames();

            List<string> availablePorts = new List<string>(ports);

            return availablePorts;
        }
        public string ConnectPLC(string portName, string BaudRate)
        {
            if (isConnect == false)
            {
                try
                {
                    int baudRate = Convert.ToInt32(BaudRate);
                    int dataBits = 8;
                    StopBits stopBits = StopBits.One;
                    Parity parity = Parity.None;

                    master = new ModbusASCIIMaster(portName, baudRate, dataBits, stopBits, parity);
                    master.Connection();
                    isConnect = true;

                    plc = new Thread(new ThreadStart(PLCThreadProcess));
                    plc.IsBackground = true;
                    plc.Start();

                    return "Đã kết nối PLC!";
                }
                catch (Exception ex)
                {
                    isConnect = false;
                    return "Kết nối thất bại: " + ex.Message;
                }
            }
            else
            {
                return "Đã có kết nối trước!";
            }
        }

        private void PLCThreadProcess()
        {
            while (isConnect)
            {
                try
                {
                    if (Read())
                    {
                        Console.WriteLine("Đọc dữ liệu từ PLC thành công.");
                    }

                    if (Write())
                    {
                        Console.WriteLine("Ghi dữ liệu vào PLC thành công.");
                    }
                    Thread.Sleep(500);
                }
                catch (Exception ex)
                {
                    Console.WriteLine("Lỗi trong luồng PLC: " + ex.Message);
                }
            }
            Console.WriteLine("Luồng PLC đã dừng.");
        }


        public void DisconnectPLC()
        {
            if (isConnect && master != null)
            {
                try
                {
                    if (plc != null && plc.IsAlive)
                    {
                        Console.WriteLine("Đang dừng luồng PLC...");
                        isConnect = false;

                        plc.Join(2000);
                    }

                    master.Disconnection();
                    IsConnect = false;
                    Console.WriteLine("Đã ngắt kết nối PLC và dừng luồng thành công!");
                }
                catch (Exception ex)
                {
                    Console.WriteLine("Lỗi khi ngắt kết nối hoặc dừng luồng: " + ex.Message);
                }
            }
            else
            {
                Console.WriteLine("PLC chưa được kết nối!");
            }
        }

        public bool Read()
        {
            try
            {
                foreach (var readItem in addRead)
                {
                    string functionCode = readItem.Item1;
                    string address = readItem.Item2;     
                    uint startAddress = Convert.ToUInt32(address); 

                    byte[] data = null;

                    switch (functionCode)
                    {
                        case "03":
                            data = master.ReadHoldingRegisters(1, startAddress, 1);
                            break;

                        case "04":
                            data = master.ReadInputRegisters(1, startAddress, 1);
                            break;

                        default:
                            Console.WriteLine($"Function code {functionCode} không được hỗ trợ trong Read().");
                            continue;
                    }

                    if (data != null)
                    {
                        float value = BitConverter.ToSingle(data, 0); 
                        int index = addRead.IndexOf(readItem);
                        addRead[index] = new Tuple<string, string, float>(functionCode, address, value); 
                    }
                }
                return true;
            }
            catch (Exception ex)
            {
                Console.WriteLine("Lỗi khi đọc PLC: " + ex.Message);
                return false; 
            }
        }

        public bool Write()
        {
            try
            {
                foreach (var writeItem in addWrite)
                {
                    string functionCode = writeItem.Item1;
                    string address = writeItem.Item2;   
                    uint startAddress = Convert.ToUInt32(address);
                    float value = writeItem.Item3;       

                    switch (functionCode)
                    {
                        case "05":
                            bool coilValue = value > 0; 
                            master.WriteSingleCoil(1, startAddress, coilValue);
                            break;

                        case "06":
                            byte[] registerValue = BitConverter.GetBytes(value);
                            master.WriteSingleRegister(1, startAddress, registerValue);
                            break;

                        default:
                            Console.WriteLine($"Function code {functionCode} không được hỗ trợ trong Write().");
                            continue;
                    }
                }
                return true; 
            }
            catch (Exception ex)
            {
                Console.WriteLine("Lỗi khi ghi PLC: " + ex.Message);
                return false; 
            }
        }

    }
}
