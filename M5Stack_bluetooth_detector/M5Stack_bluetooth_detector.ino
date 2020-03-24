#include <M5Stack.h>
#include "BLEDevice.h"

#define BLE_DEVICE_NAME "BLEScanner001" // BLE name of this device
#define BLE_SCAN_DURATION 10 // duration in seconds for which a secion of scan lasts

BLEScan *pBLEScan;

void handleScanResult(BLEScanResults results){
  M5.Lcd.clear(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.println("scan finished");
  int deviceCount = results.getCount();
  M5.Lcd.print("There are ");
  M5.Lcd.print(deviceCount);
  M5.Lcd.println(" devices in the vicinity:");
  for(int i=0; i<deviceCount; i++){
    BLEAdvertisedDevice BLEad = results.getDevice(i);
    M5.Lcd.println(BLEad.toString().c_str());
    continue;
    M5.Lcd.println(BLEad.getServiceUUID().toString().c_str());
  }
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
} 


// Arduino main loop
void loop() {
  handleScanResult(pBLEScan->start(BLE_SCAN_DURATION));
  handleButtonInterrupt();
  delay(1000);

} 
