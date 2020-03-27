#include <SPI.h>
#include <WiFiNINA.h>

WiFiClient wifi_client;
#include <PubSubClient.h>
PubSubClient ps_client( wifi_client );

// Extra, created by SEGP team
#include "Timer.h"
// Instance of a Timer class, which allows us
// a basic task scheduling of some code.  See
// it used in Loop().
// See Timer.h for more details.
// Argument = millisecond period to schedule
// task.  Here, 2 seconds.
Timer publishing_timer(2000);


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

void setupWifi(){
  Serial.println("Connecting to wifi.");
  WiFi.begin(wifi_ssid, wifi_password);
  while(WiFi.status() != WL_CONNECTED){
    Serial.println("unable to connect to wifi. Still trying...");
    delay(500);
  }
  Serial.println("wifi connection done");
}

// Use this function to publish a message.  It currently
// checks for a connection, and checks for a zero length
// message.  Note, it doens't do anything if these fail.
//
// Note that, it publishes to MQTT_topic value
//
// Also, it doesn't seem to like a concatenated String
// to be passed in directly as an argument like:
// publishMessage( "my text" + millis() );
// So instead, pre-prepare a String variable, and then
// pass that.
void publishMessage( String message ) {

  if( ps_client.connected() ) {

    // Make sure the message isn't blank.
    if( message.length() > 0 ) {

      // Convert to char array
      char msg[ message.length() ];
      message.toCharArray( msg, message.length() );

      Serial.print(">> Tx: ");
      Serial.println( message );

      // Send
      ps_client.publish( MQTT_pub_topic, msg );
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

    // This is an example of using our timer class to
  // publish a message every 2000 milliseconds, as
  // set when we initalised the class above.
  if( publishing_timer.isReady() ) {

      // Prepare a string to send.
      // Here we include millis() so that we can
      // tell when new messages are arrive in hiveMQ
      String new_string = "hello?";
      new_string += millis();
      publishMessage( new_string );

      // Remember to reset your timer when you have
      // used it. This starts the clock again.
      publishing_timer.reset();
  }


}