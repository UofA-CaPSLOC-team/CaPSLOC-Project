using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;

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


        public Result Save(XmlBlob script, string name, string description, bool share)
        {
            // Ensure that this name is unique, if not, return error

            // Write script, name, description, and whether to share to the database

            // Return success
        }

    }
}
