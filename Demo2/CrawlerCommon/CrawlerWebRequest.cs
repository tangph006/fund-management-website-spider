using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;

namespace CrawlerCommon
{
    public class CrawlerWebRequest
    {
        public int timeOut_;
        public WebHeaderCollection headers_;
        public string header_;
        public Uri requestUri_;
        public string methodName_;
        public CrawlerWebResponse response_;
        public bool keepAlive_;
        public CrawlerWebRequest(Uri uri, bool bKeepAlive)
        {
            headers_ = new WebHeaderCollection();
            requestUri_ = uri;
            headers_["Host"] = uri.Host;
            keepAlive_ = bKeepAlive;
            if (keepAlive_)
                headers_["Connection"] = "Keep-Alive";
            methodName_ = "GET";
        }
        public static CrawlerWebRequest Create(Uri uri, CrawlerWebRequest AliveRequest, bool bKeepAlive)
        {
            if (bKeepAlive &&
                AliveRequest != null &&
                AliveRequest.response_ != null &&
                AliveRequest.response_.keepAlive_ &&
                AliveRequest.response_.socket_.Connected &&
                AliveRequest.requestUri_.Host == uri.Host)
            {
                AliveRequest.requestUri_ = uri;
                return AliveRequest;
            }
            return new CrawlerWebRequest(uri, bKeepAlive);
        }
        public CrawlerWebResponse GetResponse()
        {
            if (response_ == null || response_.socket_ == null || response_.socket_.Connected == false)
            {
                response_ = new CrawlerWebResponse();
                response_.Connect(this);
                response_.SetTimeout(timeOut_);
            }
            response_.SendRequest(this);
            response_.ReceiveHeader();
            return response_;
        }
    }

}
