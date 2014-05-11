using System;
using System.Xml;
using System.IO;
using System.Text;
using System.Text.RegularExpressions;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Resources;
using System.Threading;
using System.Runtime.InteropServices;
using System.Net;
using System.Net.Sockets;

using Input;
using Settings;
using Output;
using CrawlerCommon;
namespace Engine
{
    public class CrawlerEngine
    {
        public CrawlerSettings crawlerSettings_ { get; set; }
        public CrawlerInput crawlerInput_ { get; set; }
        public CrawlerOutput crawlerOutput_ { get; set; }

        // flag to be used to stop all running threads when user request to stop
        public bool ThreadsRunning { get; set; }

                // threads array
        private Thread[] threadsRun;

        // number of running threads
        private int threadCount;
        public int threadCount_
        {
            get { return threadCount; }
            set
            {
                try
                {
                    for (int nIndex = 0; nIndex < value; nIndex++)
                    {
                        // check if thread not created or not suspended
                        if (threadsRun[nIndex] == null || threadsRun[nIndex].ThreadState != ThreadState.Suspended)
                        {
                            // create new thread
                            threadsRun[nIndex] = new Thread(new ThreadStart(ThreadRunFunction));
                            // set thread name equal to its index
                            threadsRun[nIndex].Name = nIndex.ToString();
                            // start thread working function
                            threadsRun[nIndex].Start();
                            // check if thread doesn't added to the view
                            if (nIndex == this.listViewThreads.Items.Count)
                            {
                                // add a new line in the view for the new thread
                                ListViewItem item = this.listViewThreads.Items.Add((nIndex + 1).ToString(), 0);
                                string[] subItems = { "", "", "", "0", "0%" };
                                item.SubItems.AddRange(subItems);
                            }
                        }
                        // check if the thread is suspended
                        else if (threadsRun[nIndex].ThreadState == ThreadState.Suspended)
                        {
                            // get thread item from the list
                            ListViewItem item = this.listViewThreads.Items[nIndex];
                            item.ImageIndex = 1;
                            item.SubItems[2].Text = "Resume";
                            // resume the thread
                            threadsRun[nIndex].Resume();
                        }
                    }
                    // change thread value
                    threadCount = value;
                }
                catch (Exception)
                {
                }
                Monitor.Exit(this.listViewThreads);
            }
        }

        // constructor
        public CrawlerEngine(CrawlerSettings cSetting, CrawlerInput cInput, CrawlerOutput cOutput)
        {
            this.crawlerSettings_ = cSetting;
            this.crawlerInput_ = cInput;
            this.crawlerOutput_ = cOutput;
        }

        // begin download and parse web site
        public int RunCrawl() 
        {
            return 0;
        }

        // pause the crawling
        public int PauseCrawling()
        { 
            return 0; 
        }

        // stop the crawling
        public int StopCrawling() 
        {
            return 0; 
        }

        // resume the crawling
        public int ResumeCrawling() 
        { 
            return 0; 
        }

        void ThreadRunFunction()
        {
            MyWebRequest request = null;
            while (ThreadsRunning && int.Parse(Thread.CurrentThread.Name) < this.threadCount)
            {
                CrawlerUri uri = crawlerOutput_.DequeueUri();
                if (uri != null)
                {
                    if (crawlerSettings_.sleepConnectTime_ > 0)
                        Thread.Sleep(crawlerSettings_.sleepConnectTime_ * 1000);
                    ParseUri(uri, ref request);
                }
                else
                {
                    Thread.Sleep(crawlerSettings_.sleepFetchTime_ * 1000);
                }
            }
        }


        void ParseUri(CrawlerUri uri, ref MyWebRequest request)
        {
            string strStatus = "";
            // check if connection is kept alive from previous connections or not
            if (request != null && request.response.KeepAlive)
                strStatus += "Connection live to: " + uri.Host + "\r\n\r\n";
            else
                strStatus += "Connecting: " + uri.Host + "\r\n\r\n";

            try
            {
                // create web request
                request = MyWebRequest.Create(uri, request, KeepAlive);
                // set request timeout
                request.Timeout = RequestTimeout * 1000;
                // retrieve response from web request
                MyWebResponse response = request.GetResponse();
                // update status text with the request and response headers
                strStatus += request.Header + response.Header;

                // check for redirection
                if (response.ResponseUri.Equals(uri) == false)
                {
                    // add the new uri to the queue
                    this.EnqueueUri(new CrawlerUri(response.ResponseUri.AbsoluteUri), true);
                    // update status
                    strStatus += "Redirected to: " + response.ResponseUri + "\r\n";
                    // log current uri status
                    LogUri(uri.AbsoluteUri, strStatus);
                    // reset current request to avoid response socket opening case
                    request = null;
                    return;
                }

                // check for allowed MIME types
                if (AllMIMETypes == false && response.ContentType != null && MIMETypes.Length > 0)
                {
                    string strContentType = response.ContentType.ToLower();
                    int nExtIndex = strContentType.IndexOf(';');
                    if (nExtIndex != -1)
                        strContentType = strContentType.Substring(0, nExtIndex);
                    if (strContentType.IndexOf('*') == -1 && (nExtIndex = MIMETypes.IndexOf(strContentType)) == -1)
                    {
                        LogError(uri.AbsoluteUri, strStatus + "\r\nUnlisted Content-Type (" + strContentType + "), check settings.");
                        request = null;
                        return;
                    }
                    // find numbers
                    Match match = new Regex(@"\d+").Match(MIMETypes, nExtIndex);
                    int nMin = int.Parse(match.Value) * 1024;
                    match = match.NextMatch();
                    int nMax = int.Parse(match.Value) * 1024;
                    if (nMin < nMax && (response.ContentLength < nMin || response.ContentLength > nMax))
                    {
                        LogError(uri.AbsoluteUri, strStatus + "\r\nContentLength limit error (" + response.ContentLength + ")");
                        request = null;
                        return;
                    }
                }

                // check for response extention
                string[] ExtArray = { ".gif", ".jpg", ".css", ".zip", ".exe" };
                bool bParse = true;
                foreach (string ext in ExtArray)
                    if (uri.AbsoluteUri.ToLower().EndsWith(ext) == true)
                    {
                        bParse = false;
                        break;
                    }
                foreach (string ext in ExcludeFiles)
                    if (ext.Trim().Length > 0 && uri.AbsoluteUri.ToLower().EndsWith(ext) == true)
                    {
                        bParse = false;
                        break;
                    }

                // construct path in the hard disk
                string strLocalPath = uri.LocalPath;
                // check if the path ends with / to can crate the file on the HD 
                if (strLocalPath.EndsWith("/") == true)
                    // check if there is no query like (.asp?i=32&j=212)
                    if (uri.Query == "")
                        // add a default name for / ended pathes
                        strLocalPath += "default.html";
                // check if the uri includes a query string
                if (uri.Query != "")
                    // construct the name from the query hash value to be the same if we download it again
                    strLocalPath += uri.Query.GetHashCode() + ".html";
                // construct the full path folder
                string BasePath = this.Downloadfolder + "\\" + uri.Host + Path.GetDirectoryName(uri.AbsolutePath);
                // check if the folder not found
                if (Directory.Exists(BasePath) == false)
                    // create the folder
                    Directory.CreateDirectory(BasePath);
                // construct the full path name of the file
                string PathName = this.Downloadfolder + "\\" + uri.Host + strLocalPath.Replace("%20", " ");
                // open the output file
                FileStream streamOut = File.Open(PathName, FileMode.Create, FileAccess.Write, FileShare.ReadWrite);
                BinaryWriter writer = new BinaryWriter(streamOut);

                itemLog.SubItems[2].Text = "Download";
                itemLog.ForeColor = Color.Black;
                // receive response buffer
                string strResponse = "";
                byte[] RecvBuffer = new byte[10240];
                int nBytes, nTotalBytes = 0;
                // loop to receive response buffer
                while ((nBytes = response.socket.Receive(RecvBuffer, 0, 10240, SocketFlags.None)) > 0)
                {
                    // increment total received bytes
                    nTotalBytes += nBytes;
                    // write received buffer to file
                    writer.Write(RecvBuffer, 0, nBytes);
                    // check if the uri type not binary to can be parsed for refs
                    if (bParse == true)
                        // add received buffer to response string
                        strResponse += Encoding.ASCII.GetString(RecvBuffer, 0, nBytes);
                    // update view text
                    itemLog.SubItems[4].Text = Commas(nTotalBytes);
                    if (response.ContentLength > 0)
                        itemLog.SubItems[5].Text = '%' + (100 - (response.ContentLength - nTotalBytes) * 100 / response.ContentLength).ToString();
                    // check if connection Keep-Alive to can break the loop if response completed
                    if (response.KeepAlive && nTotalBytes >= response.ContentLength && response.ContentLength > 0)
                        break;
                }
                // close output stream
                writer.Close();
                streamOut.Close();

                if (response.KeepAlive)
                    strStatus += "Connection kept alive to be used in subpages.\r\n";
                else
                {
                    // close response
                    response.Close();
                    strStatus += "Connection closed.\r\n";
                }
                // update status
                strStatus += Commas(nTotalBytes) + " bytes, downloaded to \"" + PathName + "\"\r\n";
                // increment total file count
                FileCount++;
                // increment total bytes count
                ByteCount += nTotalBytes;

                if (ThreadsRunning == true && bParse == true && uri.Depth < WebDepth)
                {
                    strStatus += "\r\nParsing page ...\r\n";

                    // check for restricted words
                    foreach (string strExcludeWord in ExcludeWords)
                        if (strExcludeWord.Trim().Length > 0 && strResponse.IndexOf(strExcludeWord) != -1)
                        {
                            LogError(uri.AbsoluteUri, strStatus + "\r\nPage includes reserved word (" + strExcludeWord + ")");
                            EraseItem(itemLog);
                            File.Delete(PathName);
                            return;
                        }

                    // parse the page to search for refs
                    string strRef = @"(href|HREF|src|SRC)[ ]*=[ ]*[""'][^""'#>]+[""']";
                    MatchCollection matches = new Regex(strRef).Matches(strResponse);
                    strStatus += "Found: " + matches.Count + " ref(s)\r\n";
                    URLCount += matches.Count;
                    foreach (Match match in matches)
                    {
                        strRef = match.Value.Substring(match.Value.IndexOf('=') + 1).Trim('"', '\'', '#', ' ', '>');
                        try
                        {
                            if (strRef.IndexOf("..") != -1 || strRef.StartsWith("/") == true || strRef.StartsWith("http://") == false)
                                strRef = new Uri(uri, strRef).AbsoluteUri;
                            Normalize(ref strRef);
                            CrawlerUri newUri = new CrawlerUri(strRef);
                            if (newUri.Scheme != Uri.UriSchemeHttp && newUri.Scheme != Uri.UriSchemeHttps)
                                continue;
                            if (newUri.Host != uri.Host && KeepSameServer == true)
                                continue;
                            newUri.Depth = uri.Depth + 1;
                            if (this.EnqueueUri(newUri, true) == true)
                                strStatus += newUri.AbsoluteUri + "\r\n";
                        }
                        catch (Exception)
                        {
                        }
                    }
                }
                LogUri(uri.AbsoluteUri, strStatus);
            }
            catch (Exception e)
            {
                LogError(uri.AbsoluteUri, strStatus + e.Message);
                request = null;
            }
            finally
            {
                EraseItem(itemLog);
            }
        }
    }
}
