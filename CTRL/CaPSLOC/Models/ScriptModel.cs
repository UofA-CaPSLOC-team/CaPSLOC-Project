
using System.Collections.Generic;
namespace CaPSLOC.Models
{
    public class ScriptModel
    {
        public string Name { get; set; }
        public string Description { get; set; }
        public List<ScriptCommandModel> Commands { get; set; }
    }
    public class ScriptCommandModel
    {
        public string CommandId { get; set; }
        public string CommandName { get; set; }
        public int CommandType { get; set; }
        public List<ScriptParamModel> Params { get; set; }
    }

    public class ScriptParamModel
    {
        public string Id { get; set; }
        public string Name { get; set; }
        public string Value { get; set; }
    }
}