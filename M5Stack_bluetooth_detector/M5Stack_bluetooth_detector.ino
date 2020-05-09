#include <M5Stack.h>
#include "BLEDevice.h"
#include <ArduinoJson.h>

/* simple barrier simulator for m5Stack */
#include "Barrier_simulator.h"
// simulating two types of barrier: in/out
BarrierSimulator inBarrier = BarrierSimulator({12345, BarrierType::IN});
BarrierSimulator outBarrier = BarrierSimulator({54321, BarrierType::OUT});
BarrierSimulator *pCurrentBarrier = &inBarrier;
// if the barrier is switched during scanning, the latest result should be abandoned
volatile bool g_isResultValid {true};

/* device specific identifications */
#define BLE_DEVICE_NAME "barrier001" // BLE name of this device


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
                            JSON_OBJECT_SIZE(3) // root
                          + 90 };     // string copy


/* multi-tasking entries */
// Continuously checks serial input.
void startInputHandler(){
  xTaskCreatePinnedToCore(
                    listenToSerial,     /* Function to implement the task */
                    "listenToSerial",   /* Name of the task */
                    4096,      /* Stack size in words */
                    NULL,      /* Task input parameter */
                    1,         /* Priority of the task. The higher the more important */
                    NULL,      /* Task handle. */
                    0);        /* Core where the task should run */
}

void listenToSerial(void *pvParameters){
  for(;;){
    handleSerialInput();
    //delay(SERIAL_TIMEOUT);
  }
}

// Continuously checks and handles button interrupts
void startInterruptHandler(){
   xTaskCreatePinnedToCore(
                    handleButtonInterrupts,     /* Function to implement the task */
                    "handleButtonInterrupts",   /* Name of the task */
                    1024,      /* Stack size in words */
                    NULL,      /* Task input parameter */
                    2,         /* Priority of the task */
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
  // return if building fails
  if(buildIncomingJdoc(jDoc, inCStr) != 0) {return;}

  // return if barrier id does not match that of this device
  if(checkBarrierId(jDoc) != 0) {return;}

  const char* opCode = getOpCode(jDoc);
  if(opCode != NULL){
    handleOpCode(opCode);
  }
}

// deserialise json string into JsonDocument
// returns 0 on success. 1 otherwise
int buildIncomingJdoc(JsonDocument& jDoc, const char* jsonString){
  DeserializationError dErr = 
    deserializeJson(jDoc, jsonString, DeserializationOption::Filter(getInputJsonDocFilter()));
  if(dErr != DeserializationError::Ok){
    handleJsonDeserializationErr(dErr);
    return 1;
  }
  return 0;
}

// returns a ancillary json document which serves as a filter, 
// which filters out irrelevant fields 
const JsonDocument& getInputJsonDocFilter(){
  static StaticJsonDocument<receivedJDocCapacity> filter;
  static bool initialised = false;
  if(!initialised){
    filter["data_type"] = true;
    filter["barrier_id"] = true;
    filter["op_code"] = true;
    initialised = true;
  }
  return filter;
}

// returns 0 if the id in json document mathes that of this device.
//         1 otherwise.
int checkBarrierId(const JsonDocument& jDoc){
  JsonVariantConst value = jDoc["barrier_id"];
  if(value.isNull()){return 1;}

  unsigned long receivedId = value.as<unsigned long>();
  return pCurrentBarrier->checkBarrierId(receivedId) ? 0 : 1;
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
const char* getOpCode(const JsonDocument& jDoc){
  JsonVariantConst value = jDoc["data_type"];
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
  pCurrentBarrier->handleOpCode(opCode);
}


/* Group: BLE detector */
// handles bluetooth scan results
void handleScanResult(BLEScanResults results){
  PRINTLN_WHEN_DEBUG("scan finished");
  int deviceCount = results.getCount();
  PRINT_WHEN_DEBUG("There are ");
  PRINT_WHEN_DEBUG(deviceCount);
  PRINTLN_WHEN_DEBUG(" devices in the vicinity:");

  // RECALCULATE if the format of json file changes
  const int jsonCapacity =  JSON_OBJECT_SIZE(3)   // root
                          + JSON_OBJECT_SIZE(2)   // barrier_info
                          + JSON_ARRAY_SIZE(deviceCount) + (deviceCount) * JSON_OBJECT_SIZE(2) // bluetooth_decives
                          + deviceCount * 41      // duplication of strings of keys
                          + 81;                  // fixed string duplication
  DynamicJsonDocument jDoc(jsonCapacity);
  
  char **addresses = buildOutgoingJDoc(jDoc, results, deviceCount);
  if(g_isResultValid){
    printJDocToSerial(jDoc); 
  }else{
    Serial.print("#Result abandoned.#");
  }
  deleteAddresses(addresses, deviceCount);
}

// build outgoing JsonDocument from BLEScanResults
// deviceCount : got from results.getCount().
// returns: pointer to char[deviceCount][?], 
//          which stores address strings 
// NOTE: call deleteAddresses() on this pointer with deviceCount to free memory
char **buildOutgoingJDoc(JsonDocument& jDoc, BLEScanResults& results, int deviceCount){
  jDoc["data_type"] = "m5_transmit";
  
  JsonObject barrierInfo = jDoc.createNestedObject("barrier_info");
  barrierInfo["barrier_id"] = pCurrentBarrier->getBarrierId();
  barrierInfo["barrier_type"] = pCurrentBarrier->getBarrierType();
  
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
  return addresses;
}

// called on the pointer returned by buildOutgoingJDoc() 
// when the jDoc outlived its usefulness
void deleteAddresses(char **addresses, int deviceCount){
  for(int i=0; i<deviceCount; i++){
    delete[] addresses[i];
  }
  delete[] addresses;
}

// serialise JsonDocument and print it to serial
inline void printJDocToSerial(const JsonDocument& jDoc){
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
    pCurrentBarrier->setDisplay(true);
  }
}

// toogle the simulator between the two barriers of type in and out, respectively.
void toogleBarrier(){
  if(pCurrentBarrier == &inBarrier){
    pCurrentBarrier = &outBarrier;
  }else{
    pCurrentBarrier = &inBarrier;
  }
  g_isResultValid = false;
  pCurrentBarrier->showBarrierType();
}


// interrupt handler on barrier simulation mode
// Actions:
// press BtnA: toogle between open/closed state
// press BtnB: toogle between in/out barriers
// press BtnC: switch to debug mode
void handleButtonInterruptBarrier(){
  M5.update();
  if(M5.BtnA.isPressed()){
    pCurrentBarrier->toogleBarrierState();
  }else if(M5.BtnB.isPressed()){
    toogleBarrier();
  }else if(M5.BtnC.isPressed()){
    m5State = M5State::DEBUG;  // switch back to debug mode
    pCurrentBarrier->setDisplay(false);
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
  g_isResultValid = true;
  handleScanResult(pBLEScan->start(BLE_SCAN_DURATION));
} 
