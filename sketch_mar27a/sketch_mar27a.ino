#include <SPI.h>
#include <WiFiNINA.h>

WiFiClient wifi_client;
#include <PubSubClient.h>
PubSubClient ps_client( wifi_client );

// redirect serial output from USB to M5Stack
#define Serial Serial1


// Wifi settings
char wifi_ssid[] = "LAPTOP-3MHBNCLF 4177";   
char wifi_password[] = "malvinas";                     

// MQTT Settings
const char* MQTT_clientname = "barrier"; // Make up a short name
const char* MQTT_sub_topic = "m5comm"; // pub/sub topics
const char* MQTT_pub_topic = "m5comm"; // You might want to create your own

// Please leave this alone - to connect to HiveMQ
const char* server = "broker.mqttdashboard.com";
const int port = 1883;

void publishFromSerial(){
  int byteCount = Serial.available();
  if(byteCount <=0){return;}
  
  char *byteBuffer { new char[byteCount + 1] {} }; 
  Serial.readBytes(byteBuffer, byteCount);
  if(ps_client.connected()){
    ps_client.publish( MQTT_pub_topic, byteBuffer );
  }else{
    Serial.println("Can't publish message: Not connected to MQTT :( ");
  }
  delete[] byteBuffer;
}

void setupWifi(){
  Serial.println("Connecting to wifi.");
  WiFi.begin(wifi_ssid, wifi_password);
  while(WiFi.status() != WL_CONNECTED){
    Serial.println("unable to connect to wifi. Still trying...");
    delay(500);
  }
  Serial.println("wifi connection done");
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
  Serial.print("] ");

  String in_str = "";

  // Copy chars to a String for convenience.
  // Also prints to USB serial for debugging
  for (int i=0;i<length;i++) {
    in_str += (char)payload[i];
    Serial.print((char)payload[i]);
  }
  Serial.println();

  Serial.print(" << Rx: " );
  Serial.println( in_str );
}

void setupMQTT() {
    ps_client.setServer(server, port);
    ps_client.setCallback(callback);
}

void reconnect() {

  // Loop until we're reconnected
  while (!ps_client.connected()) {

    Serial.print("Attempting MQTT connection...");

    // Attempt to connect
    // Sometimes a connection with HiveMQ is refused
    // because an old Client ID is not erased.  So to
    // get around this, we just generate new ID's
    // every time we need to reconnect.
    String new_id = generateID();

    Serial.print("connecting with ID ");
    Serial.println( new_id );

    char id_array[ (int)new_id.length() ];
    new_id.toCharArray(id_array, sizeof( id_array ) );

    if (ps_client.connect( id_array ) ) {
      Serial.println("connected");

      // Once connected, publish an announcement...
      ps_client.publish( MQTT_pub_topic, "reconnected");
      // ... and resubscribe
      ps_client.subscribe( MQTT_sub_topic );
    } else {
      Serial.println(" - Coudn't connect to HiveMQ :(");
      Serial.println("   Trying again.");
      Serial.print("failed, rc=");
      Serial.print(ps_client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
  Serial.println(" - Success!  Connected to HiveMQ\n\n");
}

String generateID() {

  String id_str = MQTT_clientname;
  id_str += random(0,9999);

  return id_str;
}

void setup() {
  Serial.begin(9600);
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

  publishFromSerial();

}
