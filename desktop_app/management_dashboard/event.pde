String MQTT_topic="PIMS/test0";

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
      
      String datatype=json.getString("data_type");
      
      if(datatype.equals(MessageType.USER_REGISTER)&&json.getJSONObject("info").getInt("status")==2)
      {
        JSONObject res = api.receiveRegisterFromWeb(json);
        //refreshData();
        client.publish(MQTT_topic,res.toString());
      }
      else if(datatype.equals(MessageType.USER_LOGIN)&&json.getJSONObject("info").getInt("status")==2)
      {
        
        JSONObject tmp=api.receiveLoginFromWeb(json);
        //convert the JSONObject to String
        client.publish(MQTT_topic,tmp.toString());
      }else if(datatype.equals(MessageType.FINANCE)&&json.getJSONObject("info").getInt("status")==2)
      {
          JSONObject tmp=api.receiveFinanceFromWeb(json);
          //refreshData();
          client.publish(MQTT_topic,tmp.toString());
      }else if(datatype.equals(MessageType.RECHARGE)&&json.getJSONObject("info").getInt("status")==2)
      {
     
          JSONObject tmp=api.receiveRechargeFromWeb(json);
          //refreshData();
          client.publish(MQTT_topic,tmp.toString());
      }else if(datatype.equals(MessageType.VEHICLE_REGISTER)&&json.getJSONObject("info").getInt("status")==2)
      {
     
          JSONObject tmp=api.receiveVehicleRegisterFromWeb(json);
          //refreshData();
          client.publish(MQTT_topic,tmp.toString());
      }else if(datatype.equals(MessageType.VEHICLE_QUERY)&&json.getJSONObject("info").getInt("status")==2)
      {
     
          JSONObject tmp=api.receiveVehicleQueryFromWeb(json);
          //refreshData();
          client.publish(MQTT_topic,tmp.toString());
      }
      
    }

}

void connectionLost() {
  println("connection lost");
}
