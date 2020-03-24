#include <M5Stack.h>
#include "BLEDevice.h"

#define BLE_DEVICE_NAME "BLEScanner001" // BLE name of this device
#define BLE_SCAN_DURATION 5 // duration in seconds for which a secion of scan lasts

BLEScan *pBLEScan;

void handleScanResult(BLEScanResults results){
  M5.Lcd.println("scan finished");
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

} 
