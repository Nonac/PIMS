#include <M5Stack.h>
#include "BLEDevice.h"
#include <ArduinoJson.h>

/* simple barrier simulator for m5Stack */
#include "Barrier_simulator.h"
BarrierSimulator& barrierSimulator = BarrierSimulator::getBarrierSimulator();

/* device specific identifications */
#define BLE_DEVICE_NAME "barrier001" // BLE name of this device
#define BARRIER_ID 12345 // id of this barrier

/* BLE config */
#define BLE_SCAN_DURATION 5 // duration in seconds for which a secion of scan lasts
BLEScan *pBLEScan;

/* serial communication config */
#define SERIAL_JSON_DELIMITER '#' // signify a json file is being sent
#define SERIAL_TIMEOUT 300
#define SERIAL_NORMAL_INPUT_SIZE 50

/*  state of M5Stack
 *   
 *  DEBUG: show log and error messages
 *  BARRIER: hand over the control of the screen to BarrierSimulator class 
 */
enum M5State {DEBUG, BARRIER} m5State = DEBUG;
#define PRINT_WHEN_DEBUG(p) if(m5State == M5State::DEBUG){M5.Lcd.print(p);}
#define PRINTLN_WHEN_DEBUG(p) if(m5State == M5State::DEBUG){M5.Lcd.println(p);}

/* Json config */
// incoming json file size. (JsonDocument has a fixed size)
// RECALCULATE if the format of json file changes.
const int receivedJDocCapacity { 
                            JSON_OBJECT_SIZE(2) // root
                          + 63 };     // string copy


/* multi-tasking entries */
// Continuously checks serial input.
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

void listenToSerial(void *pvParameters){
  for(;;){
    handleSerialInput();
    delay(SERIAL_TIMEOUT);
  }
}

// Continuously checks and handles button interrupts
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





/* support functions */


/* Group: serial input handler */
// handles serial input when available
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

// handles serial input that is not enclosed in SERIAL_JSON_DELIMITER
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
  PRINTLN_WHEN_DEBUG(byteBuffer);
}

// process Json string from serial
void handleJsonSerialInput(){
  Serial.read(); // discard the left delimiter
  String inStr = Serial.readStringUntil(SERIAL_JSON_DELIMITER);
  // cast to const char* so the Json deserialiser copies the contents of the string
  const char* inCStr = (const char *) inStr.c_str();

  DynamicJsonDocument jDoc(receivedJDocCapacity);
  buildIncomingJdoc(jDoc, inCStr);

  const char* opCode = getOpCode(jDoc);
  if(opCode != NULL){
    handleOpCode(opCode);
  }
}

// deserialise json string into JsonDocument
void buildIncomingJdoc(JsonDocument& jDoc, const char* jsonString){
  DeserializationError dErr = 
    deserializeJson(jDoc, jsonString, DeserializationOption::Filter(getInputJsonDocFilter()));
  if(dErr != DeserializationError::Ok){
    handleJsonDeserializationErr(dErr);
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

// handles error occurred during deserialisation of incoming json string
void handleJsonDeserializationErr(const DeserializationError& err){
  if(err == DeserializationError::NoMemory){
    PRINTLN_WHEN_DEBUG("receival err: json doc too small for deserialisation");
  }else{
    PRINT_WHEN_DEBUG("receival err: ");
    PRINTLN_WHEN_DEBUG(err.c_str());
  }
}

// find operation code from the JsonDocument
// returns: the operation code if found.
//          NULL otherwise.
const char* getOpCode(JsonDocument& jDoc){
  JsonVariant value = jDoc["data_type"];
  if(value.isNull()){return NULL;}
  
  String dataType = value.as<String>();
  if(!dataType.equals("m5_receive")){return NULL;}
  PRINTLN_WHEN_DEBUG(dataType.c_str());
  
  value = jDoc["op_code"];
  if(value.isNull()){return NULL;}

  return value.as<const char*>();
}

// process the opCode and then pass it to BarrierSimulator class
// although there is nothing being done here right now
inline void handleOpCode(const char* opCode){
  barrierSimulator.handleOpCode(opCode);
}




/* Group: BLE detector */
void handleScanResult(BLEScanResults results){
  int deviceCount = getBTDeviceCount(results);

  // RECALCULATE if the format of json file changes
  const int outgoingJDocCapacity {  
                            JSON_OBJECT_SIZE(3)   // root
                          + JSON_OBJECT_SIZE(1)   // barrier_info
                          + JSON_ARRAY_SIZE(deviceCount) + (deviceCount) * JSON_OBJECT_SIZE(2) // bluetooth_decives
                          + deviceCount * 55      // duplication of strings of keys
                          + 128};                 // fixed string duplication
  DynamicJsonDocument jDoc(outgoingJDocCapacity);
  
  buildOutgoingJDoc(jDoc, results, deviceCount);
  sendJDocToSerial(jDoc);
}

// returns the number of bluetooth devices in the vicinity
inline int getBTDeviceCount(BLEScanResults& results){
  PRINTLN_WHEN_DEBUG("scan finished");
  int deviceCount = results.getCount();
  PRINT_WHEN_DEBUG("There are ");
  PRINT_WHEN_DEBUG(deviceCount);
  PRINTLN_WHEN_DEBUG(" devices in the vicinity:");
  return deviceCount;
}

// build outgoing JsonDocument from BLEScanResults
// deviceCount : number of BT devices in results
void buildOutgoingJDoc(JsonDocument& jDoc, BLEScanResults& results, int deviceCount){
  jDoc["data_type"] = "m5_transmit";
  
  JsonObject barrierInfo = jDoc.createNestedObject("barrier_info");
  barrierInfo["barrier_id"] = BARRIER_ID;
  
  JsonArray bthDevices = jDoc.createNestedArray("bluetooth_devices");
  for(int i=0; i<deviceCount; i++){
    BLEAdvertisedDevice BLEad = results.getDevice(i);
    std::string addressStr = BLEad.getAddress().toString();

    JsonObject bthDeviceInfo = bthDevices.createNestedObject();
    if(bthDeviceInfo == NULL){
      PRINTLN_WHEN_DEBUG("allocation of a JsonObject failed.");
    }
    // cast to const char* to force a copy of the string
    // otherwise there will be a scope issue
    bthDeviceInfo["bluetooth_address"] = (const char*)addressStr.c_str();
    bthDeviceInfo["RSSI"] = BLEad.getRSSI(); // returns a int
  }
}

// serialise JsonDocument and send to serial port
inline void sendJDocToSerial(JsonDocument& jDoc){
  Serial.print(SERIAL_JSON_DELIMITER);
  serializeJson(jDoc, Serial);
  Serial.print(SERIAL_JSON_DELIMITER);
}



/* Group: M5Stack control */
// designate interrupt handler according to the current state of M5Stack
inline void handleButtonInterrupt(){
  if(m5State == M5State::DEBUG){
    handleButtonInterruptDebug();
  }else if(m5State == M5State::BARRIER){
    handleButtonInterruptBarrier();
  }
}

// interrupt handler on debug mode
// Actions:
// press BtnB: switch off M5Stack
// press BtnA: switch to barrier simulation mode
void handleButtonInterruptDebug(){
  M5.update();
  if(M5.BtnB.isPressed()){
    M5.powerOFF();
  }else if(M5.BtnA.isPressed()){
    m5State = M5State::BARRIER; // switch to barrier simulator mode
    barrierSimulator.setDisplay(true);
  }
}

// interrupt handler on barrier simulation mode
// Actions:
// press BtnA: open barrier
// press BtnB: close barrier
// press BtnC: switch to debug mode
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

// If text has gone beyond the frame of the screen,
// clears the screen and moves cursor back to origin.
void clearScreen(){
  if( M5.Lcd.getCursorY() > M5.Lcd.height() ) {
    M5.Lcd.clear(BLACK);
    M5.Lcd.setCursor(0, 0);
  }
}

/* Arduino setup */
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


/* Arduino main loop */
void loop() {
  if(m5State == M5State::DEBUG){
    clearScreen();
  }
  handleScanResult(pBLEScan->start(BLE_SCAN_DURATION));
} 
