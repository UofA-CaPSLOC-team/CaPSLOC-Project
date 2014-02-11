using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using System.Text;
using Newtonsoft.Json;
using NLog;

namespace CaPSLOC.Controllers
{
    public class BaseController : Controller
    {
        public Logger Logger { get; set; }

        public BaseController()
            : base()
        {
            Logger = LogManager.GetCurrentClassLogger();
            Logger.Debug("Controller initialization successful");
        }
    }
    //    protected internal JsonResult JsonNet(object data)
    //    {
    //        return new JsonNetResult { Data = data };
    //    }

    //    protected internal JsonResult JsonNet(object data, JsonRequestBehavior behavior)
    //    {
    //        return new JsonNetResult { Data = data, JsonRequestBehavior = behavior };
    //    }

    //}

    //public class JsonNetResult : JsonResult
    //{
    //    //public Encoding ContentEncoding { get; set; }
    //    //public string ContentType { get; set; }
    //    //public object Data { get; set; }

    //    public JsonSerializerSettings SerializerSettings { get; set; }
    //    public Formatting Formatting { get; set; }

    //    public JsonNetResult()
    //    {
    //        SerializerSettings = new JsonSerializerSettings();
    //    }

    //    public override void ExecuteResult(ControllerContext context)
    //    {
    //        if (context == null)
    //            throw new ArgumentNullException("context");

    //        HttpResponseBase response = context.HttpContext.Response;

    //        response.ContentType = !string.IsNullOrEmpty(ContentType)
    //          ? ContentType
    //          : "application/json";

    //        if (ContentEncoding != null)
    //            response.ContentEncoding = ContentEncoding;

    //        if (Data != null)
    //        {
    //            JsonTextWriter writer = new JsonTextWriter(response.Output) { Formatting = Formatting };

    //            JsonSerializer serializer = JsonSerializer.Create(SerializerSettings);
    //            serializer.Serialize(writer, Data);

    //            writer.Flush();
    //        }
    //    }
    //}
}
