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

        // unique Uri's queue
        private Queue queueUrls_;

        // threads array
        private Thread[] threadsRunning_;

        // number of running threads
        private int threadsCount;
        public int threadsCount_
        {
            get { return threadsCount; }
            set
            {
                if (value > crawlerSettings_.maxThreadCount_)
                {
                    // TODO log: thread count exceed, set to max.
                    value = crawlerSettings_.maxThreadCount_;
                }
                // change thread value
                threadsCount = value;
            }
        }

        // engine state
        private EngineState state;
        public EngineState state_
        {
            get { return state; }
            set { state = value; }
        }

        // constructor
        public CrawlerEngine(CrawlerSettings cSetting, CrawlerInput cInput, CrawlerOutput cOutput)
        {
            this.crawlerSettings_ = cSetting;
            this.crawlerInput_ = cInput;
            this.crawlerOutput_ = cOutput;

            queueUrls_ = new Queue();
            this.threadsRunning_ = new Thread[cSetting.maxThreadCount_];
        }

        // 
        private void MyMethod()
        {

        }

        // begin download and parse web site
        public int RunCrawling() 
        {
            state_ = EngineState.engine_state_running;
            string fullUrl = crawlerInput_.fullUrl_;
            CrawlerUri.Normalize(ref fullUrl);
            CrawlerUri seedUri = new CrawlerUri(fullUrl);
            EnqueueUri(seedUri, true);
            crawlerInput_.fullUrl_ = fullUrl;
            threadsCount_ = crawlerSettings_.threadsCount_;

            System.Diagnostics.Debug.Assert(crawlerOutput_.FindCrawlerUri(seedUri) == null);
            crawlerOutput_.AddUri(seedUri);

            try
            {
                for (int nIndex = 0; nIndex < threadsCount_; nIndex++)
                {
                    // check if thread not created or not suspended
                    if (threadsRunning_[nIndex] == null || threadsRunning_[nIndex].ThreadState != ThreadState.Suspended)
                    {
                        // create new thread
                        threadsRunning_[nIndex] = new Thread(new ThreadStart(ThreadStartFunction));
                        // set thread name equal to its index
                        threadsRunning_[nIndex].Name = nIndex.ToString();
                        // start thread working function
                        threadsRunning_[nIndex].Start();
                        // join
                        threadsRunning_[nIndex].Join();
                    }
                }
            }
            catch (Exception ex)
            {
                string errMsg = ex.Message;
            }

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

        private void ThreadStartFunction()
        {
            CrawlerWebRequest request = null;
            while ( state_ == EngineState.engine_state_running && 
                    int.Parse(Thread.CurrentThread.Name) < this.threadsCount)
            {
                CrawlerUri uri = DequeueUri();
                if (uri != null && uri.depth_ <= crawlerSettings_.maxDepth_)
                {
                    if (crawlerOutput_.FindCrawlerUri(uri) != null) // this uri has been parsed.
                    {
                        continue; // do not parse it.
                    }
                    ParseUri(uri, ref request);
                    if (crawlerSettings_.sleepConnectTime_ > 0)
                        Thread.Sleep(crawlerSettings_.sleepConnectTime_ * 1000);
                }
                else
                {
                    Thread.Sleep(crawlerSettings_.sleepFetchTime_ * 1000);
                }
            }
        }


        private void ParseUri(CrawlerUri uri, ref CrawlerWebRequest request)
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
                    EnqueueUri(new CrawlerUri(response.responseUri_.AbsoluteUri), true);
                    request = null;
                    return;
                }

                // check for allowed MIME types
                if (crawlerSettings_.allowAllMIMETypes_ == false &&
                    crawlerSettings_.allowedMIMETypes_.Length > 0 &&
                    response.contentType_ != null)
                {
                    // sample response.contentType: text/html; charset=utf-8
                    string strContentType = response.contentType_.ToLower();
                    int nIndex = strContentType.IndexOf(';');
                    if (nIndex != -1)
                        strContentType = strContentType.Substring(0, nIndex);

                    nIndex = crawlerSettings_.allowedMIMETypes_.IndexOf(strContentType);
                    if (strContentType.IndexOf('*') == -1 && nIndex == -1)
                    {
                        // log: this MIME type is not listed.
                        request = null;
                        uri.state_ = CrawlerUriParseState.uri_state_not_allowed_MIME_type; 
                        return;
                    }

                    // find numbers
                    Match match = new Regex(@"\d+").Match(crawlerSettings_.allowedMIMETypes_, nIndex);
                    int nMin = int.Parse(match.Value) * 1024;
                    match = match.NextMatch();
                    int nMax = int.Parse(match.Value) * 1024;
                    if (nMin < nMax && (response.contentLength_ < nMin || response.contentLength_ > nMax))
                    {
                        // TODO: Content's length is not correct, minimize length is nMin, maximize length is nMax,
                        // but content's length is response.contentLength_
                        request = null;
                        return;
                    }
                }

                // check for excluded response file extension
                bool shouldBeParsed = true;
                foreach (string ext in crawlerSettings_.excludeFiles_)
                {
                    if (ext.Trim().Length > 0 && uri.AbsoluteUri.ToLower().EndsWith(ext) == true)
                    {
                        shouldBeParsed = false;
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
                byte[] recvBuffer = new byte[8192];
                int nBytes, nTotalBytes = 0;
                // loop to receive response buffer
                while ((nBytes = response.socket_.Receive(recvBuffer, 0, 8192, SocketFlags.None)) > 0)
                {
                    // increment total received bytes
                    nTotalBytes += nBytes;
                    // write received buffer to file
                    bWriter.Write(recvBuffer, 0, nBytes);
                    // check if the uri type not binary to can be parsed for refs
                    if (shouldBeParsed == true)
                    {
                        // add received buffer to response string
                        strResponse += Encoding.ASCII.GetString(recvBuffer, 0, nBytes);
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

                if (shouldBeParsed == true)
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
                    crawlerOutput_.uriCount_ += matches.Count;
                    foreach (Match match in matches)
                    {
                        strRef = match.Value.Substring(match.Value.IndexOf('=') + 1).Trim('"', '\'', '#', ' ', '>');
                        try
                        {
                            if (strRef.IndexOf("..") != -1 || strRef.StartsWith("/") == true || strRef.StartsWith("http://") == false)
                                strRef = new Uri(uri, strRef).AbsoluteUri;
                            CrawlerUri.Normalize(ref strRef);
                            CrawlerUri newUri = new CrawlerUri(strRef);
                            if (newUri.Scheme != Uri.UriSchemeHttp && newUri.Scheme != Uri.UriSchemeHttps)
                                continue;
                            if (newUri.Host != uri.Host && crawlerSettings_.keepSameServer_ == true)
                                continue;
                            newUri.depth_ = uri.depth_ + 1;
                            EnqueueUri(newUri, true);
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

        // pop uri from the queue
        private CrawlerUri DequeueUri()
        {
            CrawlerUri uri = null;
            Monitor.Enter(queueUrls_);
            try
            {
                uri = (CrawlerUri)queueUrls_.Dequeue();
            }
            catch (Exception)
            {
            }
            Monitor.Exit(queueUrls_);
            return uri;
        }

        // push uri to the queue
        private bool EnqueueUri(CrawlerUri uri, bool bCheckRepetition = true)
        {
            // check whether the uri has been parsed or not
            if (bCheckRepetition == true && 
                crawlerOutput_.FindCrawlerUri(uri) != null)
                return false; // the uri has been parsed already, do not parse it again

            Monitor.Enter(queueUrls_);
            try
            {
                // add the uri to the queue
                queueUrls_.Enqueue(uri);
            }
            catch (Exception)
            {
            }
            Monitor.Exit(queueUrls_);

            return true;
        }

        private string UrlStringToNormal(string strUrl)
        {
            strUrl.Replace("%20", " ");
            return strUrl;
        }
    }

    public enum EngineState
    {
        engine_state_pause,
        engine_state_running,
        engine_state_stop,
    }

}
