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
            string strMIMETypes = @"text/richtext[0,0];text/html[0,0];audio/x-aiff[0,0];";
            strMIMETypes += @"audio/basic[0,0];audio/wav[0,0];image/gif[0,0];image/jpeg[0,0];";
            strMIMETypes += @"image/pjpeg[0,0];image/tiff[0,0];image/x-png[0,0];image/x-xbitmap[0,0];";
            strMIMETypes += @"image/bmp[0,0];image/x-jg[0,0];image/x-emf[0,0];image/x-wmf[0,0];";
            strMIMETypes += @"video/avi[0,0];video/mpeg[0,0];application/postscript[0,0];application/base64[0,0];";
            strMIMETypes += @"application/macbinhex40[0,0];application/pdf[0,0];application/x-compressed[0,0];";
            strMIMETypes += @"application/x-zip-compressed[0,0];application/x-gzip-compressed[0,0];";
            strMIMETypes += @"application/java[0,0];application/x-msdownload[0,0];";
            CrawlerSettings settings = new CrawlerSettings();
            settings.allowAllMIMETypes_ = false;
            settings.downloadfolder_ = "downloadfolder1";
            settings.excludeFiles_ = new string[] { ".gif", ".jpg", ".css", ".zip", ".exe" };
            settings.excludeHosts_ = new string[] { "" };
            settings.excludeWords_ = new string[] { "" };
            settings.keepAlive_ = false;
            settings.keepSameServer_ = false;
            settings.lastRequestCount_ = 0;
            settings.allowedMIMETypes_ = strMIMETypes;
            settings.requestTimeout_ = 10;
            settings.sleepConnectTime_ = 0;
            settings.sleepFetchTime_ = 0;
            settings.threadsCount_ = 1;
            settings.maxThreadCount_ = 20;
            settings.maxDepth_ = 1;
            settings.filePath_ = "CrawlerConsoleSettings.txt";
            settings.lastModified_ = DateTime.Now;
            settings.version_ = 1;
            settings.dataTypeName_ = "CrawlerConsoleSettings";
            settings.WriteToFile();
            settings.ReadFromFile("CrawlerConsoleSettings.txt");

            CrawlerInput input = new CrawlerInput();
            input.domain_ = "baidu.com";
            input.fullUrl_ = "www.baidu.com";

            CrawlerOutput output = new CrawlerOutput();
            CrawlerEngine engine = new CrawlerEngine(settings, input, output);

            engine.RunCrawling();
        }
    }
}
