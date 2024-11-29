using Microsoft.Owin.Hosting;
using OpenCvSharp;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Net.Http;
using System.Text;

using System.Threading.Tasks;
using System.Timers;

namespace BeeAPI
{
    internal class Program
    {
       // static  Timer tmIniTial =new Timer();
        static void Main(string[] args)
        {
            string baseAddress = "http://localhost:8080/";
          
            using (WebApp.Start<Startup>(url: baseAddress))
            {
                Console.WriteLine("Bee API v1.0.4 is running at " + baseAddress);
               
                Console.WriteLine("Waiting Ininial Python ....");
               
                
              //  tmIniTial.Elapsed += TmIniTial_Elapsed;
             //   tmIniTial.Interval = 100;
             //  tmIniTial.Enabled = true;
                Console.WriteLine("Press Enter to exit...");
              Console.ReadLine();
              Global.GIL.FinalizeGIL();
            }
        }

        private static async void TmIniTial_Elapsed(object sender, ElapsedEventArgs e)
        {
            //tmIniTial.Enabled = false;
            //Process process = new Process();
            //process.Start();
            ////string apiUrl2 = "http://localhost:8080/api/bee/InitialPython"; // Kiểm tra lại URL

            //using (HttpClient client = new HttpClient())
            //{
            //    HttpResponseMessage response = await client.GetAsync(apiUrl2);

            //    if (response.IsSuccessStatusCode)
            //    {
            //        string result = await response.Content.ReadAsStringAsync();
            //        Console.WriteLine($"Response: {result}");
            //    }
            //    else
            //    {
            //        Console.WriteLine($"Error: {response.StatusCode}");
            //    }
           // }

        }
    }
}
