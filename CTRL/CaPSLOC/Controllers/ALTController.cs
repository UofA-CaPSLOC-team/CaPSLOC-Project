using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Web.Http;

namespace CaPSLOC.Controllers
{
    public class ALTController : ApiController
    {
        [HttpGet] // From browser
        public void PingSweep(IPAddress start, IPAddress mask)
        {
            // Clear list of active ALTs

            foreach(/* IP in range */)
            {
                // Send 'ping' on ALT communication TCP port
            }
        }

        [HttpPost] // From ALT
        public void PingReply(int AltId)
        {
            // Add to list of active ALT units
        }

        //GET - from browser
        public XmlBlob SendCommand(int AltId)
        {
            // Lookup ALT address in list

            // Transmit XML to ALT

            // Inform UI that command was sent
        }

        //POST - from ALT
        public AltStatus ReceiveStatus(int AltId, string status)
        {
            // Inform UI of status
        }
    }
}
