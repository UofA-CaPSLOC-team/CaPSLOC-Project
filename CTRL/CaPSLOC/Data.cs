//------------------------------------------------------------------------------
// <auto-generated>
//    This code was generated from a template.
//
//    Manual changes to this file may cause unexpected behavior in your application.
//    Manual changes to this file will be overwritten if the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

using System;
using System.Collections.Generic;

namespace CaPSLOC
{
    public partial class Data
    {
        public int Id { get; set; }
        public string ImageFilename { get; set; }
        public string ImageEncoding { get; set; }
        public System.DateTime CaptureTime { get; set; }
        public int LocationId { get; set; }
        public int ALTId { get; set; }
    
        public virtual Location Location { get; set; }
        public virtual ALT ALT { get; set; }
    }
    
}
