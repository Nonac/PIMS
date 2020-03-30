#include <M5Stack.h>
#include "BLEDevice.h"
#include <ArduinoJson.h>

#include "Barrier_simulator.h"
BarrierSimulator& barrierSimulator = BarrierSimulator::getBarrierSimulator();

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

// messages only show up on the screen in debug mode
enum M5State {DEBUG, BARRIER} m5State = DEBUG;
#define PRINT_WHEN_DEBUG(p) if(m5State == M5State::DEBUG){M5.Lcd.print(p);}
#define PRINTLN_WHEN_DEBUG(p) if(m5State == M5State::DEBUG){M5.Lcd.println(p);}

// RECALCULATE if the format of json file changes
const int receivedJDocCapacity = JSON_OBJECT_SIZE(2) // root
                          + 63;     // string copy


void startInputHandler(){
  xTaskCreatePinnedToCore(
                    listenToSerial,     /* Function to implement the task */
                    "listenToSerial",   /* Name of the task */
                    4096,      /* Stack size in words */
                    NULL,      /* Task input parameter */
                    2,         /* Priority of the task. The higher the more important */
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


void startInterruptHandler(){
   xTaskCreatePinnedToCore(
                    handleButtonInterrupts,     /* Function to implement the task */
                    "handleButtonInterrupts",   /* Name of the task */
                    1024,      /* Stack size in words */
                    NULL,      /* Task input parameter */
                    1,         /* Priority of the task */
                    NULL,      /* Task handle. */
                    0);        /* Core where the task should run */
}

void handleButtonInterrupts(void *pvParameters){
  for(;;){
    handleButtonInterrupt();
    delay(300);
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
      PRINT_WHEN_DEBUG(byteBuffer);
      index = 0;
    }
    byteBuffer[index++] = incomingByte;
  }
  byteBuffer[index] = '\0';
  index = 0;
  PRINTLN_WHEN_DEBUG(byteBuffer);
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
  PRINTLN_WHEN_DEBUG(dataType.c_str());
  
  value = jDoc["op_code"];
  if(value.isNull()){return;}

  const char* opCode = value.as<const char*>();
  handleOpCode(opCode);
}



void handleOpCode(const char* opCode){
  barrierSimulator.handleOpCode(opCode);
}

void handleJsonDeserializationErr(DeserializationError err){
  if(err == DeserializationError::NoMemory){
    PRINTLN_WHEN_DEBUG("receival err: json doc too small for deserialisation");
  }else{
    PRINT_WHEN_DEBUG("receival err: ");
    PRINTLN_WHEN_DEBUG(err.c_str());
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
  PRINTLN_WHEN_DEBUG("scan finished");
  int deviceCount = results.getCount();
  PRINT_WHEN_DEBUG("There are ");
  PRINT_WHEN_DEBUG(deviceCount);
  PRINTLN_WHEN_DEBUG(" devices in the vicinity:");

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
      PRINTLN_WHEN_DEBUG("allocation of a JsonObject failed.");
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
  if(m5State == M5State::DEBUG){
    handleButtonInterruptDebug();
  }else if(m5State == M5State::BARRIER){
    handleButtonInterruptBarrier();
  }

}

void handleButtonInterruptDebug(){
  M5.update();
  if(M5.BtnB.isPressed()){
    M5.powerOFF();
  }else if(M5.BtnA.isPressed()){
    m5State = M5State::BARRIER; // switch to barrier simulator mode
    barrierSimulator.setDisplay(true);
  }
}

void handleButtonInterruptBarrier(){
  M5.update();
  if(M5.BtnA.isPressed()){
    const char opCode[2] {BARRIER_ACTION_OPEN, '\0'};
    handleOpCode((const char*)opCode);
  }else if(M5.BtnB.isPressed()){
    const char opCode[2] {BARRIER_ACTION_CLOSE, '\0'};
    handleOpCode((const char*)opCode);
  }else if(M5.BtnC.isPressed()){
    m5State = M5State::DEBUG;  // switch back to debug mode
    barrierSimulator.setDisplay(false);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(WHITE, BLACK);
    M5.Lcd.clear(BLACK);
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
  PRINTLN_WHEN_DEBUG("BLE client initialising...");
  BLEDevice::init(BLE_DEVICE_NAME);
  pBLEScan = BLEDevice::getScan();

  Serial.begin(115200);
  Serial.setTimeout(SERIAL_TIMEOUT);
  startInputHandler();
  startInterruptHandler();
  m5State = M5State::BARRIER;
} 


// Arduino main loop
void loop() {
  if(m5State == M5State::DEBUG){
    clearScreen();
  }
  handleScanResult(pBLEScan->start(BLE_SCAN_DURATION));
} 
