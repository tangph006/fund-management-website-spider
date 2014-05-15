using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.Serialization;

namespace Cache
{
    [DataContract]
    public class CrawlerCache
    {
        [DataMember(Order = 0)]
        // history url list
        private List<string> historyUrls;
        public List<string> historyUrls_
        {
            get { return historyUrls; }
            set { historyUrls = value; }
        }

        // add url to history url list, and make sure max count is 20.
        public void AddUrlToHistoryUrlList(string url)
        {
            historyUrls.Remove(url);
            historyUrls.Add(url);
            while (historyUrls.Count > 20)
            {
                historyUrls.RemoveAt(0);
            }
        }
    }
}
