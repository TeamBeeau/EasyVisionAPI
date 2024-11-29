using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Web.Http;

namespace BeeAPI
{
    public static class WebApiConfig
    {
        public static void Register(HttpConfiguration config)
        {
           
            // Cấu hình routes cho Web API
            config.MapHttpAttributeRoutes();

            // Đăng ký route mặc định
            config.Routes.MapHttpRoute(
                name: "DefaultApi",
                routeTemplate: "api/{controller}/{id}",
                defaults: new { id = RouteParameter.Optional }
            );
        }
    }
}
