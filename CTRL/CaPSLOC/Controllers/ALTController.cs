using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Web.Mvc;
using CaPSLOC.Models;
using CaPSLOC.Services;
using System.Text;
using System.IO;

namespace CaPSLOC.Controllers
{
    public class ALTController : Controller
    {
        //[HttpGet] // From browser
        //public void PingSweep(IPAddress start, IPAddress mask)
        //{
        //    // Clear list of active ALTs

        //    foreach( IP in range )
        //    {
        //        // Send 'ping' on ALT communication TCP port
        //    }
        //}

        //[HttpPost] // From ALT
        //public void PingReply(int AltId)
        //{
        //    // Add to list of active ALT units
        //}

        // From Browser
        [HttpPost]
        public ActionResult SendCommand(InteractiveCommandModel commandModel)
        {
            try
            {
                // Lookup ALT address in list
                using (DbModelContainer models = new DbModelContainer())
                {
                    ALT alt = models.ALTs.SingleOrDefault(a => a.Id == commandModel.AltId);
                    if (alt == null)
                    {
                        return Json(new { success = false, data = "Selected ALT not found in database" }, JsonRequestBehavior.DenyGet);
                    }
                    if (!alt.RecentlyLocated)
                    {
                        return Json(new { success = false, data = "Selected ALT not recently located. Please scan for ALTs" }, JsonRequestBehavior.DenyGet);
                    }

                    // Generate XML format of command
                    XmlParsing parser = new XmlParsing();
                    string xml = parser.SerializeInteractive(commandModel);

                    // Transmit XML to ALT
                    WebRequest altRequest = WebRequest.CreateDefault(new Uri(String.Format("http://{0}/CaPSLOC/Command", alt.Address)));
                    altRequest.ContentType = "application/xml";
                    altRequest.Method = "POST";

                    byte[] xmlBytes = Encoding.UTF8.GetBytes(xml);
                    altRequest.ContentLength = xmlBytes.Length;
                    Stream dataStream = altRequest.GetRequestStream();
                    dataStream.Write(xmlBytes, 0, xmlBytes.Length);
                    dataStream.Close();

                    WebResponse resp = altRequest.GetResponse();

                    // Inform UI that command was sent

                    return Json(new { success = true, data = "Sending command returned with code: " + ((HttpWebResponse)resp).StatusCode }, JsonRequestBehavior.DenyGet);
                }

            }
            catch (Exception ex)
            {
                return Json(new { success = false, data = ex.Message }, JsonRequestBehavior.DenyGet);
            }
        }

        ////POST - from ALT
        //public AltStatus ReceiveStatus(int AltId, string status)
        //{
        //    // Inform UI of status
        //}
    }
}
