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
using System.Threading;
using System.Web;

namespace CaPSLOC.Controllers
{
    public class ALTController : BaseController
    {
        public class PingRequestState
        {
            public ManualResetEvent doneSignal;
            public WebRequest request;
            public Stream responseStream;
            public string ipAddress;
            public byte[] readBuffer;
            public PingRequestState()
            {
                readBuffer = new byte[1024];
            }
        }

        public List<string> PingReplies { get; set; }


        [HttpPost] // From browser
        public ActionResult PingSweep(string startIP, string endIP)
        {
            try
            {
                // Initialize reply list
                PingReplies = new List<string>();


                // Clear list of active ALTs
                foreach (ALT a in DbModel.ALTs)
                {
                    a.RecentlyLocated = false;
                }

                UInt32 startIpInt = ParseIP(startIP);
                UInt32 endIpInt = ParseIP(endIP);

                // List of wait handles associated with the ALT pings
                List<ManualResetEvent> pingMREs = new List<ManualResetEvent>();

                for (uint ip = startIpInt; ip <= endIpInt; ip++)
                {
                    string ipString = IPtoString(ip);
                    // Send 'ping' on ALT communication TCP port

                    WebRequest altPing = WebRequest.CreateDefault(new Uri(String.Format("http://{0}/CaPSLOC/Status", ipString)));
                    try
                    {
                        PingRequestState requestState = new PingRequestState();
                        ManualResetEvent mre = new ManualResetEvent(false);
                        requestState.doneSignal = mre;
                        requestState.request = altPing;
                        requestState.ipAddress = ipString;
                        pingMREs.Add(mre);

                        Logger.Debug(String.Format("Beginning ping to ALT at IP address '{0}'", ipString));
                        IAsyncResult pingResp = altPing.BeginGetResponse(new AsyncCallback(PingResponseCallback), requestState);
                    }
                    catch (Exception ex)
                    {
                        // Some form of tracking timeouts, etc.
                        Logger.Error(String.Format("Error when sending to IP address {0}:\n {1}", ipString, ex.Message));
                    }
                }

                WaitHandle.WaitAll(pingMREs.ToArray());
                Logger.Debug("All pings have returned. Continuing...");
                Logger.Debug(String.Format("{0} ALT units found", PingReplies.Count));

                DbModel.SaveChanges();

                return Json(new { success = true, data = DbModel.ALTs.Where(a => a.RecentlyLocated) }, JsonRequestBehavior.DenyGet);
            }
            catch (Exception ex)
            {
                return Json(new { success = false, data = ex.Message }, JsonRequestBehavior.DenyGet);
            }
        }

        private void PingResponseCallback(IAsyncResult respResult)
        {
            PingRequestState requestState = respResult.AsyncState as PingRequestState;
            try
            {               
                Logger.Debug(String.Format("Ping for IP address '{0}' returned", requestState.ipAddress));
                WebRequest request = requestState.request;
                WebResponse response = request.EndGetResponse(respResult);

                Stream respStream = response.GetResponseStream();
                requestState.responseStream = respStream;
                IAsyncResult pingResp = respStream.BeginRead(requestState.readBuffer, 0, 1024, new AsyncCallback(PingResponseReadCallback), requestState);                
            }
            catch (Exception ex)
            {
                Logger.Error(String.Format("Error occurred in ping response for '{0}': \n\t{1}", requestState.ipAddress, ex.Message));
                requestState.doneSignal.Set(); // If there is an error, we need to signal this ping complete, so it will return
            }
        }

        private void PingResponseReadCallback(IAsyncResult readResult)
        {
            PingRequestState requestState = readResult.AsyncState as PingRequestState;
            try
            {
                int bytesRead = requestState.responseStream.EndRead(readResult);
                // TODO - Change to format of ping reply
                string nameString = Encoding.UTF8.GetString(requestState.readBuffer, 0, bytesRead);
                Logger.Debug(String.Format("ALT at IP address '{0}' is named '{1}'", requestState.ipAddress, nameString));
                PingReplies.Add(nameString);

                ALT existingALT = DbModel.ALTs.SingleOrDefault(a => a.Name == nameString);  // Update existing entry if one exists
                if (existingALT != null)
                {
                    existingALT.Name = nameString;
                    existingALT.Address = requestState.ipAddress;
                    existingALT.RecentlyLocated = true;
                }
                else
                {
                    DbModel.ALTs.Add(new ALT()
                    {
                        Name = nameString,
                        Address = requestState.ipAddress,
                        RecentlyLocated = true
                    });
                }
                // Changes will be saved in the calling action method (to ensure everything completes successfully first)
            }
            finally
            {
                requestState.doneSignal.Set();
            }
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
                    altRequest.ContentType = "text/xml";
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

        // From ALT
        [HttpPost]
        public ActionResult Debug(DebugMessageModel debugMessage)
        {
            var result = Json(new { success = false }, JsonRequestBehavior.DenyGet);

            List<DebugMessageModel> cachedMessages = HttpRuntime.Cache["AppDebugInfo"] as List<DebugMessageModel>;
            if (cachedMessages == null)
            {
                cachedMessages = new List<DebugMessageModel>();
            }

            // Clean out old messages
            cachedMessages = cachedMessages.Where(c => c.Time > DateTime.Now.AddMinutes(-10)).ToList();

            // Timestamp the message, then add it to the list
            debugMessage.Time = DateTime.Now;
            cachedMessages.Add(debugMessage);

            HttpRuntime.Cache["AppDebugInfo"] = cachedMessages;

            return Json(new { success = false }, JsonRequestBehavior.DenyGet);
        }

        // From Browser
        [HttpGet]
        public ActionResult Debug(int AltId)
        {
            var result = Json(new { success = false }, JsonRequestBehavior.AllowGet);

            List<DebugMessageModel> cachedMessages = HttpRuntime.Cache["AppDebugInfo"] as List<DebugMessageModel>;
            if (cachedMessages != null)
            {
                if (AltId != 0)
                {
                    var alt = DbModel.ALTs.FirstOrDefault(a => a.Id == AltId);
                    var name = alt != null ? alt.Name : String.Empty;
                    cachedMessages = cachedMessages.Where(c => c.AltName == name).ToList();
                }

                result = Json(new { success = true, data = cachedMessages }, JsonRequestBehavior.AllowGet);
            }

            return result;
        }

        [HttpGet]
        public ActionResult RecentlyLocated()
        {
            try
            {
                using (DbModelContainer models = new DbModelContainer())
                {
                    IQueryable<ALT> alts = models.ALTs.Where(a => a.RecentlyLocated).OrderBy(a => a.Name);

                    return Json(new { success = true, data = alts.ToList() }, JsonRequestBehavior.AllowGet);
                }
            }
            catch (Exception ex)
            {
                return Json(new { success = false, data = ex.Message }, JsonRequestBehavior.AllowGet);
            }
        }

        [HttpGet]
        public ActionResult All()
        {
            try
            {
                using (DbModelContainer models = new DbModelContainer())
                {
                    IQueryable<ALT> alts = models.ALTs.OrderBy(a => a.Name);

                    return Json(new { success = true, data = alts.ToList() }, JsonRequestBehavior.AllowGet);
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
