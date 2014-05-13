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
        private Thread[] threadsRun_;

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
                        if (threadsRun_[nIndex] == null || threadsRun_[nIndex].ThreadState != ThreadState.Suspended)
                        {
                            // create new thread
                            threadsRun_[nIndex] = new Thread(new ThreadStart(ThreadRunFunction));
                            // set thread name equal to its index
                            threadsRun_[nIndex].Name = nIndex.ToString();
                            // start thread working function
                            threadsRun_[nIndex].Start();
                        }
                    }
                    // change thread value
                    threadCount = value;
                }
                catch (Exception)
                {
                }
            }
        }

        // constructor
        public CrawlerEngine(CrawlerSettings cSetting, CrawlerInput cInput, CrawlerOutput cOutput)
        {
            this.crawlerSettings_ = cSetting;
            this.crawlerInput_ = cInput;
            this.crawlerOutput_ = cOutput;
            this.threadsRun_ = null;
        }

        // begin download and parse web site
        public int RunCrawling() 
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
            CrawlerWebRequest request = null;
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


        void ParseUri(CrawlerUri uri, ref CrawlerWebRequest request)
        {
            try
            {
                // create web request
                request = CrawlerWebRequest.Create(uri, request, crawlerSettings_.keepAlive_);
                // set request timeout
                request.timeOut_ = crawlerSettings_.requestTimeout_ * 1000;
                // retrieve response from web request
                CrawlerWebResponse response = request.GetResponse();

                // check for redirection
                if (response.responseUri_.Equals(uri) == false)
                {
                    // add the new uri to the queue
                    crawlerOutput_.EnqueueUri(new CrawlerUri(response.responseUri_.AbsoluteUri), true);
                    request = null;
                    return;
                }

                // check for allowed MIME types
                if (crawlerSettings_.allMIMETypes_ == false && 
                    response.contentType_ != null &&
                    crawlerSettings_.MIMETypes_.Length > 0)
                {
                    string strContentType = response.contentType_.ToLower();
                    int nExtIndex = strContentType.IndexOf(';');
                    if (nExtIndex != -1)
                        strContentType = strContentType.Substring(0, nExtIndex);
                    if (strContentType.IndexOf('*') == -1 &&
                        (nExtIndex = crawlerSettings_.MIMETypes_.IndexOf(strContentType)) == -1)
                    {
                        //LogError(uri.AbsoluteUri, strStatus + "\r\nUnlisted Content-Type (" + strContentType + "), check settings.");
                        request = null;
                        return;
                    }

                    // find numbers
                    Match match = new Regex(@"\d+").Match(crawlerSettings_.MIMETypes_, nExtIndex);
                    int nMin = int.Parse(match.Value) * 1024;
                    match = match.NextMatch();
                    int nMax = int.Parse(match.Value) * 1024;
                    if (nMin < nMax && (response.contentLength_ < nMin || response.contentLength_ > nMax))
                    {
                        //LogError(uri.AbsoluteUri, strStatus + "\r\nContentLength limit error (" + response.contentLength_ + ")");
                        request = null;
                        return;
                    }
                }

                // check for response extension
                string[] ExtArray = { ".gif", ".jpg", ".css", ".zip", ".exe" };
                bool bParse = true;
                foreach (string ext in ExtArray)
                {
                    if (uri.AbsoluteUri.ToLower().EndsWith(ext) == true)
                    {
                        bParse = false;
                        break;
                    }
                }
                foreach (string ext in crawlerSettings_.excludeFiles_)
                {
                    if (ext.Trim().Length > 0 && uri.AbsoluteUri.ToLower().EndsWith(ext) == true)
                    {
                        bParse = false;
                        break;
                    }
                }

                // construct path in the hard disk
                string strLocalPath = uri.LocalPath;
                // check if the path ends with / to can crate the file on the HD 
                if (strLocalPath.EndsWith("/") == true)
                {
                    // check if there is no query like (.asp?i=32&j=212)
                    if (uri.Query == "")
                        // add a default name for / ended paths
                        strLocalPath += "default.html";
                }
                // check if the uri includes a query string
                if (uri.Query != "")
                    // construct the name from the query hash value to be the same if we download it again
                    strLocalPath += uri.Query.GetHashCode() + ".html";
                // construct the full path folder
                string BasePath = crawlerSettings_.downloadfolder_ + "\\" + uri.Host + Path.GetDirectoryName(uri.AbsolutePath);
                // check if the folder not found
                if (Directory.Exists(BasePath) == false)
                    // create the folder
                    Directory.CreateDirectory(BasePath);
                // construct the full path name of the file
                string PathName = crawlerSettings_.downloadfolder_ + "\\" + uri.Host + strLocalPath.Replace("%20", " ");
                // open the output file
                FileStream fStream = File.Open(PathName, FileMode.Create, FileAccess.Write, FileShare.ReadWrite);
                BinaryWriter bWriter = new BinaryWriter(fStream);

                // receive response buffer
                string strResponse = "";
                byte[] RecvBuffer = new byte[4096];
                int nBytes, nTotalBytes = 0;
                // loop to receive response buffer
                while ((nBytes = response.socket_.Receive(RecvBuffer, 0, 4096, SocketFlags.None)) > 0)
                {
                    // increment total received bytes
                    nTotalBytes += nBytes;
                    // write received buffer to file
                    bWriter.Write(RecvBuffer, 0, nBytes);
                    // check if the uri type not binary to can be parsed for refs
                    if (bParse == true)
                    {
                        // add received buffer to response string
                        strResponse += Encoding.ASCII.GetString(RecvBuffer, 0, nBytes);
                    }
                  // check if connection Keep-Alive to can break the loop if response completed
                    if (response.keepAlive_ && nTotalBytes >= response.contentLength_ && response.contentLength_ > 0)
                        break;
                }
                // close output stream
                bWriter.Close();
                fStream.Close();

                if (!response.keepAlive_)
                {
                    // close response
                    response.Close();
                }
                // increment total file count
                crawlerOutput_.fileCount_++;
                // increment total bytes count
                crawlerOutput_.byteCount_ += nTotalBytes;

                if (ThreadsRunning == true 
                    && bParse == true 
                    && uri.Depth < crawlerSettings_.webDepth_)
                {
                    // check for restricted words
                    foreach (string strExcludeWord in crawlerSettings_.excludeWords_)
                    {
                        if (strExcludeWord.Trim().Length > 0 && strResponse.IndexOf(strExcludeWord) != -1)
                        {
                            File.Delete(PathName);
                            return;
                        }
                    }

                    // parse the page to search for refs
                    string strRef = @"(href|HREF|src|SRC)[ ]*=[ ]*[""'][^""'#>]+[""']";
                    MatchCollection matches = new Regex(strRef).Matches(strResponse);
                    crawlerOutput_.URLCount_ += matches.Count;
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
                            if (newUri.Host != uri.Host && crawlerSettings_.keepSameServer_ == true)
                                continue;
                            newUri.Depth = uri.Depth + 1;
                            crawlerOutput_.EnqueueUri(newUri, true);
                        }
                        catch (Exception)
                        {
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                request = null;
            }
            finally
            {
            }
        }

        // normalize the url string
        private void Normalize(ref string strURL)
        {
            if (strURL.StartsWith("http://") == false)
                strURL = "http://" + strURL;
            if (strURL.IndexOf("/", 8) == -1)
                strURL += '/';
        }
    }
}
