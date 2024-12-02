//using BeeAPI.Model;
using Newtonsoft.Json;
using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.Serialization.Formatters.Binary;
using System.Text;

using System.Threading.Tasks;
using System.Web.Script.Serialization;
using System.Xml.Serialization;


namespace BeeAPI
{
 
    public  class Model
    {
        public DB DB { get; set; } = new DB();
        public CCD CCD { get; set; } = new CCD();
        public PLC PLC { get; set; } = new PLC();
        public Vision Vision { get; set; } = new Vision();
        public Result Result { get; set; } = new Result();


        public  Model()
        {

        }
        //public void SaveModel(Model Model, string nameModel)
        //{
        //   // string folderPath = Path.Combine("C:\\", "models");
        //    string folderPath = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments), "models");

        //    if (!Directory.Exists(folderPath))
        //    {
        //        Directory.CreateDirectory(folderPath);
        //    }

        //    string path = Path.Combine(folderPath, nameModel + ".dat");

        //    try
        //    {
        //        using (MemoryStream ms = new MemoryStream())
        //        {
        //            BinaryFormatter bf = new BinaryFormatter();
        //            bf.Serialize(ms, Model); 

        //            ms.Position = 0;
        //            byte[] buffer = new byte[(int)ms.Length];
        //            ms.Read(buffer, 0, buffer.Length);

        //            File.WriteAllText(path, Convert.ToBase64String(buffer));
        //        }
        //        Console.WriteLine("Done" + folderPath);
        //    }
        //    catch (Exception ex)
        //    {
        //        Console.WriteLine("Error while saving model: " + ex.Message);
        //    }
        //}
        public void SaveModel(string nameModel)
        {
            //Global.model.Vision.NameModel = nameModel;
            string folderPath = Path.Combine("C:\\", "models");

            if (!Directory.Exists(folderPath))
            {
                Directory.CreateDirectory(folderPath);
            }

          
            try
            {
                string path = Path.Combine(folderPath, nameModel + ".json");
                JavaScriptSerializer serializer = new JavaScriptSerializer();
                string jsonString = serializer.Serialize(Global.model);
                Console.WriteLine(jsonString);  
                File.WriteAllText(path, jsonString);
                Console.WriteLine("Model saved successfully at: " + path);
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error while saving model: " + ex.Message);
            }
        }

        public Model LoadModel(string nameModel)
        {
            Global.model.Vision.NameModel = nameModel;
            string folderPath = Path.Combine("C:\\", "models");
            string path = Path.Combine(folderPath, nameModel + ".json");
            Model Model;
            if (!File.Exists(path))
            {
                return null;
            }
        
            JavaScriptSerializer serializer = new JavaScriptSerializer();
            Model model = serializer.Deserialize<Model>(File.ReadAllText(path));
           
            return model;
        }
    }
}