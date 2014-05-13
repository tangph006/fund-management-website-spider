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
using CrawlerCommon;

namespace Output
{
    public class CrawlerOutput
    {
        // unique Uri's queue
        private Queue queueUrls;
        public Queue queueUrls_
        {
            get { return queueUrls; }
            set { queueUrls = value; }
        }

        // binary tree to keep unique Uri's
        private CrawlerSortTree urlStorage;
        public CrawlerSortTree urlStorage_
        {
            get { return urlStorage; }
            set { urlStorage = value; }
        }

        // downloaded file count
        private long fileCount;
        public long fileCount_
        {
            get { return fileCount; }
            set { fileCount = value; }
        }

        // downloaded file data byte count
        private long byteCount;
        public long byteCount_
        {
            get { return byteCount; }
            set { byteCount = value; }
        }

        // downloaded URL count
        private long URLCount;
        public long URLCount_
        {
            get { return URLCount; }
            set { URLCount = value; }
        }

        // pop uri from the queue
        public CrawlerUri DequeueUri()
        {
            Monitor.Enter(queueUrls_);
            CrawlerUri uri = null;
            try
            {
                uri = (CrawlerUri)queueUrls_.Dequeue();
            }
            catch (Exception)
            {
            }
            return uri;
        }

        // push uri to the queue
        public bool EnqueueUri(CrawlerUri uri, bool bCheckRepetition)
        {
            // add the uri to the binary tree to check if it is duplicated or not
            if (bCheckRepetition == true && AddURL(ref uri) == false)
                return false;

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

        // add URL 
        public bool AddURL(ref CrawlerUri uri)
        {
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
}
