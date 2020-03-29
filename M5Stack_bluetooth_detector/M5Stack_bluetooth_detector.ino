#include <M5Stack.h>
#include "BLEDevice.h"
#include <ArduinoJson.h>

/*
 * device specific 
 */
#define BLE_DEVICE_NAME "BLEScanner001" // BLE name of this device
#define BARRIER_ID 12345 // id of this barrier

#define SERIAL_JSON_DELIMITER '#' // signify a json file is being sent
#define SERIAL_TIMEOUT 300
#define SERIAL_NORMAL_INPUT_SIZE 50

#define BLE_SCAN_DURATION 5 // duration in seconds for which a secion of scan lasts

BLEScan *pBLEScan;

// RECALCULATE if the format of json file changes
const int receivedJDocCapacity = JSON_OBJECT_SIZE(2) // root
                          + 63;     // string copy


void startInputHandler(){
  xTaskCreatePinnedToCore(
                    listenToSerial,     /* Function to implement the task */
                    "listenToSerial",   /* Name of the task */
                    4096,      /* Stack size in words */
                    NULL,      /* Task input parameter */
                    1,         /* Priority of the task */
                    NULL,      /* Task handle. */
                    0);        /* Core where the task should run */
}

// Thread entry
// Continuously checks serial input.
void listenToSerial(void *pvParameters){
  for(;;){
    handleSerialInput();
    delay(SERIAL_TIMEOUT);
  }
}

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

void handleNormalSerialInput(){
  static char byteBuffer[SERIAL_NORMAL_INPUT_SIZE + 1] {};
  int index = 0;
  int8_t incomingByte;
  while((incomingByte = Serial.read()) != -1){
    if(index == SERIAL_NORMAL_INPUT_SIZE - 1){
      M5.Lcd.print(byteBuffer);
      index = 0;
    }
    byteBuffer[index++] = incomingByte;
  }
  byteBuffer[index] = '\0';
  index = 0;
  M5.Lcd.println(byteBuffer);
}

void handleJsonSerialInput(){
  Serial.read(); // discard the left delimiter
  String inStr = Serial.readStringUntil(SERIAL_JSON_DELIMITER);
  // cast to const char* so the Json deserialiser copies the contents of the string
  const char* inCStr = (const char *) inStr.c_str();


  DynamicJsonDocument jDoc(receivedJDocCapacity);
  DeserializationError dErr = 
    deserializeJson(jDoc, inCStr, DeserializationOption::Filter(getInputJsonDocFilter()));
  if(dErr != DeserializationError::Ok){
    handleJsonDeserializationErr(dErr);
    return;
  }

  JsonVariant value = jDoc["data_type"];
  if(value.isNull()){return;}
  
  String dataType = value.as<String>();
  if(!dataType.equals("m5_receive")){return;}
  M5.Lcd.println(dataType.c_str());
  
  value = jDoc["op_code"];
  if(value.isNull()){return;}

  char opCode = value.as<char>();
  handleOpCode(opCode);
}



void handleOpCode(char opcode){
  M5.Lcd.println(opcode);
}

void handleJsonDeserializationErr(DeserializationError err){
  if(err == DeserializationError::NoMemory){
    M5.Lcd.println("receival err: json doc too small for deserialisation");
  }else{
    M5.Lcd.print("receival err: ");
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

  Serial.print(SERIAL_JSON_DELIMITER);
  serializeJson(jDoc, Serial);
  //Serial.flush();
  Serial.print(SERIAL_JSON_DELIMITER);

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

void clearScreen(){
  if( M5.Lcd.getCursorY() > M5.Lcd.height() ) {
    M5.Lcd.clear(BLACK);
    M5.Lcd.setCursor(0, 0);
  }
}

// Arduino setup
void setup() {
  M5.begin();
  M5.Lcd.println("BLE client initialising...");
  BLEDevice::init(BLE_DEVICE_NAME);
  pBLEScan = BLEDevice::getScan();

  Serial.begin(115200);
  Serial.setTimeout(SERIAL_TIMEOUT);
  startInputHandler();
} 


// Arduino main loop
void loop() {
  clearScreen();
  //handleSerialInput(); 

  handleScanResult(pBLEScan->start(BLE_SCAN_DURATION));
  handleButtonInterrupt();
} 
