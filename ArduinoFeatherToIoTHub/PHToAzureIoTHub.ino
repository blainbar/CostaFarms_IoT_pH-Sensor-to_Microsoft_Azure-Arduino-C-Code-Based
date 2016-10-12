/*
  This code connects to an unencrypted Wifi network.
  It then prints the  MAC address,IP address obtained,
  and other network details of the Wifi Shield.
  It then pulls data from a pH Sensor and displays
  the data to a Azure SQL Server database.

  Hardware used is a Arduino Adafruit Feather MO Wifi

  created 13 July 2010 by dlf (Metodo2 srl)
  modified by Blain Barton Oct4 2016 for Arduino
  SAS token created on Oct2 and valid for 365 days only.
*/

//include libraries
#include <SPI.h>
#include <WiFi101.h>
#include <Wire.h>
#define address 99

// declare local variables
char computerdata[20];
byte received_from_computer = 0;
byte code = 0;
char ph_data[20];
byte in_char = 0;
byte i = 0;
int time_ = 1800;

//your current wifi SSID and password, make sure a chance to your settings
char ssid[] = "SSID";                              // the name of your network
char pass[] = "password";                          // the password of your network
int status = WL_IDLE_STATUS;                       // the Wifi radio's status

//azure connection string - SAS token good for 365 days
WiFiSSLClient client;
char hostname[] = "Whatevername.azure-devices.net"; // host name address for your Azure IoT Hub
char authSAS[] = "SharedAccessSignature sr=Whatevertoken";

void setup() {
  WiFi.setPins(8, 7, 4, 2);
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to open SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // you're connected now, so print out the data:
  Serial.print("You're connected to wifi network");
  printCurrentNet();
  printWifiData();
}


void loop() {
  // check the network connection once every 10 seconds:
  delay(10000);
  printCurrentNet();

  // get ph sensor data and put in local variable ph_data
  getPHValue();

  String uri = "/devices/phsensor/messages/events?api-version=2016-02-03";
  String myData = "{\"DateCreated\":2016-09-29,\"NurseryId\":1,\"RowId\":222,\"SensorId\":99,\"Strain\":\"Arduino\",\"MinValue\":1.0,\"MaxValue\":6.0,\"CurrentValue\":PHVALUE,\"SensorType\":\"PH\"}";

  // add ph value to json string
  myData.replace("PHVALUE", ph_data);

  // send data to Azure
  httpRequest("POST", uri, "application/atom+xml;type=entry;charset=utf-8", myData);
}

void getPHValue()
{
  // reads sensor pH data
  Serial.println("--- start reading_ph from sensor ----");
  Wire.begin();

  time_ = 1800;
  Wire.beginTransmission(address);
  Wire.write('R');
  Wire.endTransmission();
  delay(time_);
  Wire.requestFrom(address, 20, 1);
  code = Wire.read();
  switch (code) {                    //switch case based on what the response code is.
    case 1:                          //decimal 1.
      Serial.println("Success");     //means the command was successful.
      break;                         //exits the switch case.

    case 2:                         //decimal 2.
      Serial.println("Failed");     //means the command has failed.
      break;                        //exits the switch case.

    case 254:                      //decimal 254.
      Serial.println("Pending");   //means the command has not yet been finished calculating.
      break;                         //exits the switch case.

    case 255:                      //decimal 255.
      Serial.println("No Data");   //means there is no further data to send.
      break;                       //exits the switch case.
  }

  //concat data plus PH plus current time
  while (Wire.available()) {         //are there bytes to receive.
    in_char = Wire.read();           //receive a byte.
    ph_data[i] = in_char;            //load this byte into our array.
    i += 1;                          //incur the counter for the array element.
    if (in_char == 0) {              //if we see that we have been sent a null command.
      i = 0;                         //reset the counter i to 0.
      Wire.endTransmission();        //end the I2C data transmission.
      break;                         //exit the while loop.
    }
  }
  Serial.println(ph_data);           //print the data.
}

// connect to wifi
void printWifiData() {

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  Serial.print(mac[5], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.println(mac[0], HEX);

  // print your subnet mask:
  IPAddress subnet = WiFi.subnetMask();
  Serial.print("NetMask: ");
  Serial.println(subnet);

  // print your gateway address:
  IPAddress gateway = WiFi.gatewayIP();
  Serial.print("Gateway: ");
  Serial.println(gateway);
}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  Serial.print(bssid[5], HEX);
  Serial.print(":");
  Serial.print(bssid[4], HEX);
  Serial.print(":");
  Serial.print(bssid[3], HEX);
  Serial.print(":");
  Serial.print(bssid[2], HEX);
  Serial.print(":");
  Serial.print(bssid[1], HEX);
  Serial.print(":");
  Serial.println(bssid[0], HEX);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
}

// request (POST) call to Azure
void httpRequest(String verb, String uri, String contentType, String content)
{
  Serial.println("--- Start Process --- ");
  if (verb.equals("")) return;
  if (uri.equals("")) return;

  // close any connection before send a new request.
  // This will free the socket on the WiFi shield
  client.stop();

  // if there's a successful connection:
  if (client.connect(hostname, 443)) {
    Serial.println("--- We are Connected --- ");
    Serial.print("*** Sending Data To:  ");
    Serial.println(hostname + uri);
    client.print(verb); //send POST, GET or DELETE
    client.print(" ");
    client.print(uri);  // any of the URI
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(hostname);  //with hostname header
    client.print("Authorization: ");
    client.println(authSAS);  //Authorization SAS token obtained from Azure IoT device explorer

    if (verb.equals("POST"))
    {
      Serial.println("--- Sending POST ----");
      client.print("Content-Type: ");
      client.println(contentType);
      client.print("Content-Length: ");
      client.println(content.length());
      client.println();
      client.println(content);
    } else
    {
      client.println();
    }
  }

  Serial.println("--- Trying to Read Client ----");
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }
  Serial.println("--- End ----");
}





