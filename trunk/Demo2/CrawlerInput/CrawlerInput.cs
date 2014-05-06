using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Engine
{
    public class CrawlerInput
    {
        // the url going to parse
        private string strFullUrl;
        public string FullUrl
        {
            get { return strFullUrl; }
            set { strFullUrl = value; }
        }

        // domain name
        private string strDomain;
        public string Domain
        {
            get { return strDomain; }
            set { strDomain = value; }
        } 
    }
}
