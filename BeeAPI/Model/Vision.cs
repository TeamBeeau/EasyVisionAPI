using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace BeeAPI
{
  
    public class Vision
    {
        private  string nameModel;
        private  string pathPythonTest;
        private  string pathYolo;
        private  string pathPythonTrain;
        private  string ex;
        private  float score = 0.6f;
        private  int counter;
        private  int cycle;
        private  bool isOK;
        private  float maxObject;
   
        private string pathimg;
        private long wires;
        public  string NameModel { get => nameModel; set => nameModel = value; }
        public  string PathPythonTest { get => pathPythonTest; set => pathPythonTest = value; }
        public  string PathYolo { get => pathYolo; set => pathYolo = value; }
        public  string PathPythonTrain { get => pathPythonTrain; set => pathPythonTrain = value; }
        public  string Ex { get => ex; set => ex = value; }
        public  float Score { get => score; set => score = value; }
        public int Counter { get => counter; set => counter = value; }
        public int Cycle { get => cycle; set => cycle = value; }
        public bool IsOK { get => isOK; set => isOK = value; }
        public float MaxObject { get => maxObject; set => maxObject = value; }

      
        public string PathImg { get => pathimg; set => pathimg = value; }
        public long Wires { get => wires; set => wires = value; }

        public Vision()
        {

        }
    
        public string GetVision(string vision)
        {
            switch (vision)
            {
                case "NameModel":
                    vision = NameModel;
                    return vision;

                case "PathPythonTest":
                    vision = PathPythonTest;
                    return vision;

                case "PathYolo":
                    vision = PathYolo;
                    return vision;

                case "PathPythonTrain":
                    vision = PathPythonTrain;
                    return vision;

                case "Score":
                    vision = Score.ToString() ;
                    return vision;

                case "Counter":
                    vision = Counter.ToString();
                    return vision;

                case "Cycle":
                    vision = Cycle.ToString();
                    return vision;

                case "MaxObject":
                    vision = MaxObject.ToString();
                    return vision;
                case "Ex":
                    Ex = vision;
                    return "Ex đã được cập nhật";


                default:
                    return "Parameter không hợp lệ";
            }
        }
        public string SetVision(string vision, string value)
        {
            switch (vision)
            {
                case "NameModel":
                    NameModel = value;
                    return "NameModel đã được cập nhật";

                case "PathPythonTest":
                    PathPythonTest = value;
                    return "PathPythonTest đã được cập nhật";

                case "PathYolo":
                    PathPythonTest = value;
                    return "PathYolo đã được cập nhật";

                case "PathPythonTrain":
                    PathPythonTest = value;
                    return "PathPythonTrain đã được cập nhật";

                case "Score":
                    Score = Convert.ToSingle(value);
                    return "Height đã được cập nhật";

                case "Counter":
                    Counter = Convert.ToInt32(value);
                    return "Counter đã được cập nhật";

                case "Cycle":
                    Cycle = Convert.ToInt32(value);
                    return "Cycle đã được cập nhật";

                case "MaxObject":
                    MaxObject = Convert.ToSingle(value);
                    return "MaxObject đã được cập nhật";
                case "Ex":
                    Ex = value;
                    return "Ex đã được cập nhật";
                default:
                    return "Parameter không hợp lệ";
            }
        }
    }
}
