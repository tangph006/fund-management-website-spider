using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CrawlerCommon
{
    public class CrawlerUri : System.Uri
    {
        // parsing depth limit
        private int depth;
        public int Depth
        {
            get { return depth; }
            set { depth = value; }
        }

        // 0: not handled; 1: handle_successed; 2: handle_failed; 3: other
        private int state;
        public int state_
        {
            get { return state; }
            set { state = value; }
        }

        // domain name
        private string domain;
        public string domain_
        {
            get { return domain; }
            set { domain = value; }
        }

        public CrawlerUri(string uriString) : base(uriString) { }
    }
}
