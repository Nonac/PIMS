String MQTT_topic="/PIMS/do";

void clientConnected() {
  println("client connected");
  client.subscribe(MQTT_topic);
}

void messageReceived(String topic, byte[] payload) {
   JSONObject json = parseJSONObject(new String(payload));
    if (json == null) {
     println("Order could not be parsed");
    }
    else
    {api.saveMessageToDB(json);}
}

void connectionLost() {
  println("connection lost");
}
