using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System;
using System.Web.Http;
using BeeCplus;
using System.Net.Http;
using System.Net;
using OpenCvSharp;
using System.Web.Http.Results;
using System.Xml.Linq;
namespace BeeAPI.Controllers
{
    
    [RoutePrefix("api/bee")]
    public class MyController : ApiController
    {

        [HttpGet]
        [Route("SetVision")]
        public IHttpActionResult SetVision( string vision,  string value)
        {
            if (string.IsNullOrWhiteSpace(vision) || string.IsNullOrWhiteSpace(value))
            {
                return BadRequest("Tên thuộc tính vision và giá trị là bắt buộc và không được để trống." );
            }

            string result = Global.model.Vision.SetVision(vision, value);

            if (result == "Parameter không hợp lệ" || result.Contains("không hợp lệ"))
            {
                return BadRequest(result );
            }

            return Ok(new { message = result });
        }
        [HttpGet]
        [Route("SetPara")]
        public IHttpActionResult SetPara( string para,  string value)
        {
            var result = Global.model.CCD.SetParaA(para, value);

            return Ok(new { message = result ?? "Không nhận được phản hồi từ SetPara" } );
        }
        [HttpGet]
        [Route("GetVision")]
        public IHttpActionResult GetVision(string vision)
        {
            if (string.IsNullOrWhiteSpace(vision))
            {
                return BadRequest("Tên thuộc tính vision là bắt buộc và không được để trống.");
            }

            string result = Global.model.Vision.GetVision(vision);

            if (result == "Parameter không hợp lệ")
            {
                return BadRequest("Thuộc tính không hợp lệ.");
            }

            return Ok(new { parameter = vision, value = result });
        }
        [HttpGet]
        [Route("GetParaModel")]
        public IHttpActionResult GetParaModel( string para)
        {
            try
            {
                var result = Global.model.CCD.GetParaCCD(para);
                return Ok(new { parameter = para, value = result } );
            }
            catch
            {
                return BadRequest($"Sai định dạng" );
            }
        }
        [HttpGet]
        [Route("GetPara")]
        public IHttpActionResult GetPara( string para)
        {
            try
            {
                var result = Global.model.CCD.GetParaA(para);
                return Ok(new { parameter = para, value = result } );
            }
            catch
            {
                return BadRequest($"Sai định dạng" );
            }
        }
        [HttpGet]
        [Route("SaveModel")]
        public IHttpActionResult SaveModel( string nameModel)
        {
            if (string.IsNullOrWhiteSpace(nameModel))
            {
                return BadRequest("Tên của mô hình là bắt buộc và không được để trống." );
            }

            try
            {
                Global.model.SaveModel(nameModel);
                return Ok(new { message = $"Lưu mô hình thành công với tên {nameModel}." } );
            }
            catch (UnauthorizedAccessException)
            {
                return StatusCode(System.Net.HttpStatusCode.Unauthorized);// 403, "Không có quyền ghi vào thư mục được chỉ định." );
            }
            catch (Exception ex)
            {
                return BadRequest($"Lưu mô hình thất bại: {ex.Message}" );
            }
        }
        [HttpGet]
        [Route("LoadModel")]
        public IHttpActionResult LoadModel( string nameModel)
        {
            if (string.IsNullOrWhiteSpace(nameModel))
            {
                return BadRequest("Tên của mô hình là bắt buộc và không được để trống." );
            }
            try
            {
                Global.model = Global.model.LoadModel(nameModel);

                if (Global.model == null)
                {
                    return StatusCode(System.Net.HttpStatusCode.NotFound); // ($"Không tìm thấy tệp dữ liệu cho mô hình '{nameModel}'." );
                }

               
                return Ok(new { message = $"Tải mô hình thành công với tên {nameModel}.", model = Global.model });
            }
            catch (UnauthorizedAccessException)
            {
                return StatusCode(System.Net.HttpStatusCode.Unauthorized);//, "Không có quyền truy cập vào thư mục được chỉ định." );
            }
            catch (Exception ex)
            {
                return BadRequest($"Tải mô hình thất bại: {ex.Message}" );
            }
        }
        [HttpGet]
        [Route("InitialPython")]
        public IHttpActionResult IniGIL()
        {
            try
            {
                var result = Global.GIL.IniGIL();
                if(result.Contains("SUCCESS"))
                return Ok(new { message = "Python initialized and connected successfully." } );
              else
                  return StatusCode(System.Net.HttpStatusCode.RequestTimeout);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.ToString());
                return BadRequest($"Error Python: {ex.Message}" );
            }
        }
        [HttpGet]
        [Route("TestYolo")]
        public IHttpActionResult TestYolo(float Score)
        {
            string value = Global.GIL.TestYolo(Score);
            Console.WriteLine($"Response: {value}");
            return Ok(new { value = value });
        }

        [HttpGet]
        [Route("GrabCheck")]
        public IHttpActionResult GrabCheck()
        {
            Native.GrabBasler();

            return Ok(new { value = Global.GIL.CheckYolo(Global.model.Vision.Score)});
        }
        [HttpGet]
        [Route("ImageResult")]
        public IHttpActionResult ImageResult()
        {
            byte[] imageData = GetImg.ByteResult();

            if (imageData == null || imageData.Length == 0)
            {
                return BadRequest("Không có dữ liệu hình ảnh.");
            }
            HttpResponseMessage response = new HttpResponseMessage(HttpStatusCode.OK)
            {
                Content = new ByteArrayContent(imageData)
            };

            response.Content.Headers.ContentType = new System.Net.Http.Headers.MediaTypeHeaderValue("image/jpeg");

            return ResponseMessage(response);
        }
        [HttpGet]
        [Route("ImageRaw")]
        public IHttpActionResult ImageRaw()
        {
            byte[] imageData = GetImg.ByteRaw();

            if (imageData == null || imageData.Length == 0)
            {
                return BadRequest("Không có dữ liệu hình ảnh.");
            }
            HttpResponseMessage response = new HttpResponseMessage(HttpStatusCode.OK)
            {
                Content = new ByteArrayContent(imageData)
            };

            response.Content.Headers.ContentType = new System.Net.Http.Headers.MediaTypeHeaderValue("image/jpeg");

            return ResponseMessage(response);
        }
        [HttpGet]
        [Route("GrabRaw")]
        public IHttpActionResult GrabRaw()
        {
            Native.GrabBasler();
            byte[] imageData = GetImg.ByteRaw();

            if (imageData == null || imageData.Length == 0)
            {
                return BadRequest("Không có dữ liệu hình ảnh.");
            }
            HttpResponseMessage response = new HttpResponseMessage(HttpStatusCode.OK)
            {
                Content = new ByteArrayContent(imageData)
            };
            response.Content.Headers.ContentType = new System.Net.Http.Headers.MediaTypeHeaderValue("image/jpeg");
            return ResponseMessage(response);
        }
        [HttpGet]
        [Route("ScanCam")]
        public IHttpActionResult ScanCam()
        {
            try
            {
                List<string> cameraList = Global.CCD.ScanBasler();
                if (cameraList == null || cameraList.Count == 0)
                {
                    return StatusCode(System.Net.HttpStatusCode.NotFound);// new { message = "Không tìm thấy camera nào." } );
                }
                return Ok(new { cameras = cameraList });
            }
            catch (Exception ex)
            {
                return BadRequest($"Lỗi khi quét camera: {ex.Message}" );
            }
        }
        [HttpGet]
        [Route("ConnectCam")]
        public IHttpActionResult ConnectCam( string nameCam)
        {
            if (string.IsNullOrWhiteSpace(nameCam))
            {
                return BadRequest("Tên camera là bắt buộc và không được để trống." );
            }
            try
            {
                string result = Global.CCD.ConnectBasler(nameCam);
                return Ok(new { message = result ?? "Đã kết nối camera thành công." });
            }
            catch (Exception ex)
            {
                return BadRequest($"Lỗi khi kết nối camera: {ex.Message}" );
            }
        }
        [HttpGet]
        [Route("DisconnectCam")]
        public IHttpActionResult DisconnectCam()
        {
            try
            {
               
                return Ok(new { message = Global.CCD.DisconnectBasler()});
            }
            catch (Exception ex)
            {
                return BadRequest($"Lỗi khi ngắt kết nối camera: {ex.Message}" );
            }
        }
       
    }
}
