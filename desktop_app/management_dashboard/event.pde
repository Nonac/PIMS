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
    {
      String datatype=json.getString("daya_type");
      if(datatype.equals(MessageType.REGISTER))
      {
        api.saveMessageToDB(json);
      }
      else if(datatype.equals(MessageType.LOGIN))
      {
        JSONObject tmp=api.sendConfirmInfoToWeb(json);
        //convert the JSONObject to String
        client.publish(MQTT_topic,tmp.toString());
      }
    }

}

void connectionLost() {
  println("connection lost");
}
