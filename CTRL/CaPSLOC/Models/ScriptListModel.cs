using System.Collections.Generic;
using System;
namespace CaPSLOC.Models
{
    public class ScriptListModel
    {
        public Boolean success { get; set; }
        public List<ScriptListEntryModel> data { get; set; }
    }

    public class ScriptListEntryModel
    {
        public string Name { get; set; }
        public DateTime Modified { get; set; }
    }
}