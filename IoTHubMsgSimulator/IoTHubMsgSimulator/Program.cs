using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.Azure.Devices.Client;
using Newtonsoft.Json;
using System.Configuration;

namespace IoTHubMsgSimulator
{
    class Program
    {
        static DeviceClient deviceClient;
        static string iotHubUri = ConfigurationManager.AppSettings["iotHubUri"];
        static string deviceKey = ConfigurationManager.AppSettings["deviceKey"];
        static void Main(string[] args)
        {
            Console.WriteLine("Simulated device\n");
            deviceClient = DeviceClient.Create(iotHubUri, new DeviceAuthenticationWithRegistrySymmetricKey("phsensor", deviceKey));

            SendDeviceToCloudMessagesAsync();
            Console.ReadLine();
        }

        private static async void SendDeviceToCloudMessagesAsync()
        {

            Random rnd = new Random();

            while (true)
            {

                SensorReading tSensorReading = new SensorReading();

                tSensorReading.NurseryId = 1;
                tSensorReading.RowId = 222;
                tSensorReading.SensorId = 99;
                tSensorReading.Strain = "Arduino";
                tSensorReading.MinValue = 1;
                tSensorReading.MaxValue = 6;
                tSensorReading.CurrentValue = Math.Round((rnd.NextDouble() * 8), 1);
                tSensorReading.SensorType = "PH";

                var messageString = JsonConvert.SerializeObject(tSensorReading);
                var message = new Message(Encoding.ASCII.GetBytes(messageString));

                await deviceClient.SendEventAsync(message);
                Console.WriteLine("{0} > Sending message: {1}", DateTime.Now, messageString);

                Task.Delay(1000).Wait();
            }
        }
        class SensorReading
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
}
