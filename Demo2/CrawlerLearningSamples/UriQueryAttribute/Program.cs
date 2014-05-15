using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace UriQueryAttribute
{
    class Program
    {
        static void Main(string[] args)
        {
            Uri baseUri = new Uri("http://quote.eastmoney.com/");
            Uri myUri = new Uri(baseUri, "f1.html?code=600000&market=1");

            Console.WriteLine(myUri.Query);
        }
    }
}
