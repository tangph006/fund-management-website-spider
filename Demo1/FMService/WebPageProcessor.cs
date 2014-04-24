using System;
using System.IO;
using System.Net;
using System.Text.RegularExpressions;

using FM.Util;

namespace FM.Service.WebSpider
{
    /// <summary>
    /// WebPageProcessor is an implementation of the IWebPageProcessor that 
    /// does the actually work of reading in the content from an URI and calls
    /// the process content delegates which perform additional work.
    /// </summary>
    public class WebPageProcessor : IWebPageProcessor
    {
        #region public interface
        /// <summary>
        /// Process performs the action of reading in the contents from the URI 
        /// assigned to the WebPageState object that is passed in.
        /// <param name="state">The state object containst the URI to process and will hold onto state regarding the URI as it is processed</param>
        /// <returns>True if the process worked without exception</returns>
        /// </summary>
        public bool Process(WebPageState webPageState)
        {
            webPageState.processStarted_ = true;
            webPageState.processSuccessfull_ = false;

            try
            {
                WebRequest req = WebRequest.Create(webPageState.uri_);
                WebResponse res = null;

                try
                {
                    res = req.GetResponse();

                    if (res is HttpWebResponse)
                    {
                        webPageState.statusCode_ = ((HttpWebResponse)res).StatusCode.ToString();
                        webPageState.statusDescription_ = ((HttpWebResponse)res).StatusDescription;
                    }
                    if (res is FileWebResponse)
                    {
                        webPageState.statusCode_ = "OK";
                        webPageState.statusDescription_ = "OK";
                    }

                    if (webPageState.statusCode_.Equals("OK"))
                    {
                        StreamReader sr = new StreamReader(res.GetResponseStream());

                        webPageState.content_ = sr.ReadToEnd();

                        if (contentHandler_ != null)
                        {
                            contentHandler_(webPageState);
                        }
                    }

                    webPageState.processSuccessfull_ = true;
                }
                catch (Exception ex)
                {
                    HandleException(ex, webPageState);
                }
                finally
                {
                    if (res != null)
                    {
                        res.Close();
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.ToString());
            }
            Console.WriteLine("Successfull: {0}", webPageState.processSuccessfull_);

            return webPageState.processSuccessfull_;
        }
        #endregion

        #region local interface
        // Assign status code and description based on thrown exception
        private void HandleException(Exception ex, WebPageState state)
        {
            if (ex is WebException && LookupWebException(ex.ToString(), state, new String[] { 
                                                                                               "(400) Bad Request", 
                                                                                               "(401) Unauthorized", 
                                                                                               "(402) Payment Required", 
                                                                                               "(403) Forbidden", 
                                                                                               "(404) Not Found", 
                                                                                               "(405) Method not allowed", 
                                                                                               "(406) Page format not understood", 
                                                                                               "(407) Request must be authorized first", 
                                                                                               "(408) Request timed out", 
                                                                                               "(409) Conflict, to many requests for resource", 
                                                                                               "(410) Page use to be there, but now it's gone", 
                                                                                               "(411) Content-length missing", 
                                                                                               "(412) Pre-condition not met", 
                                                                                               "(413) Too big", 
                                                                                               "(414) URL is to long", 
                                                                                               "(415) Unsupported media type", 
                                                                                               "(500) Internal Error", 
                                                                                               "(501) Not implemented", 
                                                                                               "(502) Bad Gateway", 
                                                                                               "(503) Server Unavailable", 
                                                                                               "(504) Gateway Timeout",
                                                                                               "(505) HTTP not supported" }))
            {
                return;
            }

            if (ex.InnerException != null && ex.InnerException is FileNotFoundException)
            {
                state.statusCode_ = "FileNotFound";
                state.statusDescription_ = ex.InnerException.Message;
            }
            else
            {
                state.statusDescription_ = ex.ToString();
            }
        }

        // Each web error such as 404 does not show up as specific error so lookup the code from a WebException
        private bool LookupWebException(string ex, WebPageState state, string[] errors)
        {
            foreach (string error in errors)
            {
                string errCode = error.Substring(0, 5);
                if (ex.IndexOf(errCode) != -1)
                {
                    state.statusCode_ = errCode;
                    state.statusDescription_ = error;
                    return true;
                }
            }
            return false;
        }

        #endregion

        #region properties
        public WebPageContentDelegate contentHandler_ { get; set; }
        #endregion
    }
}
