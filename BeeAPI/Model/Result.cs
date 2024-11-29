using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BeeAPI
{
    public class Result
    {
        private int counter;
        private int cycle;
        private long wires;

        public int Counter { get => counter; set => counter = value; }
        public int Cycle { get => cycle; set => cycle = value; }
        public long Wires { get => wires; set => wires = value; }
    }
}
