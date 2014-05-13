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
            CrawlerInput input = new CrawlerInput();
            CrawlerOutput output = new CrawlerOutput();
            CrawlerEngine engine = new CrawlerEngine(settings, input, output);

            engine.RunCrawling();
        }
    }
}
