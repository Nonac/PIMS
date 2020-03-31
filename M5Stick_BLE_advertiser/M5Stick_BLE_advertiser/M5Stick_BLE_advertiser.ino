#include "BLEDevice.h"
#define BtnPin 35
#define BLE_DEVICE_NAME "M5Stick" // BLE name of this device

BLEAdvertising* pBLEAdvertising;
void setup() {
  // put your setup code here, to run once:
  BLEDevice::init(BLE_DEVICE_NAME);
  pBLEAdvertising = BLEDevice::getAdvertising();
  pBLEAdvertising->start();
  Serial.begin(115200);


}

// send our own BLE address to serial
void sendBLEAddrToSerial(){
    static const std::string BLEaddr = BLEDevice::getAddress().toString();
    Serial.println(BLEaddr.c_str());
}

void loop() {
  // put your main code here, to run repeatedly:
  sendBLEAddrToSerial();
  delay(300);

}
