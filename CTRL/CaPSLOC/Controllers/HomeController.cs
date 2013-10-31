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
            using (DbModelContainer model = new DbModelContainer())
            {
                Location loc = new Location()
                {
                    Latitude = 41.4256,
                    Longitude = -80.2356,
                    Altitude = 307.127,
                    Name = "Akron Guess"
                };
                model.Locations.AddObject(loc);
                model.SaveChanges();
            }

            return View();
        }

    }
}
