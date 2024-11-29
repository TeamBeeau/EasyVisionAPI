using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BeeAPI
{
 
    public struct Global
    {
        public static BeeCplus.CCD CCD = new BeeCplus.CCD();
        public static GIL GIL = new GIL();
        public static Model model = new Model();

    }
   
}
