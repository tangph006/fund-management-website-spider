using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace CrawlerCommon
{
    public class CrawlerCommons
    {
        public static void WriteDataString(StreamWriter sWriter, string strData)
        {
        }

        public static string[] ReadJsonStringArray(StreamReader sReader)
        {
            string[] strArray = {"1", "2", "3"};
            return strArray;
        }

        public static string ReadJsonString(StreamReader sReader)
        {
            return "123";
        }
    }
}
