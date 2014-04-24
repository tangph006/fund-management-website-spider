using System;
using System.Net;

using FM.Util;

namespace FM.Service.WebSpider
{
    public class WebPageState
    {
        private WebPageState() { }

        public WebPageState(Uri uri)
        {
            uri_ = uri;
            processStarted_ = false;
            processSuccessfull_ = false;
        }

        public WebPageState(string uri)
        {
            uri_ = new Uri(uri);
            processStarted_ = false;
            processSuccessfull_ = false;
        }

        #region properties
        public Uri uri_ { get; set; }
        public string content_ { get; set; }
        public string processInstructions_ { get; set; }
        public bool processStarted_ { get; set; }
        public bool processSuccessfull_ { get; set; }
        public string statusCode_ { get; set; }
        public string statusDescription_ { get; set; }
        #endregion
    }
}
