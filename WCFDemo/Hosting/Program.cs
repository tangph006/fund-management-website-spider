 using System;
 using System.ServiceModel;
using System.ServiceModel.Description;
 using Artech.WcfServices.Contracts;
 using Artech.WcfServices.Services;
namespace Artech.WcfServices.Hosting
{
    class Program
    {
        static void Main(string[] args)
        {
            using (ServiceHost host = new ServiceHost(typeof(CalculatorService)))
            {
//                 host.AddServiceEndpoint(typeof(ICalculator), new WSHttpBinding(), "http://127.0.0.1:9999/calculatorservice");
//                 if (host.Description.Behaviors.Find<ServiceMetadataBehavior>() == null)
//                 {
//                     ServiceMetadataBehavior behavior = new ServiceMetadataBehavior();
//                     behavior.HttpGetEnabled = true;
//                     behavior.HttpGetUrl = new Uri("http://127.0.0.1:9999/calculatorservice/metadata");
//                     host.Description.Behaviors.Add(behavior);
//                 }
                host.Opened += delegate
                {
                    Console.WriteLine("Host program is running, press any key to quit...");
                };

                host.Open();
                Console.Read();
            }
        }
    }
}
