using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Text.RegularExpressions;

namespace CrawlerCommon
{
    public class CrawlerWebResponse
    {
        public Uri responseUri_;
        public string contentType_;
        public int contentLength_;
        public WebHeaderCollection headers_;
        public string header_;
        public Socket socket_;
        public bool keepAlive_;
        public CrawlerWebResponse()
        {
        }
        public void Connect(CrawlerWebRequest request)
        {
            responseUri_ = request.requestUri_;

            socket_ = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            IPEndPoint remoteEP = new IPEndPoint(Dns.GetHostEntry(responseUri_.Host).AddressList[0], responseUri_.Port);
            socket_.Connect(remoteEP);
        }
        public void SendRequest(CrawlerWebRequest request)
        {
            responseUri_ = request.requestUri_;

            request.header_ = request.methodName_ + " " + responseUri_.PathAndQuery + " HTTP/1.0\r\n" + request.headers_;
            socket_.Send(Encoding.ASCII.GetBytes(request.header_));
        }
        public void SetTimeout(int Timeout)
        {
            socket_.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.SendTimeout, Timeout * 1000);
            socket_.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReceiveTimeout, Timeout * 1000);
        }
        public void ReceiveHeader()
        {
            header_ = "";
            headers_ = new WebHeaderCollection();

            byte[] bytes = new byte[10];
            while (socket_.Receive(bytes, 0, 1, SocketFlags.None) > 0)
            {
                header_ += Encoding.ASCII.GetString(bytes, 0, 1);
                if (bytes[0] == '\n' && header_.EndsWith("\r\n\r\n"))
                    break;
            }
            MatchCollection matches = new Regex("[^\r\n]+").Matches(header_.TrimEnd('\r', '\n'));
            for (int n = 1; n < matches.Count; n++)
            {
                string[] strItem = matches[n].Value.Split(new char[] { ':' }, 2);
                if (strItem.Length > 0)
                    headers_[strItem[0].Trim()] = strItem[1].Trim();
            }
            // check if the page should be transfered to another location
            if (matches.Count > 0 && (
                matches[0].Value.IndexOf(" 302 ") != -1 ||
                matches[0].Value.IndexOf(" 301 ") != -1))
                // check if the new location is sent in the "location" header
                if (headers_["Location"] != null)
                {
                    try { responseUri_ = new Uri(headers_["Location"]); }
                    catch { responseUri_ = new Uri(responseUri_, headers_["Location"]); }
                }
            contentType_ = headers_["Content-Type"];
            if (headers_["Content-Length"] != null)
                contentLength_ = int.Parse(headers_["Content-Length"]);
            keepAlive_ = (headers_["Connection"] != null && headers_["Connection"].ToLower() == "keep-alive") ||
                        (headers_["Proxy-Connection"] != null && headers_["Proxy-Connection"].ToLower() == "keep-alive");
        }
        public void Close()
        {
            socket_.Close();
        }
    }
}
