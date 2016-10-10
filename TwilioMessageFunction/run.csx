using System;
using Twilio;
using System.Configuration;
using Newtonsoft.Json;

public static void Run(string myEventHubMessage, TraceWriter log)
{
    // Log the Event
    log.Info($"C# Event Hub trigger function processed a message: {myEventHubMessage}");

    // Put the message object into class
    SensorReading myR = new SensorReading();
    myR = JsonConvert.DeserializeObject<SensorReading>(myEventHubMessage);

    // Create message
    string msg = $"Alert | Nursery: {myR.NurseryId}, Row: {myR.RowId}, Strain: {myR.Strain}, Sensor: {myR.SensorType} reading {myR.CurrentValue}";

    log.Info($"Sending Message: {msg}");

    // Setup Twilio
    string AccountSid = ConfigurationManager.AppSettings["TwilioAccountSID"];
    string AuthToken = ConfigurationManager.AppSettings["TwilioAuthToken"];
    var twilio = new TwilioRestClient(AccountSid, AuthToken);

    // Send to Twilio
    log.Info("Attempting to Send Message");
    var message = twilio.SendMessage(
        ConfigurationManager.AppSettings["TwilioFrom"],
        ConfigurationManager.AppSettings["TwilioTo"],
        msg
    );
    //log.Info("Message ID: " + message.Sid);

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
    public DateTime DateCreated { get; set; }
}