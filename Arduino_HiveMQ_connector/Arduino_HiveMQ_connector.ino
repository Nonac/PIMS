#include <SPI.h>
#include <WiFiNINA.h>

WiFiClient wifi_client;
#include <PubSubClient.h>
PubSubClient ps_client( wifi_client );

//Default: 128bytes, which is too small.
//change the header file there: C:\Users\?\Documents\Arduino\libraries\PubSubClient\src
//#define MQTT_MAX_PACKET_SIZE 8192



#define SERIAL_JSON_DELIMITER '#'

// Wifi settings
char wifi_ssid[] = "wifi_ssid";   
char wifi_password[] = "wifi_password";   

// Serial settings
const int serial1_timeout {300}; // in milliseconds

// MQTT Settings
const char* MQTT_clientname = "barrier"; // Make up a short name
const char* MQTT_sub_topic = "m5_receive"; // pub/sub topics
const char* MQTT_pub_topic = "m5_transmit"; // You might want to create your own

// Please leave this alone - to connect to HiveMQ
const char* server = "broker.mqttdashboard.com";
const int port = 1883;

// publish message to MQTT broker from serial1
// Note: serial1 port is supposed to be connected to M5Stack
void publishFromSerial1(){
  int byteCount = Serial1.available();
  if(byteCount <=0){return;}
  Serial1.find(SERIAL_JSON_DELIMITER);
  String serial1Read = Serial1.readStringUntil(SERIAL_JSON_DELIMITER);
  
  if(ps_client.connected()){
    Serial.println(serial1Read);
    publishMessage(serial1Read);
  }else{
    Serial1.println("Can't publish message: Not connected to MQTT :( ");
    Serial.println("Can't publish message: Not connected to MQTT :( ");
  }
}

// connects to wifi
void setupWifi(){
  Serial1.println("Connecting to wifi.");
  do{
    WiFi.begin(wifi_ssid, wifi_password);
    const int waitSectionCount = 10;
    for(int i=0; i<waitSectionCount; i++){
      if(WiFi.status() == WL_CONNECTED){
        break;
      }
      Serial1.println("unable to connect to wifi. Still trying...");
      delay(500);
    }   
  }while(WiFi.status() != WL_CONNECTED);

  Serial1.println("wifi connected.");
}


// publish message to MQTT broker
void publishMessage( String message ) {

  if( ps_client.connected() ) {

    // Make sure the message isn't blank.
    if( message.length() > 0 ) {

      // Convert to char array
      char *msg {new char[message.length()] {}};
      message.toCharArray( msg, message.length() );

      Serial.print(">> Tx: ");
      Serial.println( msg );

      // Send
      ps_client.publish( MQTT_pub_topic, msg);
      delete[] msg;
    }

  } else {
    Serial.println("Can't publish message: Not connected to MQTT :( ");

  }
}

// This is where we pick up messages from the MQTT broker.
// This function is called automatically when a message is
// received.
//
// Note that, it receives from MQTT_topic value.
//
// Note that, you will receive messages from yourself, if
// you publish a message, activating this function.

void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("] ");

  printCallbackToSerial1((const char*)payload);
}

// print callback from MQTT broker to serial1 port
inline void printCallbackToSerial1(const char* payload){
  Serial1.print(SERIAL_JSON_DELIMITER);
  Serial1.print(payload);
  //Serial1.flush();
  Serial1.print(SERIAL_JSON_DELIMITER);
}

void setupMQTT() {
    ps_client.setServer(server, port);
    ps_client.setCallback(callback);
}

// reconnects to MQTT broker
void reconnect() {

  // Loop until we're reconnected
  while (!ps_client.connected()) {

    Serial1.print("Attempting MQTT connection...");

    // Attempt to connect
    // Sometimes a connection with HiveMQ is refused
    // because an old Client ID is not erased.  So to
    // get around this, we just generate new ID's
    // every time we need to reconnect.
    String new_id = generateID();

    Serial1.print("connecting with ID ");
    Serial1.println( new_id );

    char id_array[ (int)new_id.length() ];
    new_id.toCharArray(id_array, sizeof( id_array ) );

    if (ps_client.connect( id_array ) ) {
      Serial1.println("connected");

      // Once connected, publish an announcement...
      ps_client.publish( MQTT_pub_topic, "reconnected");
      // ... and resubscribe
      ps_client.subscribe( MQTT_sub_topic );
    } else {
      Serial1.println(" - Coudn't connect to HiveMQ :(");
      Serial1.println("   Trying again.");
      Serial1.print("failed, rc=");
      Serial1.print(ps_client.state());
      Serial1.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
  Serial1.println(" - Success!  Connected to HiveMQ\n\n");
}

String generateID() {

  String id_str = MQTT_clientname;
  id_str += random(0,9999);

  return id_str;
}

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  //Serial1.setTimeout(serial1_timeout);
  setupWifi();
  setupMQTT();
}

void loop() {
  // Leave this code here.  It checks that you are
  // still connected, and performs an update of itself.
  if (!ps_client.connected()) {
    reconnect();
  }
  ps_client.loop();

  publishFromSerial1();

}
