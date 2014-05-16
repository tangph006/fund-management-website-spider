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
        // binary tree to keep Uris that parsed
        private CrawlerBinaryTree<CrawlerUri> uriStorage;
        public CrawlerBinaryTree<CrawlerUri> urlStorage_
        {
            get { return uriStorage; }
            set { uriStorage = value; }
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

        // downloaded Uri count
        private long uriCount;
        public long uriCount_
        {
            get { return uriCount; }
            set { uriCount = value; }
        }

        public CrawlerOutput()
        {
            urlStorage_ = new CrawlerBinaryTree<CrawlerUri>();
            fileCount_ = 0;
            byteCount_ = 0;
        }

        // add Uri to the storage 
        public bool AddUri(CrawlerUri uri)
        {
            Monitor.Enter(uriStorage);
            bool bRet = false;
            try
            {
                uriStorage.AddTreeNode(uri);
                uriCount_++;
            }
            catch (Exception)
            {
            }
            Monitor.Exit(uriStorage);

            return bRet;
        }

        // find Uri from urlStorage
        public CrawlerBinaryTreeNode<CrawlerUri> FindCrawlerUri(CrawlerUri uri)
        {
            return uriStorage.FindTreeNodeByData(uri);
        }
    }
}
