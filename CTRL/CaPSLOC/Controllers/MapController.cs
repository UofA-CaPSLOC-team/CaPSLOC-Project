using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using System.Data.Entity;

namespace CaPSLOC.Controllers
{
    public class MapController : BaseController
    {
        //
        // GET: /Home/

        public ActionResult Index()
        {
            return View();
        }

        [HttpPost]
        public ActionResult SaveLocation(Location loc)
        {
            JsonResult result = Json(new { success = false, data = "An error occurred." });

            try
            {
                using (DbModelContainer model = new DbModelContainer())
                {
                    model.Locations.Add(loc);
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

        [HttpGet]
        public JsonResult AllLocations()
        {
            JsonResult result = Json(new { success = false, data = "An error occurred." }, JsonRequestBehavior.AllowGet);

            using (DbModelContainer model = new DbModelContainer())
            {
                try
                {
                    var locations = model.Locations.AsEnumerable();

                    return result = Json(new { success = true, data = locations.ToList() }, JsonRequestBehavior.AllowGet);

                }
                catch (Exception ex)
                {
                    result = Json(new { success = false, data = ex.Message }, JsonRequestBehavior.AllowGet);
                }

                return result;
            }
        }

    }
}
