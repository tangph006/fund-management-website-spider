using System;
using System.Text;

namespace Input
{
    public class CrawlerInput
    {
        // the url going to parse
        private string fullUrl;
        public string fullUrl_
        {
            get { return fullUrl; }
            set { fullUrl = value; }
        }

        // domain name
        private string domain;
        public string domain_
        {
            get { return domain; }
            set { domain = value; }
        }
        
        // default constructor
        public CrawlerInput()
        {
        }

        // copy from
        public CrawlerInput CopyFrom(CrawlerInput other)
        {
            domain_ = other.domain_;
            fullUrl_ = other.fullUrl_;
            return this;
        }
    }
}
