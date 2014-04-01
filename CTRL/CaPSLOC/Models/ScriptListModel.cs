using System.Collections.Generic;
using System;
namespace CaPSLOC.Models
{
    public class ScriptListModel
    {
        public List<ScriptListEntryModel> Scripts { get; set; }
    }

    public class ScriptListEntryModel
    {
        public string Name { get; set; }
        public DateTime Modified { get; set; }
    }
}