using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using Microsoft.ServiceBus.Messaging;
using Newtonsoft.Json;
using System.Configuration;

namespace PHMessageSimulator
{
    class Program
    {
        static string eventHubName = ConfigurationManager.AppSettings["eventHubName"];
        static string connectionString = ConfigurationManager.AppSettings["connectionString"];
        static void Main(string[] args)
        {
            Console.WriteLine("Press Ctrl-C to stop the sender process");
            Console.WriteLine("Press Enter to start now");
            Console.ReadLine();
            SendingRandomMessages();
        }

        static void SendingRandomMessages()
        {
            var eventHubClient = EventHubClient.CreateFromConnectionString(connectionString, eventHubName);
            while (true)
            {
                Random rnd = new Random();
                try
                {
                    PHReading tPhReading = new PHReading();

                    tPhReading.NurseryId = 1;
                    tPhReading.RowId = 2;
                    tPhReading.SensorId = 3;
                    tPhReading.Strain = "Florida";
                    tPhReading.MinValue = 1;
                    tPhReading.MaxValue = 6;
                    tPhReading.CurrentValue = Math.Round((rnd.NextDouble() * 10), 1);
                    tPhReading.SensorType = "PH";

                    var serializedString = JsonConvert.SerializeObject(tPhReading);

                    Console.WriteLine("{0} > Sending message: {1}", DateTime.Now, serializedString);
                    eventHubClient.Send(new EventData(Encoding.UTF8.GetBytes(serializedString)));
                }
                catch (Exception exception)
                {
                    Console.ForegroundColor = ConsoleColor.Red;
                    Console.WriteLine("{0} > Exception: {1}", DateTime.Now, exception.Message);
                    Console.ResetColor();
                }

                Thread.Sleep(3000);
            }
        }


    }

    class PHReading
    {
        public int NurseryId { get; set; }
        public int RowId { get; set; }
        public int SensorId { get; set; }
        public string Strain { get; set; }
        public double MinValue { get; set; }
        public double MaxValue { get; set; }
        public double CurrentValue { get; set; }
        public string SensorType { get; set; }

        public DateTime DateCreated = DateTime.Now;
    }

}
