#include <SPI.h>
#include <WiFiNINA.h>

WiFiClient wifi_client;
#include <PubSubClient.h>
PubSubClient ps_client( wifi_client );

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

void publishFromSerial11(){
  int byteCount = Serial1.available();
  if(byteCount <=0){return;}
  
  char *byteBuffer { new char[byteCount + 1] {} }; 
  Serial1.readBytes(byteBuffer, byteCount);
  if(ps_client.connected()){
    Serial.println(byteBuffer);
    //ps_client.publish( MQTT_pub_topic, byteBuffer );
  }else{
    Serial1.println("Can't publish message: Not connected to MQTT :( ");
  }
  delete[] byteBuffer;
}

void setupWifi(){
  Serial1.println("Connecting to wifi.");
  do{
    WiFi.begin(wifi_ssid, wifi_password);
    Serial1.println("unable to connect to wifi. Still trying...");
    const int waitSectionCount = 10;
    for(int i=0; i<waitSectionCount; i++){
      if(WiFi.status() == WL_CONNECTED){
        break;
      }
      Serial1.println("unable to connect to wifi. Still trying...");
    }   
  }while(WiFi.status() != WL_CONNECTED);

  
  Serial1.println("wifi connected.");
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

  Serial1.print("Message arrived [");
  Serial1.print(topic);
  Serial1.print("] ");

  String in_str = "";

  // Copy chars to a String for convenience.
  // Also prints to USB Serial1 for debugging
  for (int i=0;i<length;i++) {
    in_str += (char)payload[i];
    Serial1.print((char)payload[i]);
  }
  Serial1.println();

  Serial1.print(" << Rx: " );
  Serial1.println( in_str );
}

void setupMQTT() {
    ps_client.setServer(server, port);
    ps_client.setCallback(callback);
}

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
  Serial.begin(9600);
  Serial1.begin(9600);
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

  publishFromSerial11();

}
