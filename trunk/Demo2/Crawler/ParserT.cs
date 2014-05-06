using System;
using System.Drawing;
using System.Xml;
using System.IO;
using System.Text;
using System.Text.RegularExpressions;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.Resources;
using System.Threading;
using System.Runtime.InteropServices;
using System.Net;
using System.Net.Sockets;
using LiteLib;

namespace Crawler
{
    public class CrawlerEngine
    {
        public SettingT settingT { get; set; }
        public InputT inputT { get; set; }
        public OutputT outputT { get; set; }

        // flag to be used to stop all running threads when user request to stop
        public bool ThreadsRunning { get; set; }

        public CrawlerEngine(SettingT sT, InputT iT, OutputT oT)
        {
            this.settingT = sT;
            this.inputT = iT;
            this.outputT = oT;
        }

        // begin download and parse web site
        public int RunParse() 
        { 
            return 0; 
        }

        // pause the parsing
        public int PauseParse()
        { 
            return 0; 
        }

        // stop the parsing
        public int StopParse() 
        {
            return 0; 
        }

        // resume the parsing
        public int ResumeParse() 
        { 
            return 0; 
        }

        // push uri to the queue
        bool EnqueueUri(MyUri uri, bool bCheckRepetition)
        {
            // add the uri to the binary tree to check if it is duplicated or not
            if (bCheckRepetition == true && AddURL(ref uri) == false)
                return false;

            Monitor.Enter(queueURLS);
            try
            {
                // add the uri to the queue
                queueURLS.Enqueue(uri);
            }
            catch (Exception)
            {
            }
            Monitor.Exit(queueURLS);

            return true;
        }

        // pop uri from the queue
        MyUri DequeueUri()
        {
            Monitor.Enter(queueURLS);
            MyUri uri = null;
            try
            {
                uri = (MyUri)queueURLS.Dequeue();
            }
            catch (Exception)
            {
            }
            Monitor.Exit(queueURLS);
            return uri;
        }

        // add URL 
        bool AddURL(ref MyUri uri)
        {
            foreach (string str in settingT.ExcludeHosts)
            if (str.Trim().Length > 0 && uri.Host.ToLower().IndexOf(str.Trim()) != -1)
            {
                LogError(uri.AbsoluteUri, "\r\nHost excluded as it includes reserved pattern (" + str + ")");
                return false;
            }
            Monitor.Enter(urlStorage);
            bool bNew = false;
            try
            {
                string strURL = uri.AbsoluteUri;
                bNew = urlStorage.Add(ref strURL).Count == 1;
            }
            catch (Exception)
            {
            }
            Monitor.Exit(urlStorage);

            return bNew;
        }
    }

    public class MyUri : System.Uri
    {
        // parsing depth limit
        private int nDepth;
        public int Depth 
        {
            get { return nDepth; }
            set { nDepth = value; }
        }

        // 0: not handled; 1: handle_successed; 2: handle_failed; 3: other
        private int nState;
        public int State
        {
            get { return nState; }
            set { nState = value; }
        }

        // domain name
        private string strDomain;
        public string Domain
        {
            get { return strDomain; }
            set { strDomain = value; }
        }

        public MyUri(string uriString) : base(uriString){ }
    }

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

    public class MyWebResponse
    {
        public Uri ResponseUri;
        public string ContentType;
        public int ContentLength;
        public WebHeaderCollection Headers;
        public string Header;
        public Socket socket;
        public bool KeepAlive;
        public MyWebResponse()
        {
        }
        public void Connect(MyWebRequest request)
        {
            ResponseUri = request.RequestUri;

            socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            IPEndPoint remoteEP = new IPEndPoint(Dns.GetHostEntry(ResponseUri.Host).AddressList[0], ResponseUri.Port);
            socket.Connect(remoteEP);
        }
        public void SendRequest(MyWebRequest request)
        {
            ResponseUri = request.RequestUri;

            request.Header = request.Method + " " + ResponseUri.PathAndQuery + " HTTP/1.0\r\n" + request.Headers;
            socket.Send(Encoding.ASCII.GetBytes(request.Header));
        }
        public void SetTimeout(int Timeout)
        {
            socket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.SendTimeout, Timeout * 1000);
            socket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReceiveTimeout, Timeout * 1000);
        }
        public void ReceiveHeader()
        {
            Header = "";
            Headers = new WebHeaderCollection();

            byte[] bytes = new byte[10];
            while (socket.Receive(bytes, 0, 1, SocketFlags.None) > 0)
            {
                Header += Encoding.ASCII.GetString(bytes, 0, 1);
                if (bytes[0] == '\n' && Header.EndsWith("\r\n\r\n"))
                    break;
            }
            MatchCollection matches = new Regex("[^\r\n]+").Matches(Header.TrimEnd('\r', '\n'));
            for (int n = 1; n < matches.Count; n++)
            {
                string[] strItem = matches[n].Value.Split(new char[] { ':' }, 2);
                if (strItem.Length > 0)
                    Headers[strItem[0].Trim()] = strItem[1].Trim();
            }
            // check if the page should be transfered to another location
            if (matches.Count > 0 && (
                matches[0].Value.IndexOf(" 302 ") != -1 ||
                matches[0].Value.IndexOf(" 301 ") != -1))
                // check if the new location is sent in the "location" header
                if (Headers["Location"] != null)
                {
                    try { ResponseUri = new Uri(Headers["Location"]); }
                    catch { ResponseUri = new Uri(ResponseUri, Headers["Location"]); }
                }
            ContentType = Headers["Content-Type"];
            if (Headers["Content-Length"] != null)
                ContentLength = int.Parse(Headers["Content-Length"]);
            KeepAlive = (Headers["Connection"] != null && Headers["Connection"].ToLower() == "keep-alive") ||
                        (Headers["Proxy-Connection"] != null && Headers["Proxy-Connection"].ToLower() == "keep-alive");
        }
        public void Close()
        {
            socket.Close();
        }
    }
}
