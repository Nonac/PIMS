#include <M5Stack.h>
#include "BLEDevice.h"
#include <ArduinoJson.h>

/*
 * device specific 
 */
#define BLE_DEVICE_NAME "BLEScanner001" // BLE name of this device
#define BARRIER_ID 12345 // id of this barrier

#define SERIAL_DELIMITER '#'

#define BLE_SCAN_DURATION 5 // duration in seconds for which a secion of scan lasts

BLEScan *pBLEScan;

// RECALCULATE if the format of json file changes
const int receivedJDocCapacity = JSON_OBJECT_SIZE(2) // root
                          + 63;     // string copy

void handleSerialInput(){
  if(!Serial.available()){return;}
  
  Serial1.find(SERIAL_DELIMITER);
  String inStr = Serial1.readStringUntil(SERIAL_DELIMITER);
  // cast to const char* so the Json deserialiser copies the contents of the string
  const char* inCStr = (const char *) inStr.c_str();


  DynamicJsonDocument jDoc(receivedJDocCapacity);
  DeserializationError dErr = 
    deserializeJson(jDoc, inCStr, DeserializationOption::Filter(getInputJsonDocFilter()));
  if(dErr != DeserializationError::Ok){
    handleJsonDeserializationErr(dErr);
    return;
  }

   
  
  
}

void handleJsonDeserializationErr(DeserializationError err){
  if(err == DeserializationError::NoMemory){
    M5.Lcd.println("receival err: json doc too small for deserialisation");
  }else{
    M5.Lcd.println("receival err:");
    M5.Lcd.println(err.c_str());
  }
}

// returns a ancillary json document which serves as a filter, 
// which filters out irrelevant fields 
const JsonDocument& getInputJsonDocFilter(){
  static StaticJsonDocument<receivedJDocCapacity> filter;
  static bool initialised = false;
  if(!initialised){
    filter["data_type"] = true;
    filter["op_code"] = true;
    initialised = true;
  }
  return filter;
}

void handleScanResult(BLEScanResults results){
  M5.Lcd.clear(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.println("scan finished");
  int deviceCount = results.getCount();
  M5.Lcd.print("There are ");
  M5.Lcd.print(deviceCount);
  M5.Lcd.println(" devices in the vicinity:");

  // RECALCULATE if the format of json file changes
  const int jsonCapacity =  JSON_OBJECT_SIZE(3)   // root
                          + JSON_OBJECT_SIZE(1)   // barrier_info
                          + JSON_ARRAY_SIZE(deviceCount) + (deviceCount) * JSON_OBJECT_SIZE(2) // bluetooth_decives
                          + deviceCount * 55      // duplication of strings of keys
                          + 128;                  // fixed string duplication
  DynamicJsonDocument jDoc(jsonCapacity);
  
  jDoc["data_type"] = "m5_transmit";
  
  JsonObject barrierInfo = jDoc.createNestedObject("barrier_info");
  barrierInfo["barrier_id"] = BARRIER_ID;
  
  JsonArray bthDevices = jDoc.createNestedArray("bluetooth_devices");
  // Stores references to dynamically allocated c-style strings in the following loop
  // so we can delete them later.
  // (There is a scope issue if we don't do it like this.)
  char **addresses {new char*[deviceCount] {}}; 
  for(int i=0; i<deviceCount; i++){
    BLEAdvertisedDevice BLEad = results.getDevice(i);
    std::string addressStr = BLEad.getAddress().toString();
    char *addressArr {new char[addressStr.size() + 1] {}};
    addressStr.copy(addressArr, addressStr.size() + 1);
    addresses[i] = addressArr;

    JsonObject bthDeviceInfo = bthDevices.createNestedObject();
    if(bthDeviceInfo == NULL){
      M5.Lcd.println("allocation of a JsonObject failed.");
    }
    bthDeviceInfo["bluetooth_address"] = addressArr;
    bthDeviceInfo["RSSI"] = BLEad.getRSSI(); // returns a int
  }

  Serial.print(SERIAL_DELIMITER);
  serializeJson(jDoc, Serial);
  Serial.flush();
  Serial.print(SERIAL_DELIMITER);

  for(int i=0; i<deviceCount; i++){
    delete[] addresses[i];
  }
  delete[] addresses;
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

  Serial.begin(115200);
} 


// Arduino main loop
void loop() {
  handleScanResult(pBLEScan->start(BLE_SCAN_DURATION));
  handleButtonInterrupt();
  handleSerialInput();
} 
