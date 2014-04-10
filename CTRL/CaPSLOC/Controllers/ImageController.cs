using System;
using System.Collections.Generic;
using System.Data.Entity;
using System.Linq;
using System.Web.Mvc;
using System.Web;
using System.IO;
using CaPSLOC.Models;
using System.Drawing;

namespace CaPSLOC.Controllers
{
    public class ImageController : BaseController
    {

        // From browser
        [HttpPost]
        public ActionResult RefreshGrid(ImageGridFilter filter)
        {
            DateTime start = DateTime.MinValue;
            DateTime end = DateTime.MinValue;
            if (filter.StartDate != DateTime.MinValue)
            {
                Logger.Debug("Composing start datetime");
                start = filter.StartDate.Add(filter.StartTime);
            }
            if (filter.EndDate != DateTime.MinValue)
            {
                Logger.Debug("Composing end datetime");
                end = filter.EndDate.Add(filter.EndTime);
            }

            IQueryable<Data> filteredData = DbModel.Data.Include(d => d.ALT).Include(d => d.Location);  // Start with all data
            if (filter.AltId != 0)
            {
                Logger.Debug("Filter by ALT id = " + filter.AltId);
                filteredData = filteredData.Where(d => d.ALTId == filter.AltId);
            }
            if (filter.LocationId != 0)
            {
                Logger.Debug("Filter by Location id = " + filter.LocationId);
                filteredData = filteredData.Where(d => d.LocationId == filter.LocationId);
            }
            if (start != DateTime.MinValue)
            {
                Logger.Debug("Filter by capture date after " + start);
                filteredData = filteredData.Where(d => d.CaptureTime >= start);
            }
            if (end != DateTime.MinValue)
            {
                Logger.Debug("Filter by capture date before " + end);
                filteredData = filteredData.Where(d => d.CaptureTime <= end);
            }

            return PartialView(filteredData.ToList());
        }

        // From ALT
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

                //Save a thumbnail version
                file.InputStream.Position = 0;
                Bitmap bmp = new Bitmap(file.InputStream);
                //Determine scaling factor to fit in a box
                int tSize = Constants.IMAGE_THUMBNAIL_SIZE;
                
                if (Math.Max(bmp.Width, bmp.Height) > tSize)  // Check if resize is even needed
                {
                    double scale = (double)tSize / (double)Math.Max(bmp.Width, bmp.Height);
                    bmp = new Bitmap(bmp, (int)(bmp.Width * scale), (int)(bmp.Height * scale));
                }
                string tPath = Path.Combine(imagePath, "Thumbnails", filename);
                bmp.Save(tPath);
                bmp.Dispose();

                // Write location info, time, and filename to database
                ALT alt = DbModel.ALTs.SingleOrDefault(a => a.Name == altName);
                if (alt == null)
                {
                    alt = new ALT()
                    {
                        Name = altName,
                        Address = IPNetworking.GetIP4Address(Request),
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
                    CaptureTime = DateTime.Now
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
