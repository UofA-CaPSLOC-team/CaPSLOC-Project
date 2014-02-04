using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using CaPSLOC.Models;
using System.Xml.Serialization;
using System.IO;
using System.Xml;
using System.Text;

namespace CaPSLOC.Controllers
{
    public class ScriptController : Controller
    {
        //
        // GET: /Script/

        public ActionResult Index()
        {
            return View();
        }


        [HttpPost]
        public ActionResult Save(ScriptModel script)
        {
            using (DbModelContainer dbModel = new DbModelContainer())
            {
                // Ensure that this name is unique, if not, return error
                if (dbModel.Scripts.FirstOrDefault(s => s.Name == script.Name) != null)
                {
                    return Json(new { success = false, data = "Name is not unique!" }, JsonRequestBehavior.DenyGet);
                }

                // Write script, name, description, and whether to share to the database
                try
                {
                    string xml = SerializeScript(script);

                    Script added = dbModel.Scripts.Add(new Script()
                    {
                        Name = script.Name,
                        Description = script.Description,
                        Content = xml
                    });
                    dbModel.SaveChanges();
                    // Return success
                    return Json(new { success = true, data = added.Id }, JsonRequestBehavior.DenyGet);
                }
                catch (Exception ex)
                {
                    return Json(new { success = false, data = ex.Message }, JsonRequestBehavior.DenyGet);
                }

            }
        }

        private string SerializeScript(ScriptModel script)
        {
            StringBuilder builder = new StringBuilder();
            using (XmlWriter writer = XmlWriter.Create(builder))
            {
                writer.WriteStartDocument();
                writer.WriteStartElement("script");
                writer.WriteAttributeString("name", script.Name);
                foreach (ScriptCommandModel command in script.Commands)
                {
                    switch (command.CommandType)
                    {
                        case 1:
                        case 2:
                            writer.WriteStartElement("goto");
                            break;
                        case 3:
                            writer.WriteStartElement("halt");
                            break;
                        case 4:
                            writer.WriteStartElement("pause");
                            break;
                        case 5:
                            writer.WriteStartElement("resume");
                            break;
                        case 6:
                            writer.WriteStartElement("capture");
                            break;
                        case 7:
                            writer.WriteStartElement("rmotion");
                            break;
                        case 8:
                            writer.WriteStartElement("wait");
                            break;
                        case 9:
                            writer.WriteStartElement("exec");
                            break;
                        case 10:
                            writer.WriteStartElement("config");
                            break;
                        default:
                            break;
                    }
                    foreach (ScriptParamModel param in command.Params)
                    {
                        switch (String.Format("{0}-{1}", command.CommandType, param.Id.Substring(param.Id.IndexOf('-') + 1)))
                        {
                            case "1-1":
                            case "2-1":
                                writer.WriteElementString("latitude", param.Value);
                                break;
                            case "1-2":
                            case "2-2":
                                writer.WriteElementString("longitude", param.Value);
                                break;
                            case "1-3":
                            case "2-3":
                                writer.WriteElementString("altitude", param.Value);
                                break;
                            case "2-4":
                                writer.WriteElementString("name", param.Value);
                                break;
                            case "6-1":
                                writer.WriteElementString("mode", param.Value);
                                break;
                            case "6-2":
                                writer.WriteElementString("tot", String.Format("00:{0}:00", param.Value));
                                break;
                            case "6-3":
                                writer.WriteElementString("framerate", param.Value);
                                break;
                            case "6-4":
                                writer.WriteElementString("quality", param.Value);
                                break;
                            case "7-1":
                                writer.WriteElementString("direction", param.Value);
                                break;
                            case "7-2":
                                writer.WriteElementString("angle", param.Value);
                                break;
                            case "8-1":
                                writer.WriteElementString("time", String.Format("00:{0}:00", param.Value));
                                break;
                            case "9-1":
                                writer.WriteElementString("name", param.Value);
                                break;
                            case "10-1":
                                writer.WriteElementString("rmotionangle", param.Value);
                                break;
                            case "10-2":
                                writer.WriteElementString("vidtime", String.Format("00:{0}:00", param.Value));
                                break;
                            case "10-3":
                                writer.WriteElementString("framerate", param.Value);
                                break;
                            case "10-4":
                                writer.WriteElementString("quality", param.Value);
                                break;
                            case "10-5":
                                writer.WriteElementString("waittime", param.Value);
                                break;
                            case "10-6":
                                writer.WriteElementString("latoffset", param.Value);
                                break;
                            case "10-7":
                                writer.WriteElementString("longoffset", param.Value);
                                break;
                            case "10-8":
                                writer.WriteElementString("altoffset", param.Value);
                                break;
                            default:
                                break;
                        }
                    }
                    writer.WriteEndElement(); // Close the command tag
                }
                writer.WriteEndElement();
                writer.WriteEndDocument();  // Close remaining tags
                writer.Flush();
                return builder.ToString();
            }

        }

    }
}
