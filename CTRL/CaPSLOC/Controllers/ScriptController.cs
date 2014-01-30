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

namespace CaPSLOC.Controllers
{
    public class ScriptController : Controller
    {
        //
        // GET: /Script/

        public ActionResult Index()
        {
            return View();
        }


        [HttpPost]
        public ActionResult Save(ScriptModel script)
        {
            XmlSerializer serializer = new XmlSerializer(typeof(ScriptModel));
            StringWriter writer = new StringWriter();
            serializer.Serialize(writer, script);
            string xml = SerializeScript(script);
            writer.Close();
            // Ensure that this name is unique, if not, return error

            // Write script, name, description, and whether to share to the database

            // Return success
            return Json(new { success = true, data = 1 }, JsonRequestBehavior.DenyGet);
        }

        private string SerializeScript(ScriptModel script)
        {
            StringBuilder builder = new StringBuilder();
            XmlWriter writer = XmlWriter.Create(builder);
            writer.WriteStartDocument();
            writer.WriteStartElement("script");
            writer.WriteAttributeString("name", script.Name);
            foreach (ScriptCommandModel command in script.Commands)
            {

            }
            writer.WriteEndDocument();  // Close remaining tags

            return builder.ToString();
        }

    }
}
