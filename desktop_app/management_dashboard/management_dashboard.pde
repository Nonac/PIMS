// Make sure to import mqtt and controlp5 libraries before running

import mqtt.*;
import controlP5.*;

ControlP5 cp5;
MQTTClient client;
Dashboard_view view = new Dashboard_view();
MessageData api = new MessageData();
Database db = new Database();

void setup() {
    cp5 = new ControlP5(this);
    size(1760, 990);
    // connect to the broker
    //client = new MQTTClient(this);
    // connect to the broker and use a random string for clientid
    //client.connect("mqtt://try:try@broker.hivemq.com", "processing_desktop111");
    delay(100);
    // refresh the dashboard with the information
    updateDashboardData();
    //run_tests();
}

// we don't really use the draw function as controlP5 does the work
void draw() {
    //////////////////////////////////// LOOK HERE ///////////////////////////////////////////////////////
    /* Although it doesn't do much but we still need to draw, otherwise the dropdown lists don't work */
    view.buildBackGround();
    
    /* Now just need to figure out how to change backgroud colors after clicking */
    view.buildTimer();
    view.buildButton();
    view.buildLabelText();
    
}
