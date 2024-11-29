using BeeCplus;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BeeAPI
{
    public class GIL
    {
        public Yolo Yolo=new Yolo();
        public String IniGIL()
        {
            unsafe
            {
                lock (this)
                {
                    return Yolo.IniGIL();
                }
            }
        }
        public String FinalizeGIL()
        {
            unsafe
            {
                lock (this)
                {
                    return Yolo.FinalizeGIL();
                }
            }
        }
        public String TestYolo(float Score)
        {
            lock (this)
            {
                return Yolo.TestYolo(Score);
            }
        }
        public String CheckYolo(float Score)
        {
            lock (this)
            {
                return Yolo.CheckYolo(Score);
            }
        }
    }
}
