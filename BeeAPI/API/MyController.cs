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
using System.Reflection;
using System.Web.Caching;
using System.Threading;
using System.Threading.Tasks;
namespace BeeAPI.Controllers
{
    
    [RoutePrefix("api/bee")]
    public class MyController : ApiController
    {
        private static readonly object _lock = new object();
        [HttpGet]
        [Route("SetVision")]
        public IHttpActionResult SetVision( string vision,  string value)
        {
            if (string.IsNullOrWhiteSpace(vision) || string.IsNullOrWhiteSpace(value))
            {
                return BadRequest("Tên thuộc tính vision và giá trị là bắt buộc và không được để trống." );
            }
            lock (this)
            {
                string result = Global.model.Vision.SetVision(vision, value);

                if (result == "Parameter không hợp lệ" || result.Contains("không hợp lệ"))
                {
                    return BadRequest(result);
                }

                return Ok(new { message = result });
            }
        }
        [HttpGet]
        [Route("SetPara")]
        public IHttpActionResult SetPara( string para,  string value)
        {
            lock (this)
            {
                var result = Global.model.CCD.SetParaA(para, value);

                return Ok(new { message = result ?? "Không nhận được phản hồi từ SetPara" });
            }
        }
        [HttpGet]
        [Route("GetVision")]
        public IHttpActionResult GetVision(string vision)
        {
            if (string.IsNullOrWhiteSpace(vision))
            {
                return BadRequest("Tên thuộc tính vision là bắt buộc và không được để trống.");
            }
            lock (this)
            {
                string result = Global.model.Vision.GetVision(vision);

                if (result == "Parameter không hợp lệ")
                {
                    return BadRequest("Thuộc tính không hợp lệ.");
                }

                return Ok(new { parameter = vision, value = result });
            }
        }
        [HttpGet]
        [Route("GetParaModel")]
        public IHttpActionResult GetParaModel( string para)
        {
            try
            {
                lock (this)
                {
                    var result = Global.model.CCD.GetParaCCD(para);
                    return Ok(new { parameter = para, value = result });
                }
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
                lock (this)
                {
                    var result = Global.model.CCD.GetParaA(para);
                    return Ok(new { parameter = para, value = result });
                }
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
                lock (this)
                {
                    Global.model.SaveModel(nameModel);
                    return Ok(new { message = $"Lưu mô hình thành công với tên {nameModel}." });
                }
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
                lock (this)
                {
                    Global.model = Global.model.LoadModel(nameModel);
                    Global.model.CCD.SetAllValueModel();
                    if (Global.model == null)
                    {
                        return StatusCode(System.Net.HttpStatusCode.NotFound); // ($"Không tìm thấy tệp dữ liệu cho mô hình '{nameModel}'." );
                    }


                    return Ok(new { value = Global.model });
                }
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
                lock (this)
                {
                    var result = Global.GIL.IniGIL();
                        if (result.Contains("SUCCESS"))
                            return Ok(new { message = "Python initialized and connected successfully." });
                        else
                            return StatusCode(System.Net.HttpStatusCode.RequestTimeout);
                   
                }
            
                return Ok("");
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
            string result = Global.GIL.TestYolo(Score);
            string[] numbers = result.Split(',');
            if (numbers.Length > 2)
            {
                Global.model.Result.Wires = long.Parse(numbers[0]);
                Global.model.Result.Counter = int.Parse(numbers[1]);
                Global.model.Result.Cycle = int.Parse(numbers[2]);
                Console.WriteLine($"Response: {result}");
                return Ok(new { value = Global.model.Result });
                
            }
          
                Console.WriteLine($"Response: {result}");
                return Ok(result);
            
           
          
          
       
        }

        [HttpGet]
        [Route("GrabCheck")]
        public IHttpActionResult GrabCheck()
        {
            try
            {
                    
                   
                    //  Native.GrabBasler();
                   
               lock(this)
                {
                   
                        Global.CCD.GrabBasler();
                        string result = Global.GIL.CheckYolo(Global.model.Vision.Score);

                    string[] numbers = result.Split(',');
                    if (numbers.Length > 2)
                    {
                        Global.model.Result.Wires = long.Parse(numbers[0]);
                        Global.model.Result.Counter = int.Parse(numbers[1]);
                        Global.model.Result.Cycle = int.Parse(numbers[2]);
                        return Ok(new { value = Global.model.Result });
                    }
                    else
                    {
                        Console.WriteLine(result);
                        return Ok(new { value = result });

                      

                    }
             
        }
                return Ok("");




            }
            catch (Exception ex) 
            {
                return BadRequest($"Lỗi khi chụp camera: {ex.Message}");
            }
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
        HttpResponseMessage response;
        bool IsRead = false;
        [HttpGet]
        [Route("GrabRaw")]

        public async Task< IHttpActionResult> GrabRaw()
        {
            lock (this)
            {
                if (IsRead)
                    return Ok("Wait");
                IsRead = true;

                //Native.GrabBasler();
                Global.CCD.GrabBasler();

                byte[] imageData = GetImg.ByteRaw();

                if (imageData == null || imageData.Length == 0)
                {
                    IsRead = false;
                    return BadRequest("Không có dữ liệu hình ảnh.");
                }


                try
                {
                    HttpResponseMessage response = new HttpResponseMessage(HttpStatusCode.OK);
                 //     HttpResponseMessage response = await base.SendAsync(request, cancellationToken);

                    // Thiết lập các header để vô hiệu hóa cache
                    response.Headers.CacheControl = new System.Net.Http.Headers.CacheControlHeaderValue
                    {
                        NoCache = true,
                        NoStore = true,
                        MustRevalidate = true
                    };
                    response.Headers.Pragma.Add(new System.Net.Http.Headers.NameValueHeaderValue("no-cache"));
                    response.Headers.Add("Expires", "0");
                    response.Content = new ByteArrayContent(imageData);
                
                    //IsRead = false;
                    response.Content.Headers.ContentType = new System.Net.Http.Headers.MediaTypeHeaderValue("image/jpeg");
                    // Hủy tham chiếu đối tượng để garbage collector thu hồi
                    //    mediaType = null;

                    // Thực thi garbage collection (chưa chắc sẽ thu hồi ngay)
                    return ResponseMessage(response);

                }
                finally
                {
                    GC.Collect();
                    GC.WaitForPendingFinalizers();
                    Console.WriteLine("Release");
                    // Giải phóng tài nguyên khi xong
                    //   // Nếu đối tượng implement IDisposable
                }
            }
          //  return result;
              //  return Ok(imageData);
            
        }
        [HttpGet]
        [Route("ScanCam")]
        public IHttpActionResult ScanCam()
        {
            try
            {
                lock (this)
                {
                    List<string> cameraList = Global.CCD.ScanBasler();
                    if (cameraList == null || cameraList.Count == 0)
                    {
                        return StatusCode(System.Net.HttpStatusCode.NotFound);// new { message = "Không tìm thấy camera nào." } );
                    }
                    return Ok(new { cameras = cameraList });
                }
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
                lock (this)
                {
                    string result = Global.CCD.ConnectBasler(nameCam);
                    return Ok(new { message = result ?? "Đã kết nối camera thành công." });
                }
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
