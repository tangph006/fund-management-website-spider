using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Engine
{
    public class MyWebRequest
    {
        public int Timeout;
        public WebHeaderCollection Headers;
        public string Header;
        public Uri RequestUri;
        public string Method;
        public MyWebResponse response;
        public bool KeepAlive;
        public MyWebRequest(Uri uri, bool bKeepAlive)
        {
            Headers = new WebHeaderCollection();
            RequestUri = uri;
            Headers["Host"] = uri.Host;
            KeepAlive = bKeepAlive;
            if (KeepAlive)
                Headers["Connection"] = "Keep-Alive";
            Method = "GET";
        }
        public static MyWebRequest Create(Uri uri, MyWebRequest AliveRequest, bool bKeepAlive)
        {
            if (bKeepAlive &&
                AliveRequest != null &&
                AliveRequest.response != null &&
                AliveRequest.response.KeepAlive &&
                AliveRequest.response.socket.Connected &&
                AliveRequest.RequestUri.Host == uri.Host)
            {
                AliveRequest.RequestUri = uri;
                return AliveRequest;
            }
            return new MyWebRequest(uri, bKeepAlive);
        }
        public MyWebResponse GetResponse()
        {
            if (response == null || response.socket == null || response.socket.Connected == false)
            {
                response = new MyWebResponse();
                response.Connect(this);
                response.SetTimeout(Timeout);
            }
            response.SendRequest(this);
            response.ReceiveHeader();
            return response;
        }
    }

}
