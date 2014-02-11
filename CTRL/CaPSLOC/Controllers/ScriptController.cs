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

    }
}
