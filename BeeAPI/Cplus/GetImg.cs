using IndustrialNetworks.ModbusCore.DataTypes;
using OpenCvSharp;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BeeAPI
{
    public class GetImg
    {
         public static byte[]  ByteRaw()
        {
            int width=0, height=0, type=0;
            byte[] imageData = Global.CCD.GetRaw();

            // IntPtr intPtr =Native. GetImage(ref height, ref width, ref type);
            if (imageData.Length==0)
            {
                throw new InvalidOperationException("GetImage trả về con trỏ null.");
            }
            unsafe
            {
                Mat raw = new Mat(Global.CCD.rows, Global.CCD.cols, Global.CCD.typ, imageData);
                byte[] byteArray = raw.ImEncode(".png");
                return byteArray;
            }
        }
        public static byte[] ByteResult()
        {
            int width = 0, height = 0, type = 0;
            byte[] imageData = Global.CCD.GetResult();

            // IntPtr intPtr =Native. GetImage(ref height, ref width, ref type);
            if (imageData.Length == 0)
            {
                throw new InvalidOperationException("GetImage trả về con trỏ null.");
            }
            unsafe
            {
                Mat raw = new Mat(Global.CCD.rows, Global.CCD.cols, Global.CCD.typ, imageData);
                byte[] byteArray = raw.ImEncode(".png");
                return byteArray;
            }
        }
    }
}
