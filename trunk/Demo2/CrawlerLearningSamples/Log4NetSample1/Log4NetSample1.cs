using System;
using System.Collections.Generic;
using System.Text;
using System.Reflection;
using log4net;

[assembly: log4net.Config.XmlConfigurator(Watch = true)]
namespace Log4NetSample1
{
    /// <summary>
    /// 
    /// </summary>
    public class MainClass
    {
        public static void Main(string[] args)
        {
            //Application.Run(new MainForm());
            ILog log = log4net.LogManager.GetLogger(MethodBase.GetCurrentMethod().DeclaringType);
            // log error
            log.Error("this is a error information.", new Exception("an error occurs."));
            // log fatal error
            log.Fatal("this is a fatal error information.", new Exception("a fatal error occurs"));
            // log information
            log.Info("this is a log information.");
            // log debug information
            log.Debug("this is a debug information.");
            // log warning
            log.Warn("this is a warning information.");

            Console.Read();
        }
    }
}