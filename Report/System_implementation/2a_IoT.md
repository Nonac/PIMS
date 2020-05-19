# IOT Devices
<a name = "2aIoTSprintTable"> </a>
Sprint No.| Brief Description| Implementation
------------ | ------------- | -------------
1 | [Bluetooth Detector](#2aIoTSprint1) | [See here](#2aIoTSprint1Imp) 
2 | [MQTT Publisher/Listener](#2aIoTSprint2) | [See here](#2aIoTSprint2Imp)
3 | [Barrier Simulator](#2aIoTSprint3) | [See here](#2aIoTSprint3Imp)
4.| [Message Packer/Unpacker](#2aIoTSprint4) | [See here](#2aIoTSprint4Imp) 
5 | [Keys](#2aIoTSprint5) | [See here](#2aIoTSprint5Imp)

<a name = "2aIoTSprint1"> </a>
## Bluetooth Detector
 The first thing any of our barrier needs to do is to is scanning all the Bluetooth (BT) devices in its vicinity regularly and obtaining useful information from them. This enables the barriers to record their ambient changes, which permits our system to recognise the surroundings of each barrier and thus facilitates decision making for the barriers by our server. 
<a name = "2aIoTSprint1Imp"> </a>
### :white_circle:Implementation
(__NOTE__: _code in this section is for explanation only, so it may not match that in our source code exactly._) <br><br>
Each M5Stack came with an integrated dual mode Bluetooth, with which we could develop our BT detector. <br><br>
We incorporated the ["BLEDevice"](https://github.com/nkolban/ESP32_BLE_Arduino) library to control the BT module on M5Stack since this library was included in the Arduino IDE and was easy to use: <br>
``` c++
#include "BLEDevice.h"
``` 

<br>Then, the singleton class "BLEDevice" was initialised with a name for the device and the BLEScan object was acuired. 
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
We called this mehod in the Arduino loop with a predefined duration and passed the scan results to a result handler function. In this way, automated regular scan was achieved:
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

[Go back to the sprint table](#2aIoTSprintTable) 
<br><br><br>
<a name = "2aIoTSprint2"> </a>
## MQTT Publisher/Listener
 So far, our barriers had the ability to collect information from the environment, but they hadn't sent it to our server. Moreover, they must have some way to receive commands from our server, otherwise they would be nothing more than pieces of iron bars.<br>
 We had chosen MQTT as our protocal for the communication between different parts in our system, but firstly, the barriers had to connect to the Internet!
 
<a name = "2aIoTSprint2Imp"> </a>
### :white_circle:Implementation
Each M5Stack had an integrated 802.11 b/g/n HT40 Wi-Fi transceiver, with which it could surf the Internet when it felt bored.<br>
However, the actual device that enjoyed the luxry of surfing the Internet in one of our barriers was an Arduino MKR WiFi 1010 board. The reason was that the program storage space on an M5Stack was roughly 1.3 MB, which was less than enough for our entire program. At some point in our development period, we found it impossible to cram more functionalities into an M5Stack without rewriting some libraries. So we adopted an easier approach (probably)---- spliting tasks of an M5Stack into two parts run by two devices. The functionalities that got separated from M5Stack were WiFi connection and MQTT publication/subscription. Nevertheless, we kept this part of code compatible with M5Stacks so when we overstocked M5Stacks <del>(That's never gonna happen)</del> we could construct a barrier with two M5Stacks. <br><br>

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
In the future we will add the functionality to set WiFi SSID and password via USB connection to computers since hard-coded SSDI and password are neither user-friendly nor secure.<br><br>

After connected to WiFi, the barrier needed to connect to our chosen MQTT server and subscribe our topic. <br>
We adopted this library that was available in Arduino IDE as well to achieve these tasks:
```c++
#include <PubSubClient.h>
```
We will quickly cover our usage of this library. If you have any questions regarding the methods of this library, please refer to [their official document](https://pubsubclient.knolleary.net/).<br>
One thing worth mentioning is that this library had a limit of the maximum packet size for publising messages of 128 bytes at the time we wrote this report, which was too small for our packets. To resolve this, we changed a macro in the library header file:
```c++
#define MQTT_MAX_PACKET_SIZE 8192
```
8192 bytes could accomodate information of 90 BT devices for us (calculated with [ArduinoJson Assistent](https://arduinojson.org/v6/assistant/)), which we thought would be enough.

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
```
```c++
/* called witnin Arduino setup() */
// MQTT setup
ps_client.setServer(server, port);
ps_client.setCallback(callback);
```

