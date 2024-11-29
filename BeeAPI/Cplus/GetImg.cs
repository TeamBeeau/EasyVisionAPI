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
            IntPtr intPtr =Native. GetImage(ref height, ref width, ref type);
            if (intPtr == IntPtr.Zero)
            {
                throw new InvalidOperationException("GetImage trả về con trỏ null.");
            }
            unsafe
            {
                Mat raw = new Mat(height, width, type, intPtr);
                byte[] byteArray = raw.ImEncode(".png");
                return byteArray;
            }
        }
        public static byte[] ByteResult()
        {
            int width = 0, height = 0, type = 0;
            IntPtr intPtr = Native.GetResultImage(ref height, ref width, ref type);
            if (intPtr == IntPtr.Zero)
            {
                throw new InvalidOperationException("GetImage trả về con trỏ null.");
            }
            unsafe
            {
                Mat raw = new Mat(height, width, type, intPtr);
                byte[] byteArray = raw.ImEncode(".png");
                return byteArray;
            }
        }
    }
}
