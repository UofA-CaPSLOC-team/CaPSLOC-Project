using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using System.Data.Entity;
using System.Net;
using System.IO;
using System.Web.Script.Serialization;
using CaPSLOC.Models;

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

                // Clean up the numbers
                loc.Latitude = Math.Round(loc.Latitude, 6);
                loc.Longitude = Math.Round(loc.Longitude, 6);
                loc.Altitude = Math.Round(loc.Altitude, 2);

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

        [HttpGet]
        public JsonResult Altitude(double latitude, double longitude)
        {
            JsonResult result = Json(new { success = false, data = "An error has occurred." }, JsonRequestBehavior.AllowGet);

            try
            {
                string elevUrl = String.Format("http://maps.googleapis.com/maps/api/elevation/json?sensor=true&locations={0},{1}", latitude, longitude);
                WebRequest altitudeRequest = WebRequest.CreateDefault(new Uri(elevUrl));
                WebResponse altitudeResponse = altitudeRequest.GetResponse();

                Stream respStream = altitudeResponse.GetResponseStream();
                string respString = new StreamReader(respStream).ReadToEnd();
                JavaScriptSerializer deserial = new JavaScriptSerializer();
                AltitudeResponseModel respData = deserial.Deserialize<AltitudeResponseModel>(respString);
                result = Json(new { success = true, data = respData.results[0].elevation }, JsonRequestBehavior.AllowGet);

            }
            catch (Exception ex)
            {
                result = Json(new { success = false, data = ex.Message }, JsonRequestBehavior.AllowGet);
            }

            return result;
        }

    }
}
