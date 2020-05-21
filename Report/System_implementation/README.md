# System implementation

In this part, we will go into details about how we implemented the design to the software and hardware.

## Contents

* [Implementation timelines](#_a1)
  * [Software timeline](#_a1)
  * [Hardware timeline](#_hardware)

<a name="_a1"></a>

## Implementation timelines

This project began in February 2020 and ended by the end of May. We have set relatively reasonable work plans and sprint goals to. We created the GitHub repository for this project on February 6 and spent a few days after that to get the team familiar with the GitHub platform by completing small practical tasks. We then allocated specific time periods for the different components and review the plan after each sprint.

![Gantt](Gantt.png)
### Things to note down first
As we can see in the system design section, the desktop program not only acts as the main body of the desktop program but also a server for the web and barrier ends. So for the program development, back-end needs to be implemented simultaneously with the web app and M5Stack development. The process went relatively smoothly, as our development team was relatively well-staffed, and the back-end developers could meet the design requirements of docking in both directions simultaneously. Because the desktop app uses object-oriented programming ideas, it can be constructed separately from the development front-end. 

As the requirements were raised first, the data structure and the protocol for web-controller communication is mainly designed by the web developer, and the controller that deals with web requests is implemented according to the data structure. In other words, this part is directed by the data structure that is designed based on the client requirements.

As the data structure may change during the web development and it is absolutely important to use a confirmed and determined data structure to direct the controller's implementation, there is a chronological order in the development of this part of our software -- web development first and controller logics for web must be implemented after the web logics finished.

For the web side (front-end), the process is logic-oriented. HTML pages and CSS styles are designed based on the built up JavaScript logics and communication protocol. 

For the server controller, it needs to negotiate data structure and communication rules with web side as well as the hardware, before determining how to design its database.

For the hardware (M5Stack and M5Stick), the communication between them and other elements in the system is relatively simple - publishing Bluetooth addresses and receiving simple commands (open or close). Which means the hardware implementation is rather independent from the software's. It is the communication between M5Stack and M5Stick that is challenging and thus the main focus of the hardware part. Hence, we will introduce the sprints of hardware and software implementation separately.

<a name="software"></a>

### Software timeline

<a name="sprint1"></a>

#### Sprint 1: Initial design

___Objectives___

* Initial sketch of desktop app UI
* A rough JSON data structure satisfying all the requirements
* Simple web pages for running JavaScript  implemented with HTML and CSS

<a name="sprint1front"></a>

##### Desktop front end

We completed the preliminary design of the entire system prototype between February 24 and February 28. We determined the way MQTT communicated with each component and determined the default form of the data structure within the entire system. Of course, this data form is quite different from the final data structure used, but this prototype has formalized our engineering direction. Based on determining the system prototype and data structure, we completed the front-end UI sketch design for the desktop app between February 28 and March 5.

<a name="sprint1back"></a>

##### Desktop back end

The first part is to design the JSON file format according to the needs of the product. Three Json structures with data type "m5_transmit", "m5_receive", and "parking" are designed to receive data, send data, and store parking records. Because the JSON format is actually the most important part, we must first determine the system requirements with the M5Stack part, and then unify the format
```
{
    "data_type": "m5_transmit",
    "barrier_info":{
        "barrier_id": 12345,
        "barrier_type": "in"
    },
    "bluetooth_devices":
        [{
            "bluetooth_address": "47:a9:af:d2:63:cd",
            "RSSI": -80
        }]
}
```
```
{
	"data_type": "m5_receive",
	"barrier_id": 12345,
	"op_code": "A"
}
```
```
{
			"data_type":"parking",
			"info": {
				"barrier_type": "out",
				"time_in": "2020-3-20-15-4-21",
				"time_out": "2020-3-20-18-4-21",
				"username": "lea",
				"barrier_id": 12345,
				"vehicle_id": "acdjcidjd",
				"vehicle_type":"car"
				}
}
```

<a name="sprint1web"></a>

##### Web

In this sprint the web developer designed the initial version of the data structure used for communication with controller, where 5 necessary data types are included. And we implemented the logics for login and register page.

`web_finance`, `web_recharge`, `web_login`, and `web_register` have the same name as the final version, while `web_vehicle`was split into 2 parts later. This first version was just for displayed because they are not well organized and unified.

Initial version of the web data types

```json
{
	"data_type": "web_register",  
	"login_info": {
			"username": "lea_tong",
			"password": "*******",
            "userid":"aaaaa"
		}
}

{

	"data_type": "web_login", 
	"login_info": {
			"username": "lea_tong",
			"password": "*******",
            "access":0
		}
}

{

	"data_type": "web_vehicle",   
	"vehicle_info": {
            "user_id":"aaaaa",
			"vehicle_id": "acdjcidjd",
			"vehicle_type":"car"
		}
}
{

	"data_type": "web_finance",  
	"finance_info": {
            "user_id":"aaaaa",
			"balance": 21331,
			"currency":"GBP"
		}
}

{

	"data_type": "web_recharge",  
	"recharge_info": {
            "user_id":"aaaaa",
			"balance": 21331,
			"currency":"GBP",
            "card_number":"326173173718",
            "pay_amount":"10"
		}
}
```

In this sprint only the very simple HTML and CSS pages for login and registration were implemented, as the developer was not quite familiar with the Bootstrap and JavaScript yet.

<a name="sprint2"></a>

#### Sprint 2: Initial back-end development   
___Objectives___

* Negotiate and confirm final data structure for web
* Stable connection established via MQTT
* Module test for the functionality of web stand-alone system
* Server subscribes to, retrieve, parse, store and publish user and vehicle registration information from MQTT and complete the interaction with the web site.
* Server subscribes to, retrieve, parse, store and publish vehicle access information from MQTT and complete the interaction with the M2Stack end.
* Server requests for access and information about users are logically processed and used to modify and save information about new users and required to achieve dynamic refresh.
* Server receives messages from M5Stack.
* Server provides all the data needed to be captured, thus making a back-end API for the software.

##### Web
In this sprint we finally confirmed all data structure for web-controller communication, established the MQTT connection, and did module test for web front side.

For data schema, The changes in [data structure](../../data_structure.json) are quite significant.

Since all of message are transmitted at one topic "PIMS", so each side needs a mechanism to grab package belongs to it. At beginning, we believe just using data type is enough to identify. However, when we test it , we find that if subscribing a topic , a sender will also receive the message sent from itself.  Since each request needs two message, one query message from sender and one confirmed message from receiver, and in communication between web and server, each request's two messages have same data type, both server side and web side grab the package that should not belong to them. So, we need another identifier, which is status to identify whether it should grab. we rule that status =2 must be the message from web to server, and status = 1 and status =0 must be message from server to web, where 1 means query succeeds and 0 means query fails.  Such mechanism successfully solved above problem.

In addition, we added a data type `web_vehicle_history` for fetching the parking history and split the type `web_vehicle` into `web_vehicle_query` and `web_vehicle_register` to fetch vehicle list of the user and to register a new vehicle for the user.

The module test was carried out to see whether functions dealing with coming data can perform properly and correctly. The approach was by publishing messages in broker and observing the response on web pages. By the end of this sprint, we had tested for each data type and confirmed that can work in the stand-alone system of web application.
##### Desktop back end
We transmit messages through the MQTT protocol. Both the M5Stack part and the DeskTop part subscribe to the "PIMS" topic. The goal we need to accomplish is to receive the Json data from the M5Stack that stores the Bluetooth address and signal strength, and give feedback based on different data. First determine whether there is a registered car in Json. If there is, get the current car status from the database. Second, determine whether the car is inside or outside the parking lot. If the pole can be opened outside the parking lot, the pole can be opened inside the parking lot. According to the result of the second step, send a message to open or close the rod to M5Stack. After opening the lever, the system pauses for five seconds and then closes the pole. At the same time, the parking information of this car is generated or updated.



#### Sprint 3: Development on front-end and further development on back-end 
___Objectives___
* The style sheets and dynamic rendering of front-end web pages
* Complete the functionality for in server controller side, including updating
customers' account balance.
* Decide which data types need to be stored in database and which do not.
* Electronic sketches for desktop app

##### Web and desktop controller
The controller logics for web was built in this sprint as the data structure for this part had already been confirmed. We also implemented the CSS style sheets for front-end web pages and tested the communication between web and the controller.

Since the functionalities in web side are almost done, we start controller's development. In this sprint, we finished all functionalities in server's controller, except vehicle history query. 

More specifically, the controller was now able to update the balance in customers' account. When the car enters the parking garage, record the time of entry. When the car leaves the parking garage, the time of departure needs to be recorded. According to the difference between the time of entering and leaving, the user's parking cost is calculated, and the account balance is obtained from the database and updated.


And meanwhile we decided which of data objects need to be stored into file system of database. 

*Evaluation and testing*:

All functionalities developed so far work correctly when connecting web with desktop.


##### Desktop app

Since the previous design was only a paper operation, the first step we should complete the paper design to electronic prototyping excess. The difficulty with this step is that some of the components conceived initially on paper are inherently tricky to electronic. In addition to this, the electronic design script adds a colour element that allows the use of the desktop app to be perceived through colour changes.

###### Layout simple elements to the desktop UI

It is extremely dangerous to fully develop the front-end UI directly before the desktop app back-end is developed. So we take the approach of first developing the UI part of the desktop app that does not involve the underlying logic API.

* The first part we complete is the construction of the title and text section, which only relates to the colours and fonts. It is worth mentioning that we have chosen the font Berlin Sans FB Regular, which is superior to the system default font in terms of display effect.
* Next, we finished laying out the buttons and menus, but did not write the part that interacted with the API, just did some design and tweaking of the page layout based on the art.
* Once again, we have implemented the clock tab at the top of the page. Since this project is highly correlated with time, it was essential to design a clock at the top of the desktop app to display time. While implementing the clock tag, we completed the build of the desktop app refresh mechanism. This is a significant step for follow-up.
* Finally, we finished building the colour pattern. Since this button is a pure display function independent of the system architecture, it is also effortless to implement.

###### Building elements that interact with back-end APIs

When the desktop app backend API was almost done, we started developing the display part that had logical interaction with the backend API. We completed the construction of the following components in order.

* First, we completed the presentation of the vehicle's pass record. Here we call the M5Stack pass log data stream of the backend API. In chronological order, the first entry is the latest pass record, and the display is refreshed in a previously constructed refresh mode.
* Next, we completed the information display of all vehicles present. The data is also updated in real-time by refreshing the system.
* Third, we completed the presentation of the data visualization components. Taking advantage of the PROCESSING data visualization, we effectively represented the ratio of the number of vehicles present to the total number of parking spaces and the real-time revenue from the parking lot. Due to the limited time available for the presentation of this project, we have set the time limit for the refresh mechanism at 100 seconds after group discussion.
* Finally, we have finished manually controlling the M5Stack button in the desktop app, which is done by calling the function in event. Control of the barrier is completed by packaging the JSON data to be sent.



#### Sprint 4: Review, modify and finalise

*Objectives*

* System tests in 3 stages
* Functionality of querying parking history in the controller
We conducted a total of three tests, a single-part effect demonstration, a joint test with the web end and a joint test with M5Stack. The performance and reliability of the system are refined with each test. Compared to the first bug-filled test, the third test was perfect, working smoothly with the other two sets of parts through the MQTT.
* Server receives messages sent from M5Stack and debug the program.

##### Web
In this sprint we carried out the system tests for 3 stages and fixed bugs. We did find a significant bug that could cause the system cashing in this sprint: while all three parts of our software running, the web page crashes because different types of data was wrongly accepted by web JavaScript logics in `client.onMessageArrived` in [account.js](../../web_application/user_account.js)

```javascript
    if (respondObj.info.username === username && respondObj.info.status === 1) {
        let message_type = respondObj.data_type;
        switch (message_type) {
            case 'web_finance': renderUserBalance(respondObj); break;
            case 'web_recharge': renderRecharge(respondObj); break;
            case 'web_vehicle_query': renderVehicleList(respondObj); break;
            case 'web_vehicle_register': renderVehicleRegister(); break;
            case 'web_vehicle_history': renderChart(respondObj); break;
            default: return;
        ...
```

In the code above we did not filter the data type at the entrance of the function, which will allow other data type entering the branch logics causing an access to the field of undefined (respondObj.info is undefined)

We changed the code to prevent other types entering this logic:

```javascript
    if (respondObj.data_type.startsWith('web')) {
        if (respondObj.info.username === username && respondObj.info.status === 1) {
            let message_type = respondObj.data_type;
            switch (message_type) {
                case 'web_finance': renderUserBalance(respondObj); break;
                case 'web_recharge': renderRecharge(respondObj); break;
                case 'web_vehicle_query': renderVehicleList(respondObj); break;
                case 'web_vehicle_register': renderVehicleRegister(); break;
                case 'web_vehicle_history': renderChart(respondObj); break;
                default: return;
            }
        }
        ...
```

##### Controller:

query vehicle parking history is the most complicated parts for controller development since controller need to traverse database to find the corresponding "parking" history and add the parking fee to corresponding day. After that, controller will return this message and web will plot the result  to a  table.

*test communication*:

This  functionality works correctly when connecting web with desktop.
##### Desktop app
In this sprint we finalised the charts and plots on desktop UI to make sure they are both easy to read and functioning smoothly.



<a name="_hardware"></a>

### Hardware timeline

<a name = "2aIoTST"> </a>

Sprint No.| Brief Description| Implementation
------------ | ------------- | -------------
1 | [Bluetooth Detector](#2aIoTSprint1) | [See here](#2aIoTSprint1Imp) 
2 | [MQTT Publisher/Listener](#2aIoTSprint2) | [See here](#2aIoTSprint2Imp)
3 | [Barrier Simulator](#2aIoTSprint3) | [See here](#2aIoTSprint3Imp)
4.| [Message Packer/Unpacker](#2aIoTSprint4) | [See here](#2aIoTSprint4Imp) 
5 | [Keys](#2aIoTSprint5) | [See here](#2aIoTSprint5Imp)

<a name = "2aIoTSprint1"> </a>
#### Sprint 1: Bluetooth Detector
 The first thing any of our barriers needs to do is to is scanning all the Bluetooth (BT) devices in its vicinity regularly and obtaining useful information from them. This enables the barriers to record their ambient changes, which permits our system to recognise the surroundings of each barrier and thus facilitates decision making for the barriers by our server. 
<a name = "2aIoTSprint1Imp"> </a>
##### :white_circle:Implementation
(__NOTE__: _code in this section is for explanation only, so it may not match that in our source code exactly._) <br><br>
Each M5Stack came with an integrated dual-mode Bluetooth, with which we could develop our BT detector. <br><br>
We incorporated the ["BLEDevice"](https://github.com/nkolban/ESP32_BLE_Arduino) library to control the BT module on M5Stack since this library was included in the Arduino IDE and was easy to use: <br>
``` c++
#include <BLEDevice.h>
```

<br>Then, the singleton class "BLEDevice" was initialised with a name for the device, and the BLEScan object was acquired. 
```c++
/* Global scope: */
#define BLE_DEVICE_NAME "barrier001" // BLE name of this device
BLEScan *pBLEScan; // pointer for the BLEScan object, which will be acquired later
```
```c++
/* In Arduino setup(): */
BLEDevice::init(BLE_DEVICE_NAME);
pBLEScan = BLEDevice::getScan();
```
<br> Then, we could easily obtain the information of the BT devices nearby by performing a scan, which could be done by calling the BLEScan::start method: <br>
```c++
BLEScanResults start(uint32_t duration, bool is_continue = false);
```
We called this method in the Arduino loop with a predefined duration and passed the scan results to a result handler function. In this way, an automated regular scan was achieved:
```c++
/* Global scope: */
#define BLE_SCAN_DURATION 5 // duration in seconds for which a session of scan lasts
void handleScanResult(BLEScanResults results); // signature of the result handler function
```
```c++
/* In Arduino loop(): */
handleScanResult(pBLEScan->start(BLE_SCAN_DURATION));
```
<br> Then, the number of BT devices that were detected and the information about each of the devices could be obtained:
```c++
int deviceCount = results.getCount(); // get the number of BT devices detected
```
```c++
BLEAdvertisedDevice BLEad = results.getDevice(i); // where i is the index for a BT device. ( 0 <= i < deviceCount );
```
<br> Then, the BT address and the RSSI for each BT device could be acquired:
```c++
std::string btAddress = BLEad.getAddress().toString(); // get the BT address
int rssi = BLEad.getRSSI(); // get the RSSI
```
<br>So far, we had obtained all the information we needed from the environment.<br>
##### :white_medium_square:Some questions you may ask...
*Q. Why did you set the BLE scan duration to 5 seconds, not 20s or 2s ?* <br>
A. The duration for each scan to last could be configured to any value. The shorter the duration, the quicker the barriers detect ambient changes, and therefore the more responsive our system will be, which means better user experience. However, shorter refresh cycles put more burden on our system. Since we were using MQTT for communication and moreover a single topic for all devices, it was not sensible to make the barriers publishing messages too quickly because every message on the topic would be picked up by the barriers including those sent by themselves, which would exhaust the limited processing power of them. Therefore, the scan duration was set to 5 seconds, which we thought was a reasonable value.
<br><br>
[Go back to the sprint table](#2aIoTST) 
<br><br><br>
<a name = "2aIoTSprint2"> </a>
#### Sprint 2: MQTT Publisher/Listener
 So far, our barriers could collect information from the environment, but they hadn't sent it to our server. Moreover, they must have some way to receive commands from our server, otherwise, they would be nothing more than pieces of iron bars.<br>
 We had chosen MQTT as our protocol for the communication between different parts of our system, but firstly, the barriers had to connect to the Internet!

<a name = "2aIoTSprint2Imp"> </a>
##### :white_circle:Implementation
Each M5Stack had an integrated 802.11 b/g/n HT40 Wi-Fi transceiver, with which it could surf the Internet when it felt boring.<br>
However, the actual device that enjoyed the luxury of surfing the Internet in one of our barriers was an Arduino MKR WiFi 1010 board.<br>
<a name = "2aIoT_m5memory"></a>
The reason was that the program storage space on an M5Stack was roughly 1.3 MB, which was less than enough for our entire program. At some point in our development period, we found it impossible to cram more functionalities into an M5Stack without rewriting some libraries. So we adopted an easier approach ---- splitting tasks of an M5Stack into two parts run by two devices. The functionalities that got separated from M5Stack were WiFi connection and MQTT publication/subscription. Nevertheless, we kept this part of code compatible with M5Stacks so when we overstocked M5Stacks <del>(That's never gonna happen)</del> we could construct a barrier with two M5Stacks. <br><br>

We used the WiFi library readily available in Arduino IDE, which was:
```c++
#include <WiFi.h> // For M5Stack
```
```c++
#include <WiFiNINA.h> // For Arduino
```

<br> Setting up WiFi was pretty straightforward:
```c++
/* Global scope: */
// Wifi settings
char wifi_ssid[] = "wifi_ssid";   
char wifi_password[] = "wifi_password";   
```
```c++
/* WiFi setup function */
void setupWifi(){
  Serial1.println("Connecting to wifi.");
  do{
    WiFi.begin(wifi_ssid, wifi_password);
    const int waitSectionCount = 10;
    for(int i=0; i<waitSectionCount; i++){
      if(WiFi.status() == WL_CONNECTED){
        break;
      }
      Serial1.println("unable to connect to wifi. Still trying...");
      delay(500);
    }   
  }while(WiFi.status() != WL_CONNECTED);

  Serial1.println("wifi connected.");
}
```
The above "setupWifi" function keeps trying to connect to the WiFi AP with the SSID and password defined. It requests to connect to the AP via calling WiFi.begin and checks the connection status every 500 milliseconds for 10 times. If it is still not connected to the AP after (10 + 1) times, it makes a connection request again. <br>
In the future, we will add the functionality to set WiFi SSID and password via USB connection to computers since hard-coded SSDI and password are neither user-friendly nor secure.<br><br>

After connected to WiFi, the barrier needed to connect to our chosen MQTT server and subscribe to our topic. <br>
We adopted this library that was available in Arduino IDE as well to achieve these tasks:
```c++
#include <PubSubClient.h>
```
We will quickly cover our usage of this library. If you have any questions regarding the methods of this library, please refer to [their official document](https://pubsubclient.knolleary.net/).<br>
One thing worth mentioning is that this library had a limit of the maximum packet size for publishing messages of 128 bytes at the time we wrote this report, which was too small for our packets. To resolve this, we changed a macro in the library header file:
```c++
#define MQTT_MAX_PACKET_SIZE 8192
```
8192 bytes could accommodate information of 90 BT devices for us (calculated with [ArduinoJson Assistent](https://arduinojson.org/v6/assistant/)), which we thought would be enough.

MQTT setup:
```c++
/* Global scope: */
WiFiClient wifi_client; // came with WiFi libraries
#include <PubSubClient.h>
PubSubClient ps_client( wifi_client );
// MQTT Settings
const char* server = "broker.mqttdashboard.com";
const int port = 1883;
const char* MQTT_clientname = "barrier"; 
const char* MQTT_sub_topic = "PIMS"; // topic to subscribe
const char* MQTT_pub_topic = "PIMS"; // topic to publish
void callback(char* topic, byte* payload, unsigned int length); // callback function signature
```
```c++
/* called witnin Arduino setup() */
// MQTT setup
ps_client.setServer(server, port);
ps_client.setCallback(callback);
```
The PubSubClient::setCallback method sets a callback function which is called when:
* PubSubClient::loop() is called and 
* the device is connected to the server and 
* the subscribed topic has new message available.

In order to continually receive message from the MQTT server, PubSubClient::loop should be called inside the Arduino loop():
```c++
/* In Arduino loop(): */
ps_client.loop();
```
<br>In order to publish message to the MQTT server, PubSubClient::publish shoule be called:
```c++
ps_client.publish( MQTT_pub_topic, msg); // where msg is const char* for this overloaded method
```
<br>

The purpose of the Arduino WiFi 1010 board was relaying messages for the main M5Stack, so it ought to be connected to the M5Stack.
| ![M5StackSerialPins](IoTDevices/M5StackSerialPins.jpg) | ![1010SerialPins](IoTDevices/1010SerialPins.jpg) |
|--|--|

As the image illustrates, the two devices were connected via Serial ports. The WiFi 1010 board was powered by the M5Stack and its 14th (TX of Serial1) and 13th pins (RX of Serial1) were connected to M5Stack's R0 and T0, respectively.
We chose 115200 as the Serial baud rate. The reason was that the higher the baud rate, the faster the data transfer speed and this value was one of the standard ones that were supported by both of the devices. Although data error is more likely in high baud rates, this is mainly affected by the capacitance of the cable. In our cases, our devices were connected by cables shorter than 20 centimetres and we had not experienced severe data loss, so we thought this baud rate was sensible in our situation:
```c++
/* In Arduino setup(): */
Serial.begin(115200);
```
<br>Since there might be some auxiliary messages sent from WiFi 1010 board to M5Stack (such as WiFi connection status) or from M5Stack to Serial port (for debugging), we distinguished data messages from others by a delimiter: 
```c++
#define SERIAL_JSON_DELIMITER '#'
```
<br> Every message that contains data (in our case, a JSON object) was prefixed and suffixed with the above delimiter before transferring between WiFi 1010 and M5Stack. The recipient (either M5Stack or the WiFi 1010) would first peek the first character on receival of the message to recognise the type of the message (data or others):
```c++
/* For M5Stack: */
// handles serial input when available
void handleSerialInput(){
  if(!Serial.available()){return;}
  int8_t firstByte = Serial.peek();
  switch(firstByte){
    case SERIAL_JSON_DELIMITER:
      handleJsonSerialInput();
      break;
    default:
      handleNormalSerialInput();
  }
}
```
<br> For M5Stack, if the incoming message contained data, the delimiters would be discarded and the message would be sent to JSON deserialiser. On the otherhand, if the message was not data, it would be printed to the screen of the M5Stack.
```c++
/* For WiFi 1010: */
void publishFromSerial1(){
  int byteCount = Serial1.available();
  if(byteCount <=0){return;}
  Serial1.find(SERIAL_JSON_DELIMITER);
  String serial1Read = Serial1.readStringUntil(SERIAL_JSON_DELIMITER);
  
  if(ps_client.connected()){
    Serial.println(serial1Read); // prints the message to Serial (USB) for debugging
    publishMessage(serial1Read); // publishes the message to the topic on the MQTT server
  }else{
    Serial1.println("Can't publish message: Not connected to MQTT :( "); // prints error message to M5Stack
    Serial.println("Can't publish message: Not connected to MQTT :( "); // prints error message to Serial (USB)
  }
}
```
<br> For WiFi 1010, if the delimeters were found, they were discarded before the message was sent to the MQTT server. If the first character was not the delimeter, the whole message was ignored since the recipient of the message was the computer (if existed) that connected to the M5Stack via USB because M5Stack's USB connection also used the Serial port.

<br>The WiFi 1010 kept listening to its Serial1 port and the MQTT server. If a message arrived from one end, it relayed it to the other.
Meanwhile, the M5Stack kept listening to its Serial port. While the M5Stack would handle incoming messages from its Serial port if available, it also sent all its messages to the Serial port. This is the way the M5Stack communicated with the MQTT server.
<br>
Although doing similar things, the approaches were a bit different for the WiFi 1010 board and the M5Stack.<br>
For the Wifi 1010 board, its tasks were quite simple. So we let them run on a single thread:
```c++
/* in Arduino loop(): */
  ps_client.loop(); // listens to the MQTT topic
  publishFromSerial1(); // listens to Serial1
```
<br>Relaying messages would not take too much time, so blocking was not severe for the WiFi 1010 board.<br>
For the M5Stack, however, it could not have reacted to Serial inputs in time while it was performing a five-second BT scan had we only used a single thread. 
Therefore, we employed the multi-tasking API of M5Stack:
```c++
void startInputHandler(){
  xTaskCreatePinnedToCore(
                    listenToSerial,     /* Function to implement the task */
                    "listenToSerial",   /* Name of the task */
                    4096,      /* Stack size in words */
                    NULL,      /* Task input parameter */
                    1,         /* Priority of the task. The higher the more important */
                    NULL,      /* Task handle. */
                    0);        /* Core where the task should run */
}

void listenToSerial(void *pvParameters){
  for(;;){
    handleSerialInput();
    delay(20); // random short value, which we found was suitable
  }
}
```
<br> The BT Scan was performed in the main thread, so it would not block the Serial input handler anymore.
<br><br>
##### :white_medium_square:Some questions you may ask...
*Q. Why did you choose the Arduino MKR WIFI 1010 board?* <br>
A. It was easy to migrate a sketch that had been written for M5Stack to the Arduino platform, and vice versa. Any Arduino board with WIFI and Serial ports should do. Or you could use another M5Stack if you wish. But I would argue that an MKR WIFI 1010 board costs less energy and (more importantly) money than an M5Stack.
<br><br>
*Q. My stock of M5Stacks is abundant! How can I run your code on two M5Stacks?* <br>
A. 
The [Arduino_HiveMQ_connector.ino](/Arduino_HiveMQ_connector/Arduino_HiveMQ_connector.ino) was written to be compatible with M5Stacks. <br>
Simply compile and upload it to an M5Stack, and do the same things for [M5Stack_bluetooth_detector.ino](/M5Stack_bluetooth_detector/M5Stack_bluetooth_detector.ino) using another M5Stack. <br>
Then, connect the two M5Stacks Serial to Serial. And you should be good to go.

NOTE: Do not forget to change the MQTT_MAX_PACKET_SIZE to something like 1024 or larger for the PubSubClient external library since the default value would be too small.
<br><br>

[Go back to the sprint table](#2aIoTST) 

<br><br><br>
<a name = "2aIoTSprint3"> </a>

#### Sprint 3: Barrier Simulator
Now that our barrier could collect ambient data and communicate with our server via MQTT, the core (or the processing) part of it was almost complete. However, it would have to reconsider its barrier life if it cannot do what ordinary barriers can such as lifting or descending its bar. <br>
We were at the prototyping phase of this project, <del> so having access to a real barrier was unimaginable! </del> so using a real barrier was unnecessary. <br>
Instead, we wrote a simulator that displayed the status of a real barrier (open/closed) on the screen of the M5Stack and made the buttons on the M5Stack the manual controllers of a real barrier. <br>
In fact, if we could get the barrier work in simulation, it should be quite easy to adapt the program to a real barrier using the analog input/output pins on the M5Stack.

<br><br>
<a name = "2aIoTSprint3Imp"> </a>
##### :white_circle:Implementation
The barrier simulator class was declared here:
[Barrier_simulator.h](/M5Stack_bluetooth_detector/Barrier_simulator.h)
This class was originally written to be a singleton since each M5Stack was only in charge of one barrier.
However, for demonstration purposes, we allowed multiple instances of this class to exist to simulate more barriers <del>because we only had one M5Stack</del>.<br>
We instantiated two barrier simulators in the M5Stack main program, representing a barrier at the entrance of a car park (inBarrier) and one at the exit of the same car park (outBarrier):
```c++
#include "Barrier_simulator.h"
BarrierSimulator inBarrier = BarrierSimulator({12345, BarrierType::IN});
BarrierSimulator outBarrier = BarrierSimulator({54321, BarrierType::OUT});
BarrierSimulator * volatile pCurrentBarrier = &inBarrier;
```
<br> The inBarrier and the outBarrier was assigned the id 12345 and 54321, respectively.
<br> "pCurrentBarrier" pointed to the barrier that the M5Stack was simulating at that time. Toggling of the barriers was assigned to the BtnB on M5Stack. For the same reason as the Serial input handler, the hardware interrupt handler was running in another thread, different from that of the Serial input hander and the main thread. Therefore, the "pCurrentBarrier" should be qualified "volatile". Otherwise, it might have been cached in some functions, which would have caused some problems like failing barrier id checks or sending the wrong barrier information to our server.<br>

To be consistent, remote control from our server and manual control via the buttons to the barriers were both achieved by sending an operation code to the barrier:
```c++
void BarrierSimulator::handleOpCode(const char* opCode);
```
<br> Only the first character of the opCode string was considered since hardly a barrier could perform 257 manoeuvres.<br>
In fact, we only considered two operations, to open (lift) and to close (descend), which we thought was enough for demonstration: [Barrier_orders.h](/M5Stack_bluetooth_detector/Barrier_orders.h).

Commands from our server to barriers were packed with barrier ids indicating the recipients of those commands. Only the barrier whose id matched that in a command message should execute that command. Hence id checks were performed before any opCode from our server were fed to the BarrierSimulator::handleOpCode methods of the barriers. This could be achieved by calling the BarrierSimulator::checkBarrierId method on the barriers.
```c++
bool BarrierSimulator::checkBarrierId(unsigned long idToCompare) const{
// returns true if and only if idToCompare equals the id of this barrier
  return m_barrierInfo.id == idToCompare;
}
```

<br><br><br>
<a name = "2aIoTSprint4"> </a>
#### Sprint 4: Message Packer/Unpacker
Human need languages to communicate with each other. Same for machines. A machine can only understand data in formats that it could interpret.<br>
[We had chosen JSON to be the data format of communication in our system](/Report/System_design/README.md#_e), so our barriers should have the ability to construct and parse JSON strings.


<br><br>
<a name = "2aIoTSprint4Imp"> </a>
##### :white_circle:Implementation
[ArduinoJson](https://arduinojson.org/) is an powerful, well-documented, open source JSON library for embeded c++. <br>
We used it for both constructing and parsing JSON strings.<br>
To use the library:
```c++
#include <ArduinoJson.h>
```
<br> __Build Outgoing JSON String__<br>
This library allows two ways of initialising a JSON document (either a JSON object or a JSON array): static allocation and dynamic allocation.<br>
In our case, the outgoing JSON objects of an M5Stack had the following format:
```JSON
{
    "data_type": "m5_transmit",
    "barrier_info":{
        "barrier_id": 12345,
        "barrier_type": "in"
    },
    "bluetooth_devices":
        [{
            "bluetooth_address": "47:a9:af:d2:63:cd",
            "RSSI": -80
        }]
}
```
<br> where "bluetooth_devices" was an array with an unfixed size. Moreover, if there were a large number of advertising BT devices near the barrier, the whole JSON object could be very large. Therefore, we allocated this type of JSON objects on the heap:
```c++
  const int jsonCapacity =  JSON_OBJECT_SIZE(3)   // root
                          + JSON_OBJECT_SIZE(2)   // barrier_info
                          + JSON_ARRAY_SIZE(deviceCount) + (deviceCount) * JSON_OBJECT_SIZE(2) // bluetooth_decives
                          + deviceCount * 41      // duplication of strings of keys
                          + 81;                  // fixed string duplication
  DynamicJsonDocument jDoc(jsonCapacity);
```
<br><a name = "2aIoT_forceCopy"> </a> The capacity for the JSON document was calculated with [ArduinoJson Assistent](https://arduinojson.org/v6/assistant/). <br>
It is worth noticing that the "ArduinoJson Assistent" assumes that all strings are copied to the JSON document. Acutally, [a string is copied only when it is of one of the following types: ](https://arduinojson.org/v6/api/jsonobject/subscript/#remarks)
```c++
char*
String (or std::string)
const __FlashStringHelper* (i.e. Flash string)
```
<br> So, the "ArduinoJson Assistent" computes the maximum rather than the real size of a JSON document. <br>
However, we just took the result from the Arduino Assistent directly because we had not reached the phase of optimisation. Had we preferred performance and resource utilisation over readibility and simplicity, we would have made the keys in the JSON objects much shorter. <br>

<br> A newly created JSON document was then populated with the BT scan result from the last scan, the information of the barrier, and other necessary information to complete a JSON object of type "m5_transmit": 
```c++
// build outgoing JsonDocument from BLEScanResults
// deviceCount : got from results.getCount().
void buildOutgoingJDoc(JsonDocument& jDoc, BLEScanResults& results, int deviceCount){
  jDoc["data_type"] = "m5_transmit";
  
  JsonObject barrierInfo = jDoc.createNestedObject("barrier_info");
  barrierInfo["barrier_id"] = pCurrentBarrier->getBarrierId();
  barrierInfo["barrier_type"] = pCurrentBarrier->getBarrierType();
  
  JsonArray bthDevices = jDoc.createNestedArray("bluetooth_devices");
  for(int i=0; i<deviceCount; i++){
    BLEAdvertisedDevice BLEad = results.getDevice(i);

    JsonObject bthDeviceInfo = bthDevices.createNestedObject();
    if(bthDeviceInfo == NULL){
      PRINTLN_WHEN_DEBUG("allocation of a JsonObject failed.");
      return;
    }
    bthDeviceInfo["bluetooth_address"] = (char *)BLEad.getAddress().toString().c_str();
    bthDeviceInfo["RSSI"] = BLEad.getRSSI(); // returns a int
  }
}
```
<br>The assignments of the key value paris were pretty straightfoward.

Basically, use:
```c++
JsonDocument::createNestedArray();
JsonDocument::createNestedObject();
```
to create nested array and objects,<br>
and use :
```c++
JsonDocument::operator[]
```
to assign value to a key.

Some explanation about the line:
```c++
bthDeviceInfo["bluetooth_address"] = (char *)BLEad.getAddress().toString().c_str();
```
<br> The BLEad.getAddress().toString() returned a std::string, which was then converted to const char * and then cast to char*. <br>
The final cast to char* [forced a copy of the string to be stored in the JsonObject bthDeviceInfo](#2aIoT_forceCopy). The std::string type could also force the JsonObject to take a copy, so why converted to c_str() ? Well, acutally, the BLEAddress::toString method returned a std::__cxx11::basic_string<char>, which was differnt from std::string and the JsonObject::operator[] had no overloaded method that mathched that signature.

<br> After that, the JSON document was serialised, delimetered and printed to Serial:
```c++
// serialise JsonDocument and print it to serial
void printJDocToSerial(const JsonDocument& jDoc){
  Serial.print(SERIAL_JSON_DELIMITER);
  serializeJson(jDoc, Serial);
  Serial.print(SERIAL_JSON_DELIMITER);
}
```
<br><br><br>

<br> __Parse Incoming JSON String__<br>
An M5Stack was only concerned with one type of incoming JSON string ---- "M5_receive":
```JSON
{
    "data_type": "m5_receive",
    "barrier_id": 12345,
    "op_code": "A"
}
```
This JSON object had a fixed size and was not very large, which meant it could be allocated on the stack to boost performance.
The ArduinoJson library provides a filtering functionality for deserialisation, which ignores irrelevant fields and therefore saves space in the JsonDocument. In our cases, however, we were only concerned with one format of JSON object and the JsonDocument was initialised with a fixed size, which meant saving space was not necessary. Despite that, we employed a filter since it might boost performance by not copying irrelevant fields.<br>
The following function returns our filter:
```c++
const JsonDocument& getInputJsonDocFilter(){
  static StaticJsonDocument<receivedJDocCapacity> filter;
  static bool initialised = false;
  if(!initialised){
    filter["data_type"] = true;
    filter["barrier_id"] = true;
    filter["op_code"] = true;
    initialised = true;
  }
  return filter;
}
```
<br> The filter document was constructed by the StaticJsonDocument constructor because it had a fixed and small size and resolving its address at compile time spared a call to malloc, which was never a bad idea. The filter was qualified static since we only needed one filter and we did not want to initialise it every time we asked for it. 

<br> The main JsonDocument was then initialised:
```c++
StaticJsonDocument<receivedJDocCapacity> jDoc;
```
<br>with a capacity of
```c++
const int receivedJDocCapacity { 
                            JSON_OBJECT_SIZE(3) // root
                          + 42 };     // string copy
```
<br> Then the JsonDocument was passed to the deserialiser with the string received from MQTT and the filter:
```c++
// deserialise json string into JsonDocument
// returns 0 on success. 1 otherwise
int buildIncomingJdoc(JsonDocument& jDoc, const char* jsonString){
  DeserializationError dErr = 
    deserializeJson(jDoc, jsonString, DeserializationOption::Filter(getInputJsonDocFilter()));
  if(dErr != DeserializationError::Ok){
    handleJsonDeserializationErr(dErr);
    return 1;
  }
  return 0;
}
```
<br> If the deserialisation was successful, values could be retrieved by using the operator[] with keys on the JsonDocument.
Calling operator[] on JsonDocument& returns a JsonVariant, while on const JsonDocument& returns a JsonVariantConst. In our case, we used const reference to maintain data integrity:
```c++
// returns 0 if the id in json document matches that of this device.
//         1 otherwise.
int checkBarrierId(const JsonDocument& jDoc){
  JsonVariantConst value = jDoc["barrier_id"];
  if(value.isNull()){return 1;}

  unsigned long receivedId = value.as<unsigned long>();
  return pCurrentBarrier->checkBarrierId(receivedId) ? 0 : 1;
}

const char* getOpCode(const JsonDocument& jDoc){
  // check data type
  JsonVariantConst value = jDoc["data_type"];
  if(value.isNull()){return NULL;}
  String dataType = value.as<String>();
  if(!dataType.equals("m5_receive")){return NULL;}
  PRINTLN_WHEN_DEBUG(dataType.c_str());

  // check barrier id
  if(checkBarrierId(jDoc) != 0) {return NULL;}

  // get op code
  value = jDoc["op_code"];
  if(value.isNull()){return NULL;}

  return value.as<const char*>();
}
```
<br> We checked data_type, barrier_id and co_code in order. The op_code was returned if and only if all the checks passed. Then the opcode could be safely executed by the barrier.

<a name = "2aIoTSprint5"> </a>
#### Sprint 5: Keys
Now that the barriers are complete, it is time for developing keys for the users. Keys are identifications for registered users. When a user approaches one of our barriers with a valid key, the key should be detected by that barrier and our server should be aware of the presence of the key and its position to provide the auto lifting of the barrier <del> and the auto charging from the user's balance __( ideally from their bank account )__ </del> service for that user.<br>
<a name = "2aIoT_keysPowerLevel"></a>
Since we are using RSSI to recognise the relative distance from the key to the barrier, we should keep the power of the BT modules of our keys on the same level. This is why we have chosen to use a dedicated device (M5Stick-C) to be the key rather than mobile phones or BT speakers since the power of the BT modules of different M5Stick-Cs varies negligibly compared to different mobile phones. <br>
Currently, we have not introduced any security measure such as paring and asking for an authantication key, so the implementation is very simple: The button on the M5Stick-C is in charge of toggling the advertising state (ON/OFF) of its BT module. When the M5Stick-C is advertising, it is visible to our barriers. When its advertising state is switched off, it is not detectable. <br>
Actually, users could leave their keys on all the time without any compromise on their experience since the RSSI provides enough information for our server to figure out which client should serve (the one that has the highest RSSI). The switching functionality was developed only for energy-saving purposes.

<br><br>
<a name = "2aIoTSprint5Imp"> </a>

##### :white_circle:Implementation
Libraries used:
```c++
#include <M5StickC.h> // enables display and button control
#include "BLEDevice.h" // enables BT functionality
```
<br> BT init:
```c++
/* Global scope: */
#define BLE_DEVICE_NAME "M5StickC"
BLEAdvertising* pBLEAdvertising;
```
```c++
/* In Arduino setup(): */
BLEDevice::init(BLE_DEVICE_NAME);
pBLEAdvertising = BLEDevice::getAdvertising();
```
<br> Button controlled toggling of the BLE advertising state was facilitated by:
```c++
/* Global scope: */
enum M5BLEState {ON, OFF} m5BLEState { OFF }; // the advertising state

void handleButtonInterrupt(){ // toggle the state via a button press
  M5.update();
  if(M5.BtnA.wasPressed()){
    switchBLEAdvertisingState();
  }
}

void switchBLEAdvertisingState(){
  M5.Lcd.fillScreen(BLACK); // clears screen
  M5.Lcd.setCursor(0, 0); // move the cursor back to origin
  if(m5BLEState == OFF){ // if the advertising state is OFF 
    pBLEAdvertising->start();
    m5BLEState = ON;
    M5.Lcd.println("ON"); // display the state to the screen
  }else{ // if the advertising state is ON
    pBLEAdvertising->stop();
    m5BLEState = OFF;
    M5.Lcd.println("OFF");
  }
}
```
<br> 




[Go back to the sprint table](#2aIoTST) 
<br><br>


<a name="_b"></a>

## Technique justification and design evaluation
In this section, we will justify in details why we chose certain techniques and their pros and cons during evaluations.
### Web
**Bootstrap4 and jQuery**

*Why do we choose them?*

Bootstrap4 is very friendly for beginners of web development because it provides diverse useful HTML and CSS components. Furthermore, Bootstrap4 is perfect in making responsive pages, and as the users might want to view their account info and top in on their mobile device, the responsive design will guarantee that the pages perform well in different scenarios.

jQuery helps to simplify the DOM manipulation, which means that we can choose any elements on a web page more easily. In our situation, it is enough to use jQuery for this lightweight web application.

*Limitation*

There is a limitation that the versions below 10 of Internet Explorer do not support bootstrap 4 because they do not support flex layout. This will cause some unknown problems or program crashing when the users use an IE browser to access to this application. This could potentially be unfriendly to those who use an old browser. However, as we should encourage the users to upgrade their browsers to phase out the old browsers with bad performance and bad supporting to new features, we did not consider using  Bootstrap 3 instead of 4 in this application, although it will have better compatibility towards lower version of browsers.

Directly manipulating on DOM tree is regarded as bad practice in programming nowadays, and programmers are encouraged to use frameworks that hide the DOM manipulation like React and Vue in front-end development. However, for this light-weight application, it seems unnecessary to put the so heavy frameworks in use. But one thing worth considering is that if we want to extend this application to a wider use, it would be better to use a framework such as Vue or React.

**Echarts**

*Why do we choose this?*

Echarts enables data visualization and chart dynamic rendering with several easy function calls. This helps us to draw a chart to show the parking time of users.

*Limitations*

To initialize an echarts object we have to provide the constructor function a DOM block element with specified height and width. However, in our responsive web design, the container for the chart has a percentage as height and width, which is not supported by the Echarts. To solve this, we use jQuery to set the width and height of the element dynamically at the stage of page loading, and this can give the element fixed values of length and width according to the actual size of it. 

But this brings another problem: after the jQuery setting a fix value for any attribute, the element keeps the value when even if the page size has been changed by user. This will cause the chart out of edge if the user zoom in the page. To fix this, a solution is to listen the element size using jQuery, however, this will cause a bad performance of the JavaScript logics, and it is not worth implementing because the user can refresh the pages manually when they want to change the size of the web pages.

**Cookies**

*Why cookies are in need?*

In order to keep the user logged in, the status is often stored as cookie in practice of web development to prevent illegal access to the user account page.

*Limitation*

However, in this project there is actually no 'back-end' system, as a result of which, setting cookie and keeping sessions could be quite challenging. To implement a session system in the desktop application needs plenty of work, and will add to the number of types of data schema due to the nature of MQTT. Furthermore, as all data transport via broker is explicit and can be seen by anyone who subscribe a certain topic, **security** seems not a topic that is worth considering in this small project. As a result, we simply store the username in the cookie to simulate keeping the logged in status, even if we are aware that this could be very dangerous that the user could access any of the user's account by carrying out a fake cookie.

### Desktop back-end
*Why do we use these [communication rules](../System_design/README.md#_e)?*

1. This design of data schema is different from that recommended by the lecturers.
 10 types of data make each json object is small, and each message do not have redundant information since each data type message has its own specified purpose. The three-sides applications grab the message belongs to them by identify data type and status. Such design reduce the size of package and improve the efficiency of communicating.
2. Duplex way makes our communication robust and crash-free. Since in every request, when a sender sends query message, it expects a confirmed message from receiver. So, even though our package get lost on Internet, our application will not be crashed, and sender will request again for response.

*Why do we choose this [data scheme](../System_design/README.md#_f)?*

1. Each type of json object finish one specified functionality, and no redundant data in each json object.
2. 4 out of 10 data type needs to be stored in database, not all of them, which can save the space for hardware.

#### Details of method implementations
##### 1. Find the registered car from the Bluetooth address list
Our requirement is to sort from a series of Bluetooth addresses first according to the Bluetooth signal, and select the car with the strongest Bluetooth signal which is registered.
So we used the data structure ArrayList to store the class Device.
```
ArrayList<Device> list=new ArrayList<Device>();

private class Device {
  String address;
  int rssi;
  public Device(String address, int rssi)
  {
    this.address=address;
    this.rssi=rssi;
  }
}
```
Every time the device with the strongest signal is found, then by traversing the database to determine whether the car has been registered.
```
for (JSONObject message : db.messages)
      {
        if (message!=null&&message.getString("data_type").equals(MessageType.VEHICLE_REGISTER))
        {
          JSONObject info = message.getJSONObject("info");
          if (info.getString("bluetooth_address").equals(list.get(k).address))
          {
            flag=1;
            // flag=1 means find the target vehicle which is registered
            targetUser=info.getString("username");
            targetId=info.getString("vehicle_id");
            targetType=info.getString("vehicle_type");
            break;
          }
        }
      }
```
##### 2. Generate new parking records.
 Use Processing's year (), month (), day () and other APIs to record the parking time. A delay () API is also used here, which is used to simulate the time of car storage, which is fixed for 5 seconds. Unfortunately, the dealy () function is a single-threaded function that blocks the entire program. Therefore, multi-threading will be adopted in the future. Even if a certain thread delay (), it will not affect other threads.
```
if(barrier_type.equals("in"))
      {
      JSONObject json=new JSONObject();
      JSONObject newInfo=new JSONObject();
      json.setString("data_type", MessageType.PARKING);
      newInfo.setString("username", targetUser);
      String date=year()+"-"+month()+"-"+day()+"-"+hour()+"-"+minute()+"-"+second();
      newInfo.setString("time_in", date);
      newInfo.setString("time_out", null);
      newInfo.setInt("barrier_id", transmitMessage.getJSONObject("barrier_info").getInt("barrier_id"));
      newInfo.setString("vehicle_id", targetId);
      newInfo.setString("vehicle_type", targetType);
      newInfo.setString("barrier_type", "in");
      json.setJSONObject("info", newInfo);
      saveJSONObject(json, "data/"+json.getString("data_type") + "_"+ json.getJSONObject("info").getString("username") +"_"+ json.getJSONObject("info").getString("time_in")+".json");
      refreshData();
       barrierOpen(transmitMessage.getJSONObject("barrier_info").getInt("barrier_id"));
       //to allow cars in
      delay(5000);
      barrierClose(transmitMessage.getJSONObject("barrier_info").getInt("barrier_id"));
      
      }
```

##### 3. Calculate parking fees.
Here, we need to calculate the cost based on the parking time of the garage recorded in the date. Therefore, we need to use all the information such as year, month, day, hour, minute and second to calculate the cost according to the different time of parking. Because of the need for display, we set
the tariff to £1 per second, because the time for a testing is only in the scale of seconds and minutes, instead of hours and dats. 

In this way the revenue chart can more intuitively display the changes in parking 
fees.

```
int calcParkingFee(String time_in, String time_out)
{
  int tariff=1; // 1 pound per hour/ per second for presentation purpose
  String[] in_fee=time_in.split("-");
  String[] out_fee=time_out.split("-");
  /*Using seconds here because it's easy to demonstrate, it's not the real thing*/
  int seconds=int((float(out_fee[0])-float(in_fee[0]))*365*24*3600+(float(out_fee[1])-float(in_fee[1]))*30*24*3600+(float(out_fee[2])-float(in_fee[2]))*24*3600+
    (float(out_fee[3])-float(in_fee[3]))*3600+(float(out_fee[4])-float(in_fee[4]))*60+(float(out_fee[5])-float(in_fee[5])));
  //println("charge is "+(int)(seconds*tariff/(60*60)+tariff));
  return (int)(seconds*tariff); // Due to the limited time during presentation, we are charging by seconds.
}
```
##### 4. Account register

```java
  JSONObject receiveRegisterFromWeb(JSONObject message) {
    if (message == null) {
      return null;
    }
    JSONObject res = message;
    //file's name should have datatype identifier since it is useful for searching
    if (getObjWithUsername("web_register",                                        			message.getJSONObject("info").getString("username"))==null) {
      saveJSONObject(message, "data/"+message.getString("data_type") +"_"+      			message.getJSONObject("info").getString("username") + ".json");
      refreshData();
      res.getJSONObject("info").setInt("status", 1);  
      return res;
    } else {
      res.getJSONObject("info").setInt("status", 0);  
      return res;
    }
  }
```

when an user registers an account from web side, its register information will be stored into database, which can be used to check validation when user wants to login. the file is name by `"message.getString("data_type") +"_"+message.getJSONObject("info").getString("username") + ".json"` because it is read-friendly for manager to manually manage to databases. When connecting with web size, this functionality can work correctly.

##### 5 . Account login

```java
  JSONObject receiveLoginFromWeb(JSONObject loginMessage) {
    //String userId=loginMessage.getString("user_id");
    // use reg_info object to check whether password is correct
    String datatype="web_register";
    JSONObject infoFromWeb = loginMessage.getJSONObject("info");
    String username = infoFromWeb.getString("username");
    JSONObject obj= getObjWithUsername(datatype, username);
    if (obj!=null&&obj.getJSONObject("info").getString("password").equals(infoFromWeb.getString("password")))
    {
      //loginMessage.setInt("access",1);
      infoFromWeb.setInt("status", 1);
    } else {
      //loginMessage.setInt("access",0);
      infoFromWeb.setInt("status", 0);
    }

    return loginMessage;
  }
```

When controller receives the login message from web, controller will  traverse the database to find the matched username and check whether its password is matched.When connecting with web size, this functionality can work correctly.

##### 6. Check balance of an account 

```java
  JSONObject receiveFinanceFromWeb(JSONObject financeMessage) {
    if (financeMessage == null) {
      return null;
    }
    if (getObjWithUsername("web_finance", financeMessage.getJSONObject("info").getString("username"))==null) {
      JSONObject info = financeMessage.getJSONObject("info");
      info.setInt("balance", 0);
      info.setString("currency", "GBP");
      info.setInt("status", 1);
      saveJSONObject(financeMessage, "data/"+financeMessage.getString("data_type") + "_"+ financeMessage.getJSONObject("info").getString("username") + ".json");
      refreshData();
      return financeMessage;
    }
    JSONObject res = getObjWithUsername("web_finance", financeMessage.getJSONObject("info").getString("username"));
    res.getJSONObject("info").setInt("status", 1);

    return res;
  }
```

When a user register an account, it will automatically query for balance.  So, if it is the first time to query for balance, the controller will create a new finance entity to store user's balance. After that, every time, a user queries for balance, controller will  send back this entity to web side. note, this entity's balance amount can be changed when a user tops up. When connecting with web size, this functionality can work correctly.

##### 7. Topping up

```java
  JSONObject receiveRechargeFromWeb(JSONObject rechargeMessage) {
    JSONObject infoFromWeb = rechargeMessage.getJSONObject("info");
    String username = infoFromWeb.getString("username");
    JSONObject financeMessage= getObjWithUsername(MessageType.FINANCE, username);
    if (financeMessage==null) {
      rechargeMessage.getJSONObject("info").setInt("status", 0);
      return rechargeMessage;
    }

    int rechargeAmount = rechargeMessage.getJSONObject("info").getInt("pay_amount");
    int balance = financeMessage.getJSONObject("info").getInt("balance");
    balance = balance+ rechargeAmount;
    financeMessage.getJSONObject("info").setInt("balance", balance);

    String path = dataPath("")+"\\"+MessageType.FINANCE+"_"+financeMessage.getJSONObject("info").getString("username")+".json";
    deleteFile(path);
    saveJSONObject(financeMessage, "data/"+financeMessage.getString("data_type") + "_"+ financeMessage.getJSONObject("info").getString("username") + ".json");
    refreshData();
    rechargeMessage.getJSONObject("info").setInt("balance", balance);
    rechargeMessage.getJSONObject("info").setInt("status", 1);
    return rechargeMessage;
  }
```

When a user top up its parking credit, controller will retrieve the recharge amount from sender's message, and add it into user balance in "finance" account . After that, controller set the new balance to "recharge" message and sent it back to web side. When connecting with web size, this functionality can work correctly.





##### 8. Vehicle register

```java
  JSONObject receiveVehicleRegisterFromWeb(JSONObject message) {
    if (message == null) {
      return null;
    }
    JSONObject res = message;
    //file's name should have datatype identifier since it is useful for searching
    if (getObjFromDb("web_vehicle_register", "vehicle_id", message.getJSONObject("info").getString("vehicle_id"))==null) {
      saveJSONObject(message, "data/"+message.getString("data_type") +"_"+ message.getJSONObject("info").getString("username")+
        "_"+ message.getJSONObject("info").getString("vehicle_id") + ".json");
      refreshData();
      res.getJSONObject("info").setInt("status", 1);  
      return res;
    } else {
      res.getJSONObject("info").setInt("status", 0);  
      return res;
    }
  }
```

when user registers for a new car, this object will be stored into database. When connecting with web size, this functionality can work correctly.

##### 9. Query vehicle list

```java
  JSONObject receiveVehicleQueryFromWeb(JSONObject queryMessage) {

    String datatype="web_vehicle_register";
    JSONObject infoFromWeb = queryMessage.getJSONObject("info");
    String username = infoFromWeb.getString("username");


    JSONArray values = new JSONArray();
    int count = 0;
    for (JSONObject message : db.messages)
    {
      if (message!=null&&message.getString("data_type").equals(datatype)
        && message.getJSONObject("info").getString("username").equals(username)
        )
      {
        JSONObject vehicleInfo = new JSONObject();
        JSONObject infoFromDb = message.getJSONObject("info");
        String vehicle_id =  infoFromDb.getString("vehicle_id");
        String vehicle_type =  infoFromDb.getString("vehicle_type");
        vehicleInfo.setString("vehicle_id", vehicle_id );
        vehicleInfo.setString("vehicle_type", vehicle_type );
        values.setJSONObject(count, vehicleInfo);
        count++;
      }
    }
    infoFromWeb.setJSONArray("vehicle_list", values);
    infoFromWeb.setInt("status", 1);
    
    return queryMessage;
  }
```

This method is used to query car's information of an user, and one user can bind one or more cars for one account, so return message will include a jsonarray to contain car's information. If no car binded, the array will be empty. When connecting with web size, this functionality can work correctly.

##### 10. Query vehicle parking history

```java
  JSONObject receiveVehicleHistoryFromWeb(JSONObject historyMessage) {

    String datatype="parking";
    JSONObject infoFromWeb = historyMessage.getJSONObject("info");
    String username = infoFromWeb.getString("username");
    String vehicle_id = infoFromWeb.getString("vehicle_id");
    int currentYear = year();
    int currentMonth = month();
    int currentDay = day();
    int flag=0;
    int amount0=0;
    int amount1=0;
    int amount2=0;
    int amount3=0;
    int amount4=0;
    int amount5=0;
    int amount6=0;

    for (JSONObject message : db.messages)
    {
      if (message!=null&&message.getString("data_type").equals(datatype)
        && message.getJSONObject("info").getString("username").equals(username)
        && message.getJSONObject("info").getString("vehicle_id").equals(vehicle_id)
        )
      {
        JSONObject infoFromParking = message.getJSONObject("info");
        String time_in = infoFromParking.getString("time_in");
        String time_out = infoFromParking.getString("time_out");
        String[] in_array=time_in.split("-");
        String[] out_array=time_out.split("-");
        flag =1;
        if (currentYear!= Integer.valueOf(in_array[0])
          ||currentMonth!=Integer.valueOf(in_array[1])
          ||currentDay-Integer.valueOf(in_array[2])>6
          ) {
          continue;
        }
        int amount = currentDay-Integer.valueOf(in_array[2]);
        int seconds=int((float(out_array[0])-float(in_array[0]))*365*24*3600
        +(float(out_array[1])-float(in_array[1]))*30*24*3600
        +(float(out_array[2])-float(in_array[2]))*24*3600
        +(float(out_array[3])-float(in_array[3]))*3600
        +(float(out_array[4])-float(in_array[4]))*60
        +(float(out_array[5])-float(in_array[5])));
        println(""+seconds);
        switch(amount) {
        case 0:
          amount0 = amount0 + seconds;
          break;
        case 1:
          amount1 = amount1 + seconds;
          break;  
        case 2:
          amount2 = amount2 + seconds;
          break; 
        case 3:
          amount3 = amount3 + seconds;
          break;
        case 4:
          amount4 = amount4 + seconds;
          break; 
        case 5:
          amount5 = amount5 + seconds;
          break; 
        case 6:
          amount6 = amount6 + seconds;
          break; 
        default:
          break;
        }
      }
    }
      infoFromWeb.setInt("status", 1);
      infoFromWeb.setInt("0", amount0);
      infoFromWeb.setInt("1", amount1);
      infoFromWeb.setInt("2", amount2);
      infoFromWeb.setInt("3", amount3);
      infoFromWeb.setInt("4", amount4);
      infoFromWeb.setInt("5", amount5);
      infoFromWeb.setInt("6", amount6);
    return historyMessage;
  }
```

When a user wants to check last 7 days vehicle parking history,  controller will traverse database to find the corresponding "parking" history and add the parking fee to corresponding day. After that, controller will return this message and web will plot the result  to a  table. When connecting with web size, this functionality can work correctly.

#### Desktop front-end

Due to the independent and collaborative nature of desktop app front-end development, the evaluation of desktop app front-end is divided into the following two main aspects.

##### Evaluation and testing of separate components on the front end

This part of the component can run independently on the desktop app side without calling the underlying interface so that it can be tested separately.

*Clock*

![clock](./destop_App_Font-end/clock.png)

The clock text variable calls the system time, and success is marked by the ability to refresh the text with the refresh of the DRAW() function. In other words, the ability of the top clock label to display the system time properly is a successful evaluation criterion. The difficulty with this part is that replacing the clock tag text should be done in the DRAW() method, and not under our own constructed refresh mechanism. Moreover, it is using a content replacement approach rather than an overall replacement approach. This way, we can effectively avoid reporting errors.

*Colour Setting Menu*

The color shift menu is also a display component that does not involve the lower-level API, so the evaluation of the color shift can be done separately when the desktop app undergoes front-end development.The colour shift menu is also a display component that does not involve the underlying API, so the evaluation of the colour shift can be done separately when the desktop app undergoes front-end development. It is easy to understand that once the colour shift menu is used, the desktop application view can be changed from the original default dark colour mode to light colour mode. Of course, we can also go from light to dark mode.

This section is evaluated on the ability to successfully change the colour pattern of all components without reporting errors. Due to the peculiarities of PROCESSING, we have set the colour changing function in operation to control the successful use of this menu.We use a separate method to build the colour transform component. There is also no need to refresh using our own constructed refresh mechanism, but rather to refresh using the DRAW() function. The goal is to allow for faster system response and to avoid label redundancy due to duplicate component rendering.

##### Evaluation and testing of components on the front end that need to call the back end API

All components in this section must work with the API interface provided by the back-end in order to function correctly, so their testing must be done simultaneously with the back-end as well as the web and M5Stack ends in order to be adequately evaluated.

*New cars coming in*

When the barrier end receives a signal from an oncoming car, and the rear end successfully receives the incoming and outgoing logs and saves them on the local hard drive, the display end calls up all JSONArray to pick, sort, reorganize and display. Its evaluation of success is based on the front-end's ability to display the record correctly once the file has been received and saved by the back-end.

We require to arrange backwards, so the difficulty lies in how to sort JSONArray for specific fields. This, of course, falls under the category of algorithmic issues that are no longer discussed in this project. Unfortunately, due to the limitations of controlP5, it is difficult to build an excel-like table to display data details. We can only do our best to recreate the style of the table through the drop down menu. This is something we did not anticipate during the design session.

*Details of cars in the parking lot*

Similar to the above features, this component shows the remaining vehicles in the field. Therefore, it is also necessary to receive the queue of vehicles in the field and select the traffic record as in for display. The following highlights are the criteria for the successful evaluation of this component.

* Once a vehicle enters the parking lot, this component should refresh the record of that vehicle for a limited time and display it on the display end.
* Once a vehicle leaves the parking lot, this component should refresh to delete this vehicle record for a limited time.

In this component, the disadvantages of ControlP5 are even more pronounced. We had a hard time unifying the table for each menu list due to the different name lengths. To do this, we need to make a lot of less cost-effective function choices. So we didn't do that.


*Pie chart*

Data visualization is an advantage of Processing, and we have developed data visualization widgets using this feature. The pie chart shows the real-time occupancy of the parking spaces. For the purpose of displaying the occupancy clearly, since we will only have 1 car in the lot at a time (because we only have 1 M5Stick), the total number spaces in the parking lot is set to 10. 

Similar to the two lists above, the pie chart needs to be linked to a remote M5Stack. Two requirements need to be met for their successful evaluation.

* When a vehicle enters at the barrier end and is successfully recorded by the desktop application backend, the pie chart automatically refreshes the record to increase the percentage of vehicles present.
* When a vehicle leaves the barrier end and is successfully recorded by the desktop application backend, the pie chart automatically refreshes the record, reducing the percentage of vehicles present.


*Line chart*

Similar to the above components, the line chart shows the parking lot of revenue for the day, with hourly interval on the x-axis. The chart should refresh every hour to reflect the revenue trend for the past hours of the day.

However, because the testing time only lasts for less than 1 hour, the unit of x-axis is modified to seconds and the refresh frequency is also changed to every second.

Then, because the limitation of the space where the line chart is at, we can only fit 100 seconds in the chart, which is a simple, memorable number. After 100 seconds, the line chart will no longer refresh and reflect the real-time revenue but all other charts of the UI are not affected.

The following criteria can judge its success: when the barrier end detects the vehicle leaving the parking lot, the desktop program backend monitors the MQTT stream and stores the file to the local hard drive. The front-end calls the API interface for receivable calculations, which are finally reflected in a line chart that is 
dynamically refreshed in real-time.

The limitation of this design is that during testing, we need to ensure the activities all happen within 100 seconds when line chart is still refreshing. After that we need to manually re-start the desktop app.

*Barrier control buttons*

This widget is the one that communicates directly with M5Stack, so the way he works is to call the underlying API event method to control the barrier by sending a specific data type to MQTT for M5Stack to receive.

The criteria for evaluating the success of this component can be judged in this way. We open the desktop app and M5Stack at the same time, click on the button of this component on the desktop, and the screen of M5Stack shows the barrier state when the control is complete by command. If the transformation is successful, it is evaluated as successful. The way in which the button is used to implement the functionality of this component is very appropriate and meets the expectations of modern humans for manipulating electronic devices. This is the same design as our previous one.

### IoT

<table id = "2bIoTTb">


  <tr>
    <th>Technique</th>
    <th>pros / Resons of choice</th>
    <th>cons / Limitations</th>
  </tr>

  <tr>
    <td>Making M5Stack the controller of a barrier</td>
    <td><ul>
    <li> M5Stacks are great, versatile prototyping tools with various built-in functionalities such as BlueTooth, WiFi, and analog/digital input/output </li>
    <li> The processor of an M5Stack is a 240 MHz dual-core Tensilica LX6 microcontroller, which is fast enough to perform our tasks. </li>
    <li> An M5Stack has a 320x240 Colorful TFT LCD and 3 programmable buttons, which makes it ideal for simulating a real barrier.</li>
    </ul></td>
    <td><ul>
    <li><a href = "/Report/System_implementation/README.md/#2aIoT_m5memory">The memory for storing programs on an M5Stack is limited, which means a large program may have to be split into parts run by multiple devices that communicate with each other.</a></li>
    <li>The compilation time for an M5Stack is much longer than a normal Arduino board<del>, which is very unfriendly to newbie programmers like me who will never run out of bugs to fix.</del></li>
    </ul></td>
  </tr>

  <tr>
    <td>Using M5Stick-Cs as keys</td>
    <td><ul>
    <li><a href = "/Report/System_implementation/README.md/#2aIoT_keysPowerLevel">Determining distance via RSSI is only valid when the power of the BT modules of all the keys are on the same level. </a></li>
    <li>Each M5Stick-C has a display and a button just resembling hardware bank tokens.</li>
    <li>Its size is small enough to be a key.</li>
    </ul></td>
    <td><ul>
    <li>Its battery may not sustain its power consumption for a very long time.</li>
    <li>Its manufacturing cost may be too high for mass production.</li>
    </ul></td>
  </tr>

  <tr>
  <td>Using BT address as identifications</td>
  <td><ul>
  <li>BT addresses are designed to be unique and are a bit harder to fabricate compared to other types of ids such as MAC addresses.</li>
  <li>Generally, BlueTooth consumes less energy than WiFi. For IoT devices, Bluetooth Low Energy (BLE) could be utilised to save more energy.</li>
  <li>BlueTooth is designed to facilitate data transfer between devices over short distances, which suits our situation pretty well: short-range communication between keys and barriers.</li>
  </ul></td>
  <td>
  <li>There are existing technologies that consume even less energy such as Radio-frequency identification (RFID). If we do not add more security measures such as making the barriers paring the keys and ask for more authentication keys thereafter, our BLE approach may not outperform RFID except that it is easier for the developer to implement<del>, which will never be the concern for project managers.</del></li>
  </td>
  </tr>
</table>

<a name="_c"></a>

## Social and Ethical implications

#### Data Security, privacy and data protection
Due to the time limitation on this project, the data are currently stored on local hard disks of the manager who uses the desktop application. Because the desktop application is acting as a server, all the data from users will be processed by the desktop. This means that users' personal information is not kept confidential at all at this stage. In the future, after deploying a more comprehensive database, the users' personal data will not be easily visible to the managers. In turn, the data should be completely protected and shield
from any visualisation of the employees.

#### Social benefits
With this technology, the time of paying before leaving a parking lot is saved. More car can park in the lots during busy hours as the cars in the lots can leave quicker. For the users, they can also easily review and keep track of their parking records.

#### Environmental impact
Less emission from cars queuing with their engine on to exit the parking lot. Less usage of physical currencies at the transactions are payment machines. Less public facilities to fix: without this system, there will be multiple payment machines dotted around the parking lot and built inside the exit barriers, with this system, there will be no payment machines in the parking lot not at the exit. The only publicly used facility now will be the M5Stack that controls the barriers.

#### Employment
The elimination of public facilities results in less maintenance requirements thus potentially less employment. However, the higher demand of the keys (M5Sticks) will induce more employments for the mass production.
