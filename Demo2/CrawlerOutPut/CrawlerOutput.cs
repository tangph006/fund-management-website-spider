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
        private SortTree urlStorage;
        public SortTree urlStorage_;
    {

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
        bool EnqueueUri(CrawlerUri uri, bool bCheckRepetition)
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
        bool AddURL(ref CrawlerUri uri)
        {
            foreach (string str in settingT.ExcludeHosts)
            {
                if (str.Trim().Length > 0 && uri.Host.ToLower().IndexOf(str.Trim()) != -1)
                {
                    LogError(uri.AbsoluteUri, "\r\nHost excluded as it includes reserved pattern (" + str + ")");
                    return false;
                }
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
}
