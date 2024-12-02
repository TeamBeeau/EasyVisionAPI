using Microsoft.Owin;
using Owin;
using System.Net.Http;
using System;
using System.Web.Http;
using System.Threading;
using System.Net.Http.Headers;
using System.Threading.Tasks;

[assembly: OwinStartup(typeof(BeeAPI.Startup))]

namespace BeeAPI
{


public class NoCacheHandler : DelegatingHandler
    {
        protected override async Task<HttpResponseMessage> SendAsync(HttpRequestMessage request, CancellationToken cancellationToken)
        {
            // Gọi tiếp các handler khác trong pipeline
            HttpResponseMessage response = await base.SendAsync(request, cancellationToken);

            // Thiết lập các header để vô hiệu hóa cache
            response.Headers.CacheControl = new System.Net.Http.Headers.CacheControlHeaderValue
            {
                NoCache = true,
                NoStore = true,
                MustRevalidate = true
            };
            response.Headers.Pragma.Add(new System.Net.Http.Headers.NameValueHeaderValue("no-cache"));
            response.Headers.Add("Expires", "0");

            return response;
        }
    }
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
          //  config.MessageHandlers.Add(new NoCacheHandler());
            app.UseWebApi(config);
            
           
        }
   
    }
}
