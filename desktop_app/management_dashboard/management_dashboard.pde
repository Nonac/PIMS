// Make sure to import mqtt and controlp5 libraries before running

import mqtt.*;
import controlP5.*;
import java.util.*;
 import java.util.regex.Pattern;

ControlP5 cp5;
MQTTClient client;
Dashboard_view view = new Dashboard_view();
MessageData api = new MessageData();
Database db = new Database();

void setup() {
  cp5 = new ControlP5(this);
  size(1760, 990);
  // connect to the broker
  client = new MQTTClient(this);
  // connect to the broker and use a random string for clientid
  //client.connect("mqtt://try:try@broker.hivemq.com", "processing_desktop111");
  delay(100);
  //refreshData();
  // refresh the dashboard with the information
  view.build();
  updateDashboardData(); 
  
}

// we don't really use the draw function as controlP5 does the work
void draw() {
  int s = second();  // Values from 0 - 59
  int m = minute();  // Values from 0 - 59
  int h = hour();    // Values from 0 - 23
  //////////////////////////////////// LOOK HERE ///////////////////////////////////////////////////////
  //All the complements as follow could change their color with color switch.
  //It may not work outside draw().
  view.buildBackground();  
  timer.setText(""+h+":"+m+":"+s);
}
