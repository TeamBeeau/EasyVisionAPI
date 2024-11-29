using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO.Ports;
using System.Runtime.InteropServices;
using BeeCplus;
using System.Drawing;
using System.Linq;
using OpenCvSharp;
using System.Web.UI.WebControls.WebParts;


namespace BeeAPI
{
    [Serializable()]
    public class CCD
    {
        public int width = 0;
        public int height = 0;
        private float offSetX = 0;
        private float offSetY = 0;
        private float exposure = 0;
        private float gain = 0;
        private float briness = 0;
        private float fps = 0;
        private bool isConnect = false;
        private int type = 0;
        private float gamma = 0;

        public int Width { get => width; set => width = value; }
        public int Height { get => height; set => height = value; }
        public float OffSetX { get => offSetX; set => offSetX = value; }
        public float OffSetY { get => offSetY; set => offSetY = value; }
        public float Exposure { get => exposure; set => exposure = value; }
        public float Gain { get => gain; set => gain = value; }
        public float Briness { get => briness; set => briness = value; }
        public float Fps { get => fps; set => fps = value; }
        public bool IsConnect { get => isConnect; set => isConnect = value; }
        public float Gamma { get => gamma; set => gamma = value; }
        public string SetParaA(string para, string value)
        {
            try
            {
                switch (para)
                {
                    case "Exposure":
                        if (!float.TryParse(value, out float exposureTemp))
                            return $"Invalid value for {para}: {value}";
                        Exposure = exposureTemp;
                       Native. SetPara(para, Exposure);
                        return "Exposure đã được cập nhật";
                    case "Gamma":
                        if (!float.TryParse(value, out float gammaTemp))
                            return $"Invalid value for {para}: {value}";
                        Gamma = gammaTemp;
                        Native.SetPara(para, Gamma);
                        return "Gamma đã được cập nhật";

                    case "Gain":
                        if (!float.TryParse(value, out float gainTemp))
                            return $"Invalid value for {para}: {value}";
                        Gain = gainTemp;
                        Native.SetPara(para, Gain);
                        return "Gain đã được cập nhật";

                    case "Brightness":
                        if (!float.TryParse(value, out float brinessTemp))
                            return $"Invalid value for {para}: {value}";
                        Briness = brinessTemp;
                        Native.SetPara(para, Briness);
                        return "Briness đã được cập nhật";

                    case "Width":
                        if (!int.TryParse(value, out int widthTemp))
                            return $"Invalid value for {para}: {value}";
                        Width = widthTemp;
                        Native.SetPara(para, Width);
                        return "Width đã được cập nhật";

                    case "Height":
                        if (!int.TryParse(value, out int heightTemp))
                            return $"Invalid value for {para}: {value}";
                        Height = heightTemp;
                        Native.SetPara(para, Height);
                        return "Height đã được cập nhật";

                    case "OffSetX":
                        if (!float.TryParse(value, out float offSetXTemp))
                            return $"Invalid value for {para}: {value}";
                        OffSetX = offSetXTemp;
                        Native.SetPara(para, OffSetX);
                        return "OffSetX đã được cập nhật";

                    case "OffSetY":
                        if (!float.TryParse(value, out float offSetYTemp))
                            return $"Invalid value for {para}: {value}";
                        OffSetY = offSetYTemp;
                        Native.SetPara(para, OffSetY);
                        return "OffSetY đã được cập nhật";

                    case "Fps":
                        if (!float.TryParse(value, out float fpsTemp))
                            return $"Invalid value for {para}: {value}";
                        Fps = fpsTemp;
                        return "Fps đã được cập nhật";

                    default:
                        return "Parameter không hợp lệ";
                }
            }
            catch (Exception ex)
            {
                return $"Error setting parameter '{para}': {ex.Message}";
            }
        }

        public string GetParaA(string para)
        {
            try
            {
                string value = Marshal.PtrToStringAnsi(Native.GetPara(para)); 
                if (string.IsNullOrWhiteSpace(value))
                    return $"Value for {para} is null or empty.";

                switch (para)
                {
                    case "Exposure":
                        if (!float.TryParse(value, out float exposureTemp))
                            return $"Invalid format for {para}: {value}";
                        Exposure = exposureTemp;
                        return Exposure.ToString();
                    case "Gamma":
                        try
                        {
                            if (!float.TryParse(value, out float gammaTemp))
                                return $"Invalid format for {para}: {value}";
                            Gamma = gammaTemp;
                            return Gamma.ToString();
                        }

                        catch
                        {
                            Gamma = 0;
                            return Gamma.ToString();
                        }
                    case "Gain":
                        try
                        {
                            if (!float.TryParse(value, out float gainTemp))
                                return $"Invalid format for {para}: {value}";
                            Gain = gainTemp;
                            return Gain.ToString();
                        }
                        catch
                        {
                            Gain = 0;
                            return Gain.ToString();
                        }


                    case "Brightness":
                        try
                        {
                            if (!float.TryParse(value, out float brinessTemp))
                                return $"Invalid format for {para}: {value}";
                            Briness = brinessTemp;
                            return Briness.ToString();
                        }
                        catch
                        {
                            Briness = 0;
                            return Briness.ToString();
                        }
                       

                    case "Width":
                        try
                        {
                            if (!int.TryParse(value, out int widthTemp))
                                return $"Invalid format for {para}: {value}";
                            Width = widthTemp;
                            return Width.ToString();
                        }
                        catch
                        {
                            Width = 0;
                            return Width.ToString();
                        }
                       

                    case "Height":
                        try
                        {
                            if (!int.TryParse(value, out int heightTemp))
                                return $"Invalid format for {para}: {value}";
                            Height = heightTemp;
                            return Height.ToString();
                        }
                        catch
                        {
                            Height = 0;
                            return Height.ToString();
                        }
                     

                    case "OffSetX":
                        try
                        {
                            if (!float.TryParse(value, out float offSetXTemp))
                                return $"Invalid format for {para}: {value}";
                            OffSetX = offSetXTemp;
                            return OffSetX.ToString();
                        }
                        catch
                        {
                            OffSetX = 0;
                            return OffSetX.ToString();
                        }

                    case "OffSetY":
                        try
                        {
                            if (!float.TryParse(value, out float offSetYTemp))
                                return $"Invalid format for {para}: {value}";
                            OffSetY = offSetYTemp;
                            return OffSetY.ToString();
                        }

                        catch
                        {
                            OffSetY = 0;
                            return OffSetY.ToString();
                        }
                    case "Fps":
                        return Fps.ToString();

                    default:
                        return $"Unknown parameter: {para}";
                }
            }
            catch (Exception ex)
            {
                return $"Error: {ex.Message}";
            }
        }

        public string GetParaCCD(string para)
        {
            try
            {
                switch (para)
                {
                    case "Exposure":

                        try
                        {
                            string exp = Marshal.PtrToStringAnsi(Native.SetPara(para, Exposure));
                            return Exposure.ToString();
                        }

                        catch
                        {
                            Exposure = 0;
                            return Exposure.ToString();
                        }
                    case "Gamma":
                        try
                        {
                            string gam = Marshal.PtrToStringAnsi(Native.SetPara(para, Gamma));
                            return Gamma.ToString();
                        }

                        catch
                        {
                            Gamma = 0;
                            return Gamma.ToString();
                        }
                    case "Gain":
                        try
                        {
                            string gai = Marshal.PtrToStringAnsi(Native.SetPara(para, Gain));
                            return Gain.ToString();
                        }
                        catch
                        {
                            Gain = 0;
                            return Gain.ToString();
                        }
                    case "Brightness":
                        try
                        {
                            string bri = Marshal.PtrToStringAnsi(Native.SetPara(para, Briness));
                            return Briness.ToString();
                        }
                        catch
                        {
                            Briness = 0;
                            return Briness.ToString();
                        }
                      

                    case "Width":
                        try
                        {
                            string wid = Marshal.PtrToStringAnsi(Native.SetPara(para, Convert.ToSingle(Width)));
                            return Width.ToString();
                        }
                        catch
                        {
                            Width = 0;
                            return Width.ToString();
                        }
                      

                    case "Height":
                        try
                        {
                            string hei = Marshal.PtrToStringAnsi(Native.SetPara(para, Convert.ToSingle(Height)));
                            return Height.ToString();
                        }
                        catch
                        {
                            Height = 0;
                            return Height.ToString();
                        }
                      

                    case "OffSetX":
                        try
                        {
                            string ofx = Marshal.PtrToStringAnsi(Native.SetPara(para, OffSetX));
                            return OffSetX.ToString();
                        }
                        catch
                        {
                            OffSetX = 0;
                            return OffSetX.ToString();
                        }
                       

                    case "OffSetY":
                        try
                        {
                            string ofy = Marshal.PtrToStringAnsi(Native.SetPara(para, OffSetY));
                            return OffSetY.ToString();
                        }
                        catch
                        {
                            OffSetY = 0;
                            return OffSetY.ToString();
                        }
                       

                    case "Fps":
                        return Fps.ToString();

                    default:
                        return $"Unknown parameter: {para}";
                }
            }
            catch (Exception ex)
            {
                return $"Error: {ex.Message}";
            }
        }

         public void SetAllValueModel()
        {
            try
            {
                Native.SetPara("Exposure", Exposure);
                Native.SetPara("Gamma", Gamma);
                Native.SetPara("Gain", Gain);
                Native.SetPara("Briness", Briness);
                Native.SetPara("Width", Width);
                Native.SetPara("Height", Height);
                Native.SetPara("OffSetX", OffSetX);
                Native.SetPara("OffSetY", OffSetY);
               // Console.WriteLine("Load thông số model thành công");
            }
           catch 
            {
                Console.WriteLine("Lỗi khi load para model");
            }
        }
    }
}
