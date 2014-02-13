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
                if (DbModel.Locations.FirstOrDefault(l => l.Name == loc.Name) != null)
                {
                    result = Json(new { success = false, data = "Name is not unique!" });
                    return result;
                }

                DbModel.Locations.Add(loc);
                DbModel.SaveChanges();

                result = Json(new { success = true, data = loc.Id });
            }
            catch (Exception ex)
            {
                result = Json(new { success = false, data = ex.Message });
            }

            return result;
        }

        [HttpGet]
        public JsonResult AllLocations()
        {
            JsonResult result = Json(new { success = false, data = "An error occurred." }, JsonRequestBehavior.AllowGet);

            try
            {
                var locations = DbModel.Locations.OrderBy(l => l.Name);

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
