# IOT Devices
<a name = "2aIoTST"> </a>
Sprint No.| Brief Description| Implementation
------------ | ------------- | -------------
1 | [Bluetooth Detector](#2aIoTSprint1) | [See here](#2aIoTSprint1Imp) 
2 | [MQTT Publisher/Listener](#2aIoTSprint2) | [See here](#2aIoTSprint2Imp)
3 | [Barrier Simulator](#2aIoTSprint3) | [See here](#2aIoTSprint3Imp)
4.| [Message Packer/Unpacker](#2aIoTSprint4) | [See here](#2aIoTSprint4Imp) 
5 | [Keys](#2aIoTSprint5) | [See here](#2aIoTSprint5Imp)

<a name = "2aIoTSprint1"> </a>
## Bluetooth Detector
 The first thing any of our barriers needs to do is to is scanning all the Bluetooth (BT) devices in its vicinity regularly and obtaining useful information from them. This enables the barriers to record their ambient changes, which permits our system to recognise the surroundings of each barrier and thus facilitates decision making for the barriers by our server. 
<a name = "2aIoTSprint1Imp"> </a>
### :white_circle:Implementation
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
### :white_medium_square:Some questions you may ask...
*Q. Why did you set the BLE scan duration to 5 seconds, not 20s or 2s ?* <br>
A. The duration for each scan to last could be configured to any value. The shorter the duration, the quicker the barriers detect ambient changes, and therefore the more responsive our system will be, which means better user experience. However, shorter refresh cycles put more burden on our system. Since we were using MQTT for communication and moreover a single topic for all devices, it was not sensible to make the barriers publishing messages too quickly because every message on the topic would be picked up by the barriers including those sent by themselves, which would exhaust the limited processing power of them. Therefore, the scan duration was set to 5 seconds, which we thought was a reasonable value.
<br><br>
[Go back to the sprint table](#2aIoTST) 
<br><br><br>
<a name = "2aIoTSprint2"> </a>
## MQTT Publisher/Listener
 So far, our barriers had the ability to collect information from the environment, but they hadn't sent it to our server. Moreover, they must have some way to receive commands from our server, otherwise, they would be nothing more than pieces of iron bars.<br>
 We had chosen MQTT as our protocol for the communication between different parts in our system, but firstly, the barriers had to connect to the Internet!
 
<a name = "2aIoTSprint2Imp"> </a>
### :white_circle:Implementation
Each M5Stack had an integrated 802.11 b/g/n HT40 Wi-Fi transceiver, with which it could surf the Internet when it felt boring.<br>
However, the actual device that enjoyed the luxury of surfing the Internet in one of our barriers was an Arduino MKR WiFi 1010 board. The reason was that the program storage space on an M5Stack was roughly 1.3 MB, which was less than enough for our entire program. At some point in our development period, we found it impossible to cram more functionalities into an M5Stack without rewriting some libraries. So we adopted an easier approach ---- splitting tasks of an M5Stack into two parts run by two devices. The functionalities that got separated from M5Stack were WiFi connection and MQTT publication/subscription. Nevertheless, we kept this part of code compatible with M5Stacks so when we overstocked M5Stacks <del>(That's never gonna happen)</del> we could construct a barrier with two M5Stacks. <br><br>

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
The above "setupWifi" function keeps trying to connect to the WiFi AP with the SSID and password defined. It requests to connect to the AP via calling WiFi.begin and checks the connection status every 500 mili-seconds for 10 times. If it is still not connected to the AP after (10 + 1) times, it makes a connection request again. <br>
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
### :white_medium_square:Some questions you may ask...
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
## Barrier Simulator
Now that our barrier could collect ambient data and communicate with our server via MQTT, the core (or the processing) part of it was almost complete. However, it would have to reconsider its barrier life if it cannot do what ordinary barriers can such as lifting or descending its bar. <br>
We were at the prototyping phase of this project, <del> so having access to a real barrier was unimaginable! </del> so using a real barrier was unnecessary. <br>
Instead, we wrote a simulator that displayed the status of a real barrier (open/closed) on the screen of the M5Stack and made the buttons on the M5Stack the manual controllers of a real barrier. <br>
In fact, if we could get the barrier work in simulation, it should be quite easy to adapt the program to a real barrier using the analog input/output pins on the M5Stack.

<br><br>
<a name = "2aIoTSprint3Imp"> </a>
### :white_circle:Implementation
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

To be consitent, remote control from our server and manual control via the buttons to the barriers were both achieved by sending an operation code to the barrier:
```c++
void BarrierSimulator::handleOpCode(const char* opCode);
```
<br> Only the first character of the opCode string was considered since hardly a barrier could perform 257 manoeuvres.<br>
In fact, we only considered two operations, to open (lift) and to close (descend), which we thought was enough for demonstration: [Barrier_orders.h](/M5Stack_bluetooth_detector/Barrier_orders.h).

Commands from our server to barriers were packed with barrier ids indicating the recipients of those commands. Only the barrier whose id matched that in a command message should execute that command. Hence id checks were performed before any opCode from our server were fed  to the BarrierSimulator::handleOpCode methods of the barriers. This could be achieved by calling the BarrierSimulator::checkBarrierId method on the barriers.
```c++
bool BarrierSimulator::checkBarrierId(unsigned long idToCompare) const{
// returns true if and only if idToCompare equals the id of this barrier
  return m_barrierInfo.id == idToCompare;
}
```

<br><br><br>
<a name = "2aIoTSprint4"> </a>
## Message Packer/Unpacker
Human need languages to communicate with each other. Same for machines. A machine can only understand data in formats that it could interpret.<br>
[We had chosen JSON to be the data format of communication in our system](/Report/System_design/README.md#_e), so our barriers should have the ability to construct and parse JSON strings.


<br><br>
<a name = "2aIoTSprint4Imp"> </a>
### :white_circle:Implementation
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
## Keys


<br><br>
<a name = "2aIoTSprint5Imp"> </a>
### :white_circle:Implementation

[Go back to the sprint table](#2aIoTST) 
<br>__Under construction...__
