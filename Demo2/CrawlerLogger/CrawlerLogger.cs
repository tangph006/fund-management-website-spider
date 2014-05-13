using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using log4net;

namespace Logger
{
    public class CrawlerLogger
    {
        public static ILog CrawlerGetLogger(Type type)
        {
            return log4net.LogManager.GetLogger(type);
        }
    }
}
