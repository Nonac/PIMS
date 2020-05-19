# IOT Devices
<a name = "2aIoTSprintTable"> </a>
Sprint No.| Brief Description| Implementation
------------ | ------------- | -------------
1 | [Bluetooth Detector](#2aIoTSprint1) | [See here](#2aIoTSprint1Imp) 
2 | MQTT Publisher | See here 
3 | Barrier Simulator | See here 
4 | Keys | See here 

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
// Global scope:
#define BLE_DEVICE_NAME "barrier001" // BLE name of this device
BLEScan *pBLEScan; // pointer for the BLEScan object, which will be acquired later
```
```c++
// In Arduino setup():
BLEDevice::init(BLE_DEVICE_NAME);
pBLEScan = BLEDevice::getScan();
```
<br> Then, we could easily obtain the information of the BT devices nearby by performing a scan, which could be done by calling the BLEScan::start method: <br>
```c++
BLEScanResults start(uint32_t duration, bool is_continue = false);
```
We called this mehod in the Arduino loop with a predefined duration and passed the scan results to a result handler function. In this way, automated regular scan was achieved:
```c++
// Global scope:
#define BLE_SCAN_DURATION 5 // duration in seconds for which a session of scan lasts
void handleScanResult(BLEScanResults results); // signature of the result handler function
```
```c++
// In Arduino loop():
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
[Go back to the sprint table](#2aIoTSprintTable) 
