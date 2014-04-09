using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace CaPSLOC.Models
{
    public class ImageGridFilter
    {
        public int AltId { get; set; }
        public int LocationId { get; set; }
        public DateTime StartDate { get; set; }
        public TimeSpan StartTime { get; set; }
        public DateTime EndDate { get; set; }
        public TimeSpan EndTime { get; set; }
    }
}