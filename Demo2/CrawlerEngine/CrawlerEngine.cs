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
using Engine;

namespace Engine
{
    public class CrawlerEngine
    {
        public CrawlerSettings CrawlerSettings { get; set; }
        public CrawlerInput CrawlerInput { get; set; }
        public CrawlerOutput CrawlerOutput { get; set; }

        // flag to be used to stop all running threads when user request to stop
        public bool ThreadsRunning { get; set; }

        public CrawlerEngine(CrawlerSettings cSetting, CrawlerInput cInput, CrawlerOutput cOutput)
        {
            this.CrawlerSettings = cSetting;
            this.CrawlerInput = cInput;
            this.CrawlerOutput = cOutput;
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
            foreach (string str in CrawlerSettings.ExcludeHosts)
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
}
