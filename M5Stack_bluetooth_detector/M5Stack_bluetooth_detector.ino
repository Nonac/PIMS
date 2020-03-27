#include <M5Stack.h>
#include "BLEDevice.h"
#include <ArduinoJson.h>

/*
 * device specific 
 */
#define BLE_DEVICE_NAME "BLEScanner001" // BLE name of this device
#define BARRIER_ID 12345 // id of this barrier


#define BLE_SCAN_DURATION 5 // duration in seconds for which a secion of scan lasts

BLEScan *pBLEScan;

void handleScanResult(BLEScanResults results){
  M5.Lcd.clear(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.println("scan finished");
  int deviceCount = results.getCount();
  M5.Lcd.print("There are ");
  M5.Lcd.print(deviceCount);
  M5.Lcd.println(" devices in the vicinity:");

  const int jsonCapacity = JSON_ARRAY_SIZE(deviceCount) + deviceCount * JSON_OBJECT_SIZE(2) // bluetooth_decives
                          + JSON_OBJECT_SIZE(1)   // barrier_info
                          + JSON_OBJECT_SIZE(3);  // root
  DynamicJsonDocument jDoc(jsonCapacity);
  
  jDoc["data_type"] = "m5_transmit";
  
  JsonObject barrierInfo = jDoc.createNestedObject("barrier_info");
  barrierInfo["barrier_id"] = BARRIER_ID;
  
  JsonArray bthDevices = jDoc.createNestedArray("bluetooth_devices");
  for(int i=0; i<deviceCount; i++){
    BLEAdvertisedDevice BLEad = results.getDevice(i);
    JsonObject bthDeviceInfo = bthDevices.createNestedObject();
    bthDeviceInfo["bluetooth_address"] = BLEad.getAddress().toString().c_str();
    bthDeviceInfo["RSSI"] = BLEad.getRSSI();
  }
  serializeJson(jDoc, Serial);
}

void handleButtonInterrupt(){
  M5.update();
  if(M5.BtnC.isPressed()){
    M5.powerOFF();
  }
}

// Arduino setup
void setup() {
  M5.begin();
  M5.Lcd.println("BLE client initialising...");
  BLEDevice::init(BLE_DEVICE_NAME);
  pBLEScan = BLEDevice::getScan();

  Serial.begin(9600);
} 


// Arduino main loop
void loop() {
  handleScanResult(pBLEScan->start(BLE_SCAN_DURATION));
  handleButtonInterrupt();
  delay(1000);

} 
