using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

using BeeCen;
using BeeDotNet;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;

namespace BeeCen.Controllers
{
    [ApiController]
    [Route("[controller]")]
    //[Route("api")]
    public class WeatherForecastController : ControllerBase
    {
        private static readonly string[] Summaries = new[]
        {
            "Freezing", "Bracing", "Chilly", "Cool", "Mild", "Warm", "Balmy", "Hot", "Sweltering", "Scorching"
        };

        private readonly ILogger<WeatherForecastController> _logger;

        public WeatherForecastController(ILogger<WeatherForecastController> logger)
        {
            _logger = logger;
        }
        public static class StreamController
        {
            public static bool IsLive { get; set; } = false;
        }


        [HttpGet]
        public IEnumerable<WeatherForecast> Get()
        {
            var rng = new Random();
            return Enumerable.Range(1, 5).Select(index => new WeatherForecast
            {
                Date = DateTime.Now.AddDays(index),
                TemperatureC = rng.Next(-20, 55),
                Summary = Summaries[rng.Next(Summaries.Length)]
            })
            .ToArray();
        }
        [HttpGet("GetVision")]
        public IActionResult GetVision([FromQuery] string vision)
        {
            if (string.IsNullOrWhiteSpace(vision))
            {
                return BadRequest(new { error = "Tên thuộc tính vision là bắt buộc và không được để trống." });
            }

            string result = Global.model.Vision.GetVision(vision);

            if (result == "Parameter không hợp lệ")
            {
                return BadRequest(new { error = "Thuộc tính không hợp lệ." });
            }

            return Ok(new { parameter = vision, value = result });
        }

        [HttpPost("SetVision")]
        public IActionResult SetVision([FromQuery] string vision, [FromQuery] string value)
        {
            if (string.IsNullOrWhiteSpace(vision) || string.IsNullOrWhiteSpace(value))
            {
                return BadRequest(new { error = "Tên thuộc tính vision và giá trị là bắt buộc và không được để trống." });
            }

            string result = Global.model.Vision.SetVision(vision, value);

            if (result == "Parameter không hợp lệ" || result.Contains("không hợp lệ"))
            {
                return BadRequest(new { error = result });
            }

            return Ok(new { message = result });
        }

        [HttpPost("SetPara")]
        public IActionResult SetPara([FromQuery] string para, [FromQuery] string value)
        {
            var result = Global.model.CCD.SetParaA(para, value);

            return Ok(new { message = result ?? "Không nhận được phản hồi từ SetPara" });
        }
        [HttpGet("GetParaModel")]
        public IActionResult GetParaModel([FromQuery] string para)
        {
            try
            {
                var result = Global.model.CCD.GetParaModel(para);
                return Ok(new { parameter = para, value = result });
            }
            catch
            {
                return BadRequest(new { error = $"Sai định dạng" });
            }
        }
        [HttpGet("GetPara")]
        public IActionResult GetPara([FromQuery] string para)
        {
            try
            {
                var result = Global.model.CCD.GetParaA(para);
                return Ok(new { parameter = para, value = result });
            }
            catch
            {
                return BadRequest(new { error = $"Sai định dạng" });
            }
        }
        [HttpPost("SaveModel")]
        public IActionResult SaveModel([FromQuery] string nameModel)
        {
            if (string.IsNullOrWhiteSpace(nameModel))
            {
                return BadRequest(new { error = "Tên của mô hình là bắt buộc và không được để trống." });
            }

            try
            {
                Global.model.SaveModel(nameModel);
                return Ok(new { message = $"Lưu mô hình thành công với tên {nameModel}." });
            }
            catch (UnauthorizedAccessException)
            {
                return StatusCode(403, new { error = "Không có quyền ghi vào thư mục được chỉ định." });
            }
            catch (Exception ex)
            {
                return BadRequest(new { error = $"Lưu mô hình thất bại: {ex.Message}" });
            }
        }
        [HttpGet("LoadModel")]
        public IActionResult LoadModel([FromQuery] string nameModel)
        {
            if (string.IsNullOrWhiteSpace(nameModel))
            {
                return BadRequest(new { error = "Tên của mô hình là bắt buộc và không được để trống." });
            }

            try
            {
                Model loadedModel = Global.model.LoadModel(nameModel);

                if (loadedModel == null)
                {
                    return NotFound(new { error = $"Không tìm thấy tệp dữ liệu cho mô hình '{nameModel}'." });
                }

                Global.model = loadedModel;
                return Ok(new { message = $"Tải mô hình thành công với tên {nameModel}.", model = Global.model });
            }
            catch (UnauthorizedAccessException)
            {
                return StatusCode(403, new { error = "Không có quyền truy cập vào thư mục được chỉ định." });
            }
            catch (Exception ex)
            {
                return BadRequest(new { error = $"Tải mô hình thất bại: {ex.Message}" });
            }
        }


        [DllImport(@".\GigEBasler.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        unsafe public static extern IntPtr TestYolo();


        [HttpGet("InitialPython")]
        public IActionResult InitialBasler()
        {
            try
            {

                Global.Yolo.IniYolo();
                Global.Yolo.CheckY();
                    //Native.IniBasler();
                    //Native.TestYolo();
                    //value = Marshal.PtrToStringAnsi(TestYolo());
                
              
                //Global.model.CCD.InitialBasler();
                return Ok(new { message = "Python initialized and connected successfully." });
            }
            catch (Exception ex)
            {
                return BadRequest(new { error = $"Error Python: {ex.Message}" });
            }
        }
        GigEBasler.Yolo Yolo=new GigEBasler.Yolo();
        [HttpGet("GrabCheck")]
        public IActionResult GrabWCheck()
        {
            string value = "";

            lock (this)
            {
              
                //value = Marshal.PtrToStringAnsi(TestYolo());
            }
           
            return Ok(new { value = value });
                                                        
            //try
            //{
            //    byte[] imageData = Global.model.CCD.GrabWCheck();

            //    if (imageData == null || imageData.Length == 0)
            //    {
            //        return BadRequest(new { error = "Không có dữ liệu hình ảnh." });
            //    }

            //    return File(imageData, "image/png");
            //}
            //catch (Exception ex)
            //{
            //    return BadRequest(new { error = $"Lỗi khi thực thi GrapImage: {ex.Message}" });
            //}
        }




        [HttpGet("ScanCam")]
        public IActionResult ScanCam()
        {
            try
            {
                List<string> cameraList = Global.model.CCD.ScanCam();

                if (cameraList == null || cameraList.Count == 0)
                {
                    return NotFound(new { message = "Không tìm thấy camera nào." });
                }

                return Ok(new { cameras = cameraList });
            }
            catch (Exception ex)
            {
                return BadRequest(new { error = $"Lỗi khi quét camera: {ex.Message}" });
            }
        }
        [HttpPost("ConnectCam")]
        public IActionResult ConnectCam([FromQuery] string nameCam)
        {
            if (string.IsNullOrWhiteSpace(nameCam))
            {
                return BadRequest(new { error = "Tên camera là bắt buộc và không được để trống." });
            }

            try
            {
                string result = Global.model.CCD.ConnectCam(nameCam);
                return Ok(new { message = result ?? "Đã kết nối camera thành công." });
            }
            catch (Exception ex)
            {
                return BadRequest(new { error = $"Lỗi khi kết nối camera: {ex.Message}" });
            }
        }
        [HttpPost("DisconnectCam")]
        public IActionResult DisconnectCam()
        {
            try
            {
                Global.model.CCD.DisconnectCam();
                return Ok(new { message = "Đã ngắt kết nối camera thành công." });
            }
            catch (Exception ex)
            {
                return BadRequest(new { error = $"Lỗi khi ngắt kết nối camera: {ex.Message}" });
            }
        }
        [HttpGet("GrapImage")]
        public IActionResult GrapImage()
        {
            try
            {
                byte[] imageData = Global.model.CCD.GrapImage();

                if (imageData == null || imageData.Length == 0)
                {
                    return BadRequest(new { error = "Không có dữ liệu hình ảnh." });
                }

                return File(imageData, "image/png");
            }
            catch (Exception ex)
            {
                return BadRequest(new { error = $"Lỗi khi thực thi GrapImage: {ex.Message}" });
            }
        }
        //[HttpPost("InsertData")]
        //public IActionResult InsertData([FromQuery] string dataModel, [FromQuery] string dataPO)
        //{
        //    if (string.IsNullOrWhiteSpace(dataModel) || string.IsNullOrWhiteSpace(dataPO))
        //        return BadRequest(new { error = "Both dataModel and dataPO are required." });

        //    try
        //    {
        //        Global.model.DB.InsertData(dataModel, dataPO);
        //        return Ok(new { message = "Data inserted successfully." });
        //    }
        //    catch (Exception ex)
        //    {
        //        _logger.LogError(ex, "Error inserting data.");
        //        return BadRequest(new { error = ex.Message });
        //    }
        //}
        //[HttpPost("UpdateData")]
        //public IActionResult UpdateData([FromQuery] string dataModel, [FromQuery] string dataPO)
        //{
        //    if (string.IsNullOrWhiteSpace(dataModel) || string.IsNullOrWhiteSpace(dataPO))
        //        return BadRequest(new { error = "Both dataModel and dataPO are required." });

        //    try
        //    {
        //        Global.model.DB.UpdateData(dataModel, dataPO);
        //        return Ok(new { message = "Data updated successfully." });
        //    }
        //    catch (Exception ex)
        //    {
        //        _logger.LogError(ex, "Error updating data.");
        //        return BadRequest(new { error = ex.Message });
        //    }
        //}
        //[HttpGet("SelectData")]
        //public IActionResult SelectData([FromQuery] string dataModel, [FromQuery] string dataPO)
        //{
        //    if (string.IsNullOrWhiteSpace(dataModel) || string.IsNullOrWhiteSpace(dataPO))
        //        return BadRequest(new { error = "Both dataModel and dataPO are required." });

        //    try
        //    {
        //        string result = Global.model.DB.SelectData(dataModel, dataPO);
        //        return Ok(new { data = result });
        //    }
        //    catch (Exception ex)
        //    {
        //        _logger.LogError(ex, "Error selecting data.");
        //        return BadRequest(new { error = ex.Message });
        //    }
        //}


    }
}

       