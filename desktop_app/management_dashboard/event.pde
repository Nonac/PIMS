String MQTT_topic="PIMS/test";

void clientConnected() {
  println("client connected");
  client.subscribe(MQTT_topic);
}

void messageReceived(String topic, byte[] payload) {
   JSONObject json = parseJSONObject(new String(payload));
   println(topic);
    if (json == null) {
     println("Order could not be parsed");
    }
    else 
    {
      
      String datatype=json.getString("data_type");
      
      if(datatype.equals(MessageType.USER_REGISTER)&&json.getJSONObject("info").getInt("status")==2)
      {
        JSONObject res = api.saveMessageToDB(json);
        refreshData();
        client.publish(MQTT_topic,res.toString());
        println("register");
      }
      else if(datatype.equals(MessageType.USER_LOGIN)&&json.getJSONObject("info").getInt("status")==2)
      {
        
        JSONObject tmp=api.sendConfirmInfoToWeb(json);
        //convert the JSONObject to String
        println("login");
        client.publish(MQTT_topic,tmp.toString());
      }else if(datatype.equals(MessageType.FINANCE))
      {
          JSONObject tmp=api.sendFinanceInfoToWeb(json);
          client.publish(MQTT_topic,tmp.toString());
      }
    }

}

void connectionLost() {
  println("connection lost");
}
