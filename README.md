## Learning's from Costa Farms pH Flow Controls and Automation

Written by [Blain Barton](http://aka.ms/blainbarton), Senior Technical Evangelist

Professional video published here - https://channel9.msdn.com/Shows/Microsoft-Technical-Case-Studies/Costa-Farms

[Costa Farms](http://http://www.costafarms.com/) is a third-generation, family-owned group of companies headquartered in beautiful Miami, Florida (aka, plant paradise!). The company sprouted in 1961 when the founder, Jose Costa Sr., purchased 30 acres south of Miami to grow fresh, vine-ripened tomatoes in the winter and calamondin citrus in the summer. That soon morphed into houseplants, and the Costa Farms family started innovating and introduced new houseplants such as the canela tree, Orchids and Cecilia Aglaonema. Costa Farms sells to big box stores such as Walmart, Home Depot and Lowe's.![](https://github.com/blainbar/techcasestudies-private/blob/master/images/CostaTablestech.png)

## Core Team: ##

Mani Peddada - BI Architect, mpeddada@costafarms.com 

Rodney Sanchez - Software Developer, rsanchez@costafarms.com

Blain Barton - Microsoft Senior Technical Evangelist, blainbar@microsoft.com

Joe Raio - Microsoft Senior Technical Evangelist, Joe.Raio@Microsoft.com

David Crook - Microsoft Technical Evangelist, dcrook@microsoft.com!

[](https://github.com/blainbar/techcasestudies-private/blob/master/images/CostaCore.png)
## Pain Points ##

It's time consuming, costly, and difficult for growers to measure and regulate pH throughout the day and across the lifecycle of a plant.  Indeed, pH levels are one of the key factors in plant health. Furthermore, pH levels in the water and nutrient streams change constantly.  To increase plant health via nutrient uptake in turn promoting higher yield, pH needs to be more closely monitored and adjusted in real time.The Azure loT system helps Costa Farms increase revenue and profitability in using modern technology in Agriculture/Farming. Recalling optimal pH levels are essential for nutrient uptake. Increased nutrient uptake directly effects yields in turn driving improved revenue per plant harvested. On the cost side, this system will allow growers to be more productive and utilized across all growing activities as they spend less time manually checking pH values by hand.

## Solution ##
This solution uses Microsoft Azure IoT Hub with PH sensor devices on hydro water systems testing correct PH levels. The goal was to use PH sensors for testing water entering and leaving the customers hydro systems. We used the Adafruit Feather MO Wifi and pHSensor, Microsoft Azure and IoT Hub, Stream Analytics, Event Hub, Azure Functions, messaging via Twilio and SQL Azure. We built a "Proof of Concept" that customers can use in their solutions.Below is the overall flow of the system which we'll discuss in detail -![](https://github.com/blainbar/techcasestudies-private/blob/master/images/CostaIoT1.PNG)There are multiple technologies for this particular IoT solution. The code for the hardware and software for this solution is located at [https://github.com/blainbar/CostaFarmsIoT](https://github.com/blainbar/CostaFarmsIoT) 

First, lets start with the Hardware, the Adafruit Feather MO Wifi, breadboard, Atlas Scientific pHSensor, wires that connect everything and pH probe itself.Below is the breadboard drawing that shows the layout of the components. ![](https://github.com/blainbar/techcasestudies-private/blob/master/images/Costaboard1.png)

This is the Adafruit Feather M0 Wifi using the I2C Arduino C code -![](https://github.com/blainbar/techcasestudies-private/blob/master/images/CostaAdafruitfeather2.png)

This is the Atlas Scientific pH red circuit board -![](https://github.com/blainbar/techcasestudies-private/blob/master/images/CostaPhcircuit3.png)

Here's an image of the device we built starting with the pH Probe -![](https://github.com/blainbar/techcasestudies-private/blob/master/images/CostaPhprobe1.PNG)
For the Microsoft Azure Setup, we used IoT Hub, Stream Analytics, Azure SQL database, sending messages to Event Hub and to the Azure functions, then sending the messages based ob the range of the Min and Max values. 

This is the Microsoft Azure setup for Costa Farms. 

[](https://github.com/blainbar/techcasestudies-private/blob/master/images/Costaazureportalapps.PNG)

We started with the hardware and C code setup which starts the flow into Microsoft AzureNext, the messages are sent to IoT Hub.

• Here the messages go into SQL Azure database and stored to be used for Power BI.    

• The messages are then sent via Stream Analytics within SQL Azure seen here.   

• Event Hub then is used to receive the data coming in.   

• There is input and output from the IoT Hub via a QUERY that is in place to route the messages based on values.  

• QUERY states from SensorIoTHub to phalerts give us the the NurseryId, RowId, SensorId, Strain, MinValue, MaxValue, CurrentValue, DateCreated, SensorType, and also put everything into the Azure SQL database. The messages are then sent to Event Hub.  

• Event Hubs are configured for phalerts and phdata.   

• Event Hub then triggers phalerts to be sent.  

• Here we use Azure Functions in the flow to send messages via Twilio.   

• Here's the code that is used for the Event Hub messages, where we log the event, put the message object into class, create the message, setup and then send to Twilio services to a mobile phone.   

• Azure functions send messages based on the mix and max pH levels to Twilio.   

• phalert messages are then sent to the growers mobile phone. The final output sent to the mobile phone seen here.![](https://github.com/blainbar/techcasestudies-private/blob/master/images/Costafinaloutput.PNG)Here are some of the plants that are grown at Costa Farms. ![](https://github.com/blainbar/techcasestudies-private/blob/master/images/CostaFlowers.png)
##

“I have been able to learn some new and interesting technologies. It has been an amazing experience thanks to the Microsoft team that has worked with us: Blain Barton, Joe Raio and David Crook.The collaboration has been very good and it has been fun to build something from scratch that works and serves its purpose”.  

**Rodney Sánchez**- Software Developer

“Through this POC I’ve learned about IoT solution architecture that gets deployed using Azure services. Besides the obvious technical learning's from this process, I have also developed a broad understanding on ways to evaluate and design IoT device requirements, infrastructure requirements and Structure engagements. 

This has been an incredible learning experience, thank you David, Blain and Joe for coming out here and helping us expand our horizons”  

**Mani Mihira Peddada**-BI Architect

## Resources ##

[![Costa Farms Video](http://img.youtube.com/vi/xVkgiIojwCc/0.jpg)](http://www.youtube.com/watch?v=xVkgiIojwCc)


[Setting up the **hardware** for pHsensor](https://blogs.msdn.microsoft.com/blainbar/2016/10/25/hardware-assembly-for-the-adafruit-feather-m0-wifi-with-the-atlas-scientific-ph-sensor-for-remotely-monitoring-ph-water-levels-in-microsoft-azure-article-1-or-2/)

[Setting up the **software** for pHsensor](https://blogs.msdn.microsoft.com/blainbar/2016/10/25/setting-up-software-for-the-adafruit-feather-m0-wifi-using-the-arduino-ide-and-c-code-for-remotely-monitoring-ph-sensors-in-microsoft-azure-article-2-of-2)
