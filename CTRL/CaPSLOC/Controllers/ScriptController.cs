using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using CaPSLOC.Models;
using System.Xml.Serialization;
using System.IO;
using System.Xml;
using System.Text;
using CaPSLOC.Services;
using System.Net;
using System.Web.Script.Serialization;

namespace CaPSLOC.Controllers
{
    public class ScriptController : BaseController
    {
        //
        // GET: /Script/

        public ActionResult Index()
        {
            return View();
        }

        [HttpGet]
        public ActionResult All()
        {
            var result = Json(new { success = false, data = "An error has occurred." }, JsonRequestBehavior.AllowGet);
            try
            {
                var allScripts = DbModel.Scripts.Select(s => new { Name = s.Name, Id = s.Id }).ToList();
                result = Json(new { success = true, data = allScripts }, JsonRequestBehavior.AllowGet);
            }
            catch (Exception ex)
            {
                result = Json(new { success = false, data = ex.Message }, JsonRequestBehavior.DenyGet);
            }
            return result;
        }

        [HttpPost]
        public ActionResult Save(ScriptModel script)
        {
            using (DbModelContainer dbModel = new DbModelContainer())
            {
                // Ensure that this name is unique, if not, return error
                if (dbModel.Scripts.FirstOrDefault(s => s.Name == script.Name) != null)
                {
                    return Json(new { success = false, data = "Name is not unique!" }, JsonRequestBehavior.DenyGet);
                }

                // Write script, name, description, and whether to share to the database
                try
                {
                    XmlParsing parser = new XmlParsing();
                    string xml = parser.SerializeScript(script);

                    Script added = dbModel.Scripts.Add(new Script()
                    {
                        Name = script.Name,
                        Description = script.Description,
                        Content = xml
                    });
                    dbModel.SaveChanges();
                    // Return success
                    return Json(new { success = true, data = added.Id }, JsonRequestBehavior.DenyGet);
                }
                catch (Exception ex)
                {
                    return Json(new { success = false, data = ex.Message }, JsonRequestBehavior.DenyGet);
                }

            }
        }

        // This will come from the ALT, if it determines that it does not have the script it needs
        [HttpGet]
        public ActionResult RequestMissing(string scriptName)
        {
            try
            {
                Script script = DbModel.Scripts.SingleOrDefault(s => s.Name == scriptName);
                if (script == null)
                {
                    return Json(new { success = false, data = String.Format("Script '{0}' not found", scriptName) }, JsonRequestBehavior.AllowGet);
                }
                return Json(new { success = true, data = script.Content }, JsonRequestBehavior.AllowGet);
            }
            catch (Exception ex)
            {
                return Json(new { success = false, data = ex.Message }, JsonRequestBehavior.AllowGet);
            }
        }

        // This will come from the browser, to delete scripts from ALT
        [HttpPost]
        public ActionResult AddToALT(int altId, int scriptId)
        {
            try
            {
                ALT alt = DbModel.ALTs.SingleOrDefault(a => a.Id == altId);
                if (alt == null)
                {
                    return Json(new { success = false, data = "Selected ALT not found in database" }, JsonRequestBehavior.DenyGet);
                }
                if (!alt.RecentlyLocated)
                {
                    return Json(new { success = false, data = "Selected ALT not recently located. Please scan for ALTs" }, JsonRequestBehavior.DenyGet);
                }
                Script script = DbModel.Scripts.SingleOrDefault(s => s.Id == scriptId);
                if (script == null)
                {
                    return Json(new { success = false, data = "Selected script not found in database" }, JsonRequestBehavior.DenyGet);
                }

                // Transmit request to ALT
                WebRequest altRequest = WebRequest.CreateDefault(new Uri(String.Format("http://{0}/CaPSLOC/Script", alt.Address)));
                altRequest.ContentType = "application/json";
                altRequest.Method = "POST";

                JavaScriptSerializer serial = new JavaScriptSerializer();
                string json = serial.Serialize(new { name = script.Name, script = script.Content });

                byte[] jsonBytes = Encoding.UTF8.GetBytes(json);
                altRequest.ContentLength = jsonBytes.Length;
                Stream dataStream = altRequest.GetRequestStream();
                dataStream.Write(jsonBytes, 0, jsonBytes.Length);
                dataStream.Close();

                WebResponse resp = altRequest.GetResponse();
                Stream respStream = resp.GetResponseStream();

                string respString = new StreamReader(respStream).ReadToEnd();
                JavaScriptSerializer deserial = new JavaScriptSerializer();
                ScriptListModel respData = deserial.Deserialize<ScriptListModel>(respString);
                return Json(new { success = true, data = respData }, JsonRequestBehavior.AllowGet);               
            }
            catch (Exception ex)
            {
                return Json(new { success = false, data = ex.Message }, JsonRequestBehavior.AllowGet);
            }
        }

        // This will come from the browser, to delete scripts from ALT
        [HttpPost]
        public ActionResult DeleteFromALT(int altId, string scriptName)
        {
            try
            {
                ALT alt = DbModel.ALTs.SingleOrDefault(a => a.Id == altId);
                if (alt == null)
                {
                    return Json(new { success = false, data = "Selected ALT not found in database" }, JsonRequestBehavior.DenyGet);
                }
                if (!alt.RecentlyLocated)
                {
                    return Json(new { success = false, data = "Selected ALT not recently located. Please scan for ALTs" }, JsonRequestBehavior.DenyGet);
                }

                // Transmit request to ALT
                WebRequest altRequest = WebRequest.CreateDefault(new Uri(String.Format("http://{0}/CaPSLOC/Script?name={1}", alt.Address, scriptName)));
                altRequest.Method = "DELETE";

                WebResponse resp = altRequest.GetResponse();
                Stream respStream = resp.GetResponseStream();

                string respString = new StreamReader(respStream).ReadToEnd();
                JavaScriptSerializer deserial = new JavaScriptSerializer();
                ScriptListModel respData = deserial.Deserialize<ScriptListModel>(respString);
                return Json(new { success = true, data = respData }, JsonRequestBehavior.AllowGet);                
            }
            catch (Exception ex)
            {
                return Json(new { success = false, data = ex.Message }, JsonRequestBehavior.AllowGet);
            }
        }
    }
}
