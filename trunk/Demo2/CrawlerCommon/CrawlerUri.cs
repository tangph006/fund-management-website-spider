using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CrawlerCommon
{
    public class CrawlerUri : System.Uri , IComparable
    {
        // parsing depth limit
        private int depth;
        public int depth_
        {
            get { return depth; }
            set { depth = value; }
        }

        // the state of this Uri
        private CrawlerUriParseState state;
        public CrawlerUriParseState state_
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

        // normalize the url string
        public static void Normalize(ref string strURL)
        {
            if (strURL.StartsWith("http://") == false)
                strURL = "http://" + strURL;
            if (strURL.IndexOf("/", 8) == -1)
                strURL += '/';
        }
        public CrawlerUri(string uriString) : base(uriString) 
        {
            state_ = CrawlerUriParseState.uri_state_not_handled;
            depth_ = 0; // default crawling depth is 1.
        }

        public int CompareTo(object obj)
        {
            CrawlerUri other = obj as CrawlerUri;
            return this.AbsoluteUri.CompareTo(other.AbsoluteUri);
        }
    }

    public enum CrawlerUriParseState
    {
        uri_state_not_handled,
        uri_state_handled_succeed,
        uri_state_handled_failed,
        uri_state_not_allowed_MIME_type,
        uri_state_other
    }
}
