
using CaPSLOC.Models;
using System.Text;
using System.Xml;
using System;

namespace CaPSLOC.Services
{
    public class XmlParsing
    {
        StringBuilder Builder = new StringBuilder();
        XmlWriter Writer;

        public XmlParsing()
        {
            XmlWriterSettings settings = new XmlWriterSettings();
            settings.Encoding = Encoding.UTF8;
            Writer = XmlWriter.Create(Builder, settings);
        }

        public string SerializeInteractive(InteractiveCommandModel interactive)
        {
            Writer.WriteStartDocument();
            Writer.WriteStartElement("command");
            Writer.WriteAttributeString("user", interactive.UserLevel.ToString());
            SerializeCommand(interactive.Command);
            Writer.WriteEndElement();
            Writer.WriteEndDocument();  // Close remaining tags
            Writer.Flush();
            return Builder.ToString();
        }

        public string SerializeScript(ScriptModel script)
        {
                Writer.WriteStartDocument();
                Writer.WriteStartElement("script");
                Writer.WriteAttributeString("name", script.Name);
                foreach (ScriptCommandModel command in script.Commands)
                {
                    SerializeCommand(command);
                }
                Writer.WriteEndElement();
                Writer.WriteEndDocument();  // Close remaining tags
                Writer.Flush();
                return Builder.ToString();
        }

        private void SerializeCommand(ScriptCommandModel command)
        {
            switch (command.CommandType)
            {
                case 1:
                case 2:
                    Writer.WriteStartElement("goto");
                    break;
                case 3:
                    Writer.WriteStartElement("halt");
                    break;
                case 4:
                    Writer.WriteStartElement("pause");
                    break;
                case 5:
                    Writer.WriteStartElement("resume");
                    break;
                case 6:
                    Writer.WriteStartElement("capture");
                    break;
                case 7:
                    Writer.WriteStartElement("rmotion");
                    break;
                case 8:
                    Writer.WriteStartElement("wait");
                    break;
                case 9:
                    Writer.WriteStartElement("exec");
                    break;
                case 10:
                    Writer.WriteStartElement("config");
                    break;
                default:
                    break;
            }
            foreach (ScriptParamModel param in command.Params)
            {
                SerializeParam(param, command.CommandType);
            }
            Writer.WriteEndElement(); // Close the command tag
        }

        private void SerializeParam(ScriptParamModel param, int commandType)
        {
            switch (String.Format("{0}-{1}", commandType, param.Id.Substring(param.Id.IndexOf('-') + 1)))
            {
                case "1-1":
                case "2-1":
                    Writer.WriteElementString("latitude", param.Value);
                    break;
                case "1-2":
                case "2-2":
                    Writer.WriteElementString("longitude", param.Value);
                    break;
                case "1-3":
                case "2-3":
                    Writer.WriteElementString("altitude", param.Value);
                    break;
                case "2-4":
                    Writer.WriteElementString("name", param.Value);
                    break;
                case "6-1":
                    Writer.WriteElementString("mode", param.Value);
                    break;
                case "6-2":
                    Writer.WriteElementString("tot", String.Format("00:{0}:00", param.Value));
                    break;
                case "6-3":
                    Writer.WriteElementString("framerate", param.Value);
                    break;
                case "6-4":
                    Writer.WriteElementString("quality", param.Value);
                    break;
                case "7-1":
                    Writer.WriteElementString("direction", param.Value);
                    break;
                case "7-2":
                    Writer.WriteElementString("angle", param.Value);
                    break;
                case "8-1":
                    Writer.WriteElementString("time", String.Format("00:{0}:00", param.Value));
                    break;
                case "9-1":
                    Writer.WriteElementString("name", param.Value);
                    break;
                case "10-1":
                    Writer.WriteElementString("rmotionangle", param.Value);
                    break;
                case "10-2":
                    Writer.WriteElementString("vidtime", String.Format("00:{0}:00", param.Value));
                    break;
                case "10-3":
                    Writer.WriteElementString("framerate", param.Value);
                    break;
                case "10-4":
                    Writer.WriteElementString("quality", param.Value);
                    break;
                case "10-5":
                    Writer.WriteElementString("waittime", param.Value);
                    break;
                case "10-6":
                    Writer.WriteElementString("latoffset", param.Value);
                    break;
                case "10-7":
                    Writer.WriteElementString("longoffset", param.Value);
                    break;
                case "10-8":
                    Writer.WriteElementString("altoffset", param.Value);
                    break;
                default:
                    break;
            }
        }
    }
}