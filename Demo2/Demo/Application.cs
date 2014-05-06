using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;

namespace Demo
{
    public partial class App : Application
    {
        // user settings
        public SettingT settingT;
        // user input
        public InputT inputT;
        // output of parse/crawl result
        public OutputT outputT;
        // the hard working man
        public ParserT parserT;
    }
}
