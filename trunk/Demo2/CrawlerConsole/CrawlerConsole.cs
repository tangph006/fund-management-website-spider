using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;
using Logger;
using log4net;
using Input;
using Output;
using Settings;
using Engine;

namespace CrawlerConsole
{
    class CrawlerConsoleMain
    {
        static void Main(string[] args)
        {
            CrawlerSettings settings = new CrawlerSettings();
            settings.allMIMETypes_ = false;
            settings.downloadfolder_ = ";";
            settings.excludeFiles_ = new string[] { "" };
            settings.excludeHosts_ = new string[] { "" };
            settings.excludeWords_ = new string[] { "" };
            settings.keepAlive_ = false;
            settings.keepSameServer_ = false;
            settings.lastRequestCount_ = 0;
            settings.MIMETypes_ = "";
            settings.requestTimeout_ = 10;
            settings.sleepConnectTime_ = 5;
            settings.sleepFetchTime_ = 5;
            settings.threadsCount_ = 1;
            settings.webDepth_ = 1;
            settings.filePath_ = "CrawlerConsoleSettings.txt";
            settings.lastModified_ = DateTime.Now;
            settings.version_ = 1;
            settings.dataTypeName_ = "CrawlerConsoleSettings";
            settings.WriteToFile();
            settings.ReadFromFile();

            CrawlerInput input = new CrawlerInput();
            input.filePath_ = "CrawlerConsoleInput.txt";
            input.lastModified_ = DateTime.Now;
            input.version_ = 1;
            input.dataTypeName_ = "CrawlerConsoleInput";
            input.domain_ = "baidu.com";
            input.fullUrl_ = "www.baidu.com";

            CrawlerOutput output = new CrawlerOutput();
            CrawlerEngine engine = new CrawlerEngine(settings, input, output);

            engine.RunCrawling();
        }
    }
}
