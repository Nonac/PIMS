#include <M5StickC.h>
#include "BLEDevice.h"

#define BLE_DEVICE_NAME "M5StickC"
BLEAdvertising* pBLEAdvertising;

enum M5BLEState {ON, OFF} m5BLEState { OFF };


// send our own BLE address to serial
void sendBLEAddrToSerial(){
    static const std::string BLEaddr = BLEDevice::getAddress().toString();
    Serial.println(BLEaddr.c_str());
}

inline void handleButtonInterrupt(){
  M5.update();
  if(M5.BtnA.wasPressed()){
    switchBLEAdvertisingState();
  }
}

void switchBLEAdvertisingState(){
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  if(m5BLEState == OFF){
    pBLEAdvertising->start();
    m5BLEState = ON;
    M5.Lcd.println("ON");
  }else{
    pBLEAdvertising->stop();
    m5BLEState = OFF;
    M5.Lcd.println("OFF");
  }
  sendBLEAddrToSerial();
}

void setup() {
  M5.begin();
  Serial.begin(115200);
  BLEDevice::init(BLE_DEVICE_NAME);

  pBLEAdvertising = BLEDevice::getAdvertising();
  M5.Lcd.setTextSize(5);
  switchBLEAdvertisingState();
}

void loop() {
  handleButtonInterrupt();
  delay(300);
}
