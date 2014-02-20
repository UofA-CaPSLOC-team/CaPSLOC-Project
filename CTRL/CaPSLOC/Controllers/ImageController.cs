using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Web.Http;
using System.Web.Mvc;
using System.Web;
using System.IO;
using CaPSLOC.Models;

namespace CaPSLOC.Controllers
{
    public class ImageController : BaseController
    {

        public JsonResult Save(HttpPostedFileBase file, string altName, double latitude, double longitude, double altitude, string locName, DateTime captureTime)
        {
            JsonResult result = Json(new { success = false, data = "An unknown error has occurred" }, JsonRequestBehavior.DenyGet);

            try
            {
                // Read images folder from settings
                string imagePath = DbModel.AppSettings.Single(a => a.ShortName == Constants.IMAGE_STORAGE_PATH).Value;

                // Generate filename (based on time, or some other method to prevent name collisions)
                string filename = Path.GetRandomFileName();
                switch (file.ContentType)
                {
                    case "image/jpeg":
                        filename = Path.ChangeExtension(filename, "jpg");
                        break;
                    case "image/png":
                        filename = Path.ChangeExtension(filename, "png");
                        break;
                    default:
                        throw new Exception("Invalid MIME type: " + file.ContentType);
                }
                string filePath = Path.Combine(imagePath, filename);

                // Save image to folder
                using(Stream fileStream = System.IO.File.Create(filePath))
                {
                    file.InputStream.CopyTo(fileStream);
                }

                // Write location info, time, and filename to database
                ALT alt = DbModel.ALTs.SingleOrDefault(a => a.Name == altName);
                if (alt == null)
                {
                    alt = new ALT()
                    {
                        Name = altName,
                        Address = Request.UserHostAddress,
                        RecentlyLocated = true
                    };
                }

                Location loc = DbModel.Locations.SingleOrDefault(l => l.Latitude == latitude && l.Longitude == longitude && l.Altitude == altitude);
                if (loc == null)
                {
                    loc = new Location()
                    {
                        Latitude = latitude,
                        Longitude = longitude,
                        Altitude = altitude,
                        Name = locName ?? String.Format("AutoGen-{0}", Path.GetRandomFileName().Substring(0, 8))
                    };
                }

                Data data = new Data()
                {
                    ImageFilename = filename,
                    ALT = alt,
                    Location = loc,
                    ImageEncoding = Path.GetExtension(filename),
                    CaptureTime = captureTime
                };

                DbModel.Data.Add(data);
                DbModel.SaveChanges();

                result = Json(new { success = true, data = "Save successful" }, JsonRequestBehavior.DenyGet);

                return result;
            }
            catch (Exception ex)
            {
                return Json(new { success = false, data = "An error has occurred: " + ex.Message }, JsonRequestBehavior.DenyGet);
            }
        }


    }
}
