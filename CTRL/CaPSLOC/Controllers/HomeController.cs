using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;

namespace CaPSLOC.Controllers
{
    public class HomeController : Controller
    {
        //
        // GET: /Home/

        public ActionResult Index()
        {
            return View();
        }

        public ActionResult SaveLocation(Location loc)
        {
            JsonResult result = Json(new { success = false, data = "An error occurred." });

            try
            {
                using (DbModelContainer model = new DbModelContainer())
                {
                    model.Locations.AddObject(loc);
                    model.SaveChanges();
                }
                result = Json(new { success = true, data = loc.Id });
            }
            catch(Exception ex)
            {
                result = Json(new { success = false, data = ex.Message });
            }

            return result;
        }

    }
}
