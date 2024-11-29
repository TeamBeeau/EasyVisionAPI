using Microsoft.Owin;
using Owin;
using System.Net.Http;
using System;
using System.Web.Http;
using System.Threading;

[assembly: OwinStartup(typeof(BeeAPI.Startup))]

namespace BeeAPI
{
    public class Startup
    {
        public async void Configuration(IAppBuilder app)
        {
            HttpConfiguration config = new HttpConfiguration();

            // Cấu hình các route Web API
            config.MapHttpAttributeRoutes();

            // Đăng ký route mặc định
            config.Routes.MapHttpRoute(
                name: "DefaultApi",
                routeTemplate: "api/{controller}/{id}",
                defaults: new { id = RouteParameter.Optional }
            );

            app.UseWebApi(config);
            
           
        }
    }
}
