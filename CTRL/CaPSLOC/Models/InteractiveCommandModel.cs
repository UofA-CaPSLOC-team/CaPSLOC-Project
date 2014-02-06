using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace CaPSLOC.Models
{
    public class InteractiveCommandModel
    {
        public int AltId { get; set; }
        public CommandUserLevel UserLevel {get; set;}
        public ScriptCommandModel Command { get; set; }
    }

    public enum CommandUserLevel
    {
        std = 1,
        admin = 2
    }
}