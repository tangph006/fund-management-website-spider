using System;
using System.Collections;
using System.Text.RegularExpressions;
using System.IO;
using System.Net;
using System.Windows.Forms;
using System.Text;

using FM.Util;

namespace FM.Service.WebSpider
{
    /// <summary>
    /// Summary description for Spider.
    /// </summary>
    public class WebSpider
    {
        #region properties
        public IWebPageProcessor webPageProcessor_ { get; set; }
        public Uri startUri_ { get; set; }
        public Uri baseUri_ { get; set; }
        public int uriProcessedCount_ { get; set; }
        public int uriProcessedCountMax_ { get; set; }
        public bool keepWebContent_ { get; set; }
        public Hashtable webPages_ { get; set; }
        public Queue webPagesPending_ { get; set; }
        #endregion

        public WebSpider(string startUri) : this(startUri, -1) { }

        public WebSpider(string startUri, int uriProcessedCountMax) :
            this(startUri, "", uriProcessedCountMax, false, new WebPageProcessor()) { }

        public WebSpider(string startUri, string baseUri, int uriProcessedCountMax) :
            this(startUri, baseUri, uriProcessedCountMax, false, new WebPageProcessor()) { }

        public WebSpider(string startUri, string baseUri, int uriProcessedCountMax, bool keepWebContent, IWebPageProcessor webPageProcessor)
        {
            startUri_ = new Uri(startUri);

            // In future this could be null and will process cross-site, but for now must exist
            baseUri_ = new Uri(Is.EmptyString(baseUri) ? startUri_.GetLeftPart(UriPartial.Authority) : baseUri);

            uriProcessedCountMax_ = uriProcessedCountMax;
            keepWebContent_ = keepWebContent;

            webPagesPending_ = new Queue();
            webPages_ = new Hashtable();

            webPageProcessor_ = webPageProcessor;

            webPageProcessor_.contentHandler_ += new WebPageContentDelegate(this.HandleLinks);
        }

        #region public interface
        public void Execute()
        {
            uriProcessedCount_ = 0;
            DateTime start = DateTime.Now;
            AddWebPage(startUri_, startUri_.AbsoluteUri);
            try
            {
                while (webPagesPending_.Count > 0 && (uriProcessedCountMax_ == -1 || uriProcessedCount_ < uriProcessedCountMax_))
                {
                    WebPageState webPageState = (WebPageState)webPagesPending_.Dequeue();
                    webPageProcessor_.Process(webPageState);
                    if (!keepWebContent_)
                    {
                        webPageState.content_ = null;
                    }

                    uriProcessedCount_++;
                }
            }
            catch (Exception ex)
            {
                StringBuilder strBuilder = new StringBuilder();
                strBuilder.AppendFormat(@"Error occurs in Execute(), error message: {0}", ex.Message);
                MessageBox.Show(strBuilder.ToString(), "error");
            }

            DateTime end = DateTime.Now;
            float elasped = (end.Ticks - start.Ticks) / 10000000;
        }

        public void HandleLinks(WebPageState state)
        {
            if (state.processInstructions_.IndexOf("Handle Links") != -1)
            {
                int counter = 0;
                Match m = RegExUtil.GetMatchRegEx(RegularExpression.UrlExtractor, state.content_);

                while (m.Success)
                {
                    if (AddWebPage(state.uri_, m.Groups["url"].ToString()))
                    {
                        counter++;
                    }

                    m = m.NextMatch();
                }
            }
        }

        private bool AddWebPage(Uri baseUri, string newUri)
        {
            // Remove any anchors
            string url = StrUtil.LeftIndexOf(newUri, "#");

            // Construct a Uri, using the current page Uri as a base reference
            Uri uri = new Uri(baseUri, url);

            if (!IsValidPage(uri.LocalPath) || webPages_.Contains(uri))
            {
                return false;
            }
            WebPageState webPageState = new WebPageState(uri);

            // Only process links for pages within the same site. 
            if (uri.AbsoluteUri.StartsWith(baseUri_.AbsoluteUri))
            {
                webPageState.processInstructions_ += "Handle Links";
            }

            webPagesPending_.Enqueue(webPageState);
            webPages_.Add(uri, webPageState);

            return true;
        }
        #endregion

        #region local interface
        private static readonly string[] m_validPageExtensions = new string[] { "html", "php", "asp", "htm", "jsp", "shtml", "php3", "aspx", "pl", "cfm" };

        // A page is considered valid if there is no extension or if the
        // last character is a /.  If there is an extension then the page
        // is considered valid if that extension is classed as valid.
        private bool IsValidPage(string path)
        {
            int pos = path.IndexOf(".");

            if (pos == -1 || path[path.Length - 1] == 47) /*.ToString( ).Equals( "/" )*/
            {
                return true;
            }

            string uriExt = StrUtil.RightOf(path, pos).ToLower();

            // Uri ends in an extension
            foreach (string ext in m_validPageExtensions)
            {
                if (uriExt.Equals(ext))
                {
                    return true;
                }
            }

            return false;
        }
        #endregion


    }
}
