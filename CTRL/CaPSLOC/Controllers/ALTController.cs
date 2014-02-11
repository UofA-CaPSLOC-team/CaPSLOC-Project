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
using System.Threading.Tasks;

namespace CaPSLOC.Controllers
{
    public class ALTController : BaseController
    {
        [HttpPost] // From browser
        public ActionResult PingSweep(string startIP, string endIP)
        {
            using (DbModelContainer models = new DbModelContainer())
            {
                // Clear list of active ALTs
                foreach (ALT a in models.ALTs)
                {
                    a.RecentlyLocated = false;
                }

                UInt32 startIpInt = ParseIP(startIP);
                UInt32 endIpInt = ParseIP(endIP);

                for (uint ip = startIpInt; ip <= endIpInt; ip++)
                {
                    string ipString = IPtoString(ip);
                    // Send 'ping' on ALT communication TCP port

                    WebRequest altPing = WebRequest.CreateDefault(new Uri(String.Format("http://{0}/CaPSLOC/Status", ipString)));
                    try
                    {
                        WebResponse pingResp = altPing.GetResponse();
                        Stream nameStream = pingResp.GetResponseStream();
                        byte[] nameBytes = new byte[nameStream.Length];
                        nameStream.Read(nameBytes, 0, (int)nameStream.Length);
                        string nameString = Encoding.UTF8.GetString(nameBytes);

                        models.ALTs.Add(new ALT()
                        {
                            Name = nameString,
                            Address = ipString,
                            RecentlyLocated = true
                        });
                    }
                    catch (Exception ex)
                    {
                        // Some form of tracking timeouts, etc.
                        System.Diagnostics.Debug.WriteLine(String.Format("Error when sending to IP address {0}:\n {1}", ipString, ex.Message));
                    }
                }

                models.SaveChanges();
            }

            return Json(new { success = false, data = "An unknown error has occurred" }, JsonRequestBehavior.DenyGet);
        }

        private UInt32 ParseIP(string ipAddr)
        {
            string[] ipParts = ipAddr.Split(new char[] { '.' });
            UInt32 ipInt = 0;
            foreach (string part in ipParts)
            {
                uint intPart = UInt32.Parse(part);
                ipInt = ipInt << 8;
                ipInt = ipInt | intPart;
            }
            return ipInt;
        }

        private string IPtoString(UInt32 ipAddr)
        {
            return string.Format("{0}.{1}.{2}.{3}", (ipAddr & 0xFF000000) >> 24, (ipAddr & 0xFF0000) >> 16, (ipAddr & 0xFF00) >> 8, ipAddr & 0xFF);
        }

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

        [HttpGet]
        public ActionResult ListALTs()
        {
            try
            {
                using (DbModelContainer models = new DbModelContainer())
                {
                    IQueryable<ALT> alts = models.ALTs.Where(a => a.RecentlyLocated);

                    return Json(new { success = true, data = alts.ToList() }, JsonRequestBehavior.DenyGet);
                }
            }
            catch (Exception ex)
            {
                return Json(new { success = false, data = ex.Message }, JsonRequestBehavior.AllowGet);
            }
        }

        ////POST - from ALT
        //public AltStatus ReceiveStatus(int AltId, string status)
        //{
        //    // Inform UI of status
        //}
    }
}
