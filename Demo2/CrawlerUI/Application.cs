using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using Engine;

namespace Demo
{
    public partial class App : Application
    {
        // user settings
        public CrawlerSettings settingT;
        // user input
        public InputT inputT;
        // output of parse/crawl result
        public CrawlerOutput outputT;
        // the hard working man
        public CrawlerEngine parserT;
    }
}
