using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ActUtlType64Lib;


namespace BeeForm
{
    internal class PLC
    {
        private ActUtlType64 plc; 
        private bool isConnected = false; 

        public PLC()
        {
            plc = new ActUtlType64();
        }

        public bool IsConnected()
        {
            return isConnected;
        }

        public bool OpenConnection(int stationNumber)
        {
            try
            {
                plc.ActLogicalStationNumber = stationNumber;
                int result = plc.Open();

                if (result == 0) // Kết nối thành công
                {
                    isConnected = true;
                    Console.WriteLine("Kết nối tới PLC thành công!");
                }
                else
                {
                    isConnected = false;
                    Console.WriteLine($"Không thể kết nối tới PLC. Mã lỗi: {result}");
                }
                return isConnected;
            }
            catch (Exception ex)
            {
                Console.WriteLine("Lỗi kết nối: " + ex.Message);
                return false;
            }
        }

        // Đọc giá trị từ biến bit (VD: X000)
        public bool ReadBit(string address, out bool value)
        {
            value = false;
            if (!isConnected)
            {
                Console.WriteLine("Chưa kết nối với PLC.");
                return false;
            }

            try
            {
                int deviceValue;
                int result = plc.GetDevice(address, out deviceValue); // Trả về giá trị int

                if (result == 0)
                {
                    value = deviceValue == 1; // Chuyển đổi int thành bool (1 -> true, 0 -> false)
                    Console.WriteLine($"Dữ liệu tại {address}: {value}");
                    return true;
                }
                else
                {
                    Console.WriteLine($"Đọc dữ liệu thất bại. Mã lỗi: {result}");
                    return false;
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Lỗi khi đọc dữ liệu: " + ex.Message);
                return false;
            }
        }

        // Ghi giá trị vào biến bit (VD: Y000)
        public bool WriteBit(string address, bool value)
        {
            if (!isConnected)
            {
                Console.WriteLine("Chưa kết nối với PLC.");
                return false;
            }

            try
            {
                int result = plc.SetDevice(address, value ? 1 : 0); // Ghi giá trị 1 hoặc 0 vào PLC

                if (result == 0)
                {
                    Console.WriteLine($"Đã ghi {value} vào {address} thành công!");
                    return true;
                }
                else
                {
                    Console.WriteLine($"Ghi dữ liệu thất bại. Mã lỗi: {result}");
                    return false;
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Lỗi khi ghi dữ liệu: " + ex.Message);
                return false;
            }
        }

        // Đọc giá trị từ biến từ PLC dưới dạng int (VD: D100, có thể mở rộng cho các loại dữ liệu khác)
        public bool ReadInt(string address, out int value)
        {
            value = 0;
            if (!isConnected)
            {
                Console.WriteLine("Chưa kết nối với PLC.");
                return false;
            }

            try
            {
                int result = plc.GetDevice(address, out value); // Trả về giá trị int

                if (result == 0)
                {
                    Console.WriteLine($"Dữ liệu tại {address}: {value}");
                    return true;
                }
                else
                {
                    Console.WriteLine($"Đọc dữ liệu thất bại. Mã lỗi: {result}");
                    return false;
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Lỗi khi đọc dữ liệu: " + ex.Message);
                return false;
            }
        }

        // Ghi giá trị vào biến int (VD: D100)
        public bool WriteInt(string address, int value)
        {
            if (!isConnected)
            {
                Console.WriteLine("Chưa kết nối với PLC.");
                return false;
            }

            try
            {
                int result = plc.SetDevice(address, value); // Ghi giá trị int vào PLC

                if (result == 0)
                {
                    Console.WriteLine($"Đã ghi {value} vào {address} thành công!");
                    return true;
                }
                else
                {
                    Console.WriteLine($"Ghi dữ liệu thất bại. Mã lỗi: {result}");
                    return false;
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Lỗi khi ghi dữ liệu: " + ex.Message);
                return false;
            }
        }

        // Đóng kết nối với PLC
        public void CloseConnection()
        {
            if (isConnected)
            {
                try
                {
                    plc.Close();
                    isConnected = false;
                    Console.WriteLine("Đã ngắt kết nối với PLC.");
                }
                catch (Exception ex)
                {
                    Console.WriteLine("Lỗi khi ngắt kết nối: " + ex.Message);
                }
            }
            else
            {
                Console.WriteLine("Chưa kết nối với PLC.");
            }
        }
    }
}
