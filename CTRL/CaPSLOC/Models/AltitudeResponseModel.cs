using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace CaPSLOC.Models
{
    public class AltitudeResponseModel
    {
        public string status { get; set; }
        public List<AltitudeLocationModel> results { get; set; }
    }

    public class AltitudeLocationModel
    {
        public double elevation { get; set; }
        public double resolution { get; set; }
        public LatLngModel location { get; set; }
    }

    public class LatLngModel
    {
        public double lat { get; set; }
        public double lng { get; set; }
    }
}