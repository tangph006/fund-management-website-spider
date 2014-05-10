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
namespace Engine
{
    public class CrawlerEngine
    {
        public CrawlerSettings CrawlerSettings { get; set; }
        public CrawlerInput CrawlerInput { get; set; }
        public CrawlerOutput CrawlerOutput { get; set; }

        // flag to be used to stop all running threads when user request to stop
        public bool ThreadsRunning { get; set; }

        // constructor
        public CrawlerEngine(CrawlerSettings cSetting, CrawlerInput cInput, CrawlerOutput cOutput)
        {
            this.CrawlerSettings = cSetting;
            this.CrawlerInput = cInput;
            this.CrawlerOutput = cOutput;
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
    }
}
