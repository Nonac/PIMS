String MQTT_topic="PIMS";

void clientConnected() {
  println("client connected");
  client.subscribe(MQTT_topic);
}

void messageReceived(String topic, byte[] payload) {
  JSONObject json = parseJSONObject(new String(payload));
  if (json == null) {
    println("Order could not be parsed");
  } else 
  {

    String datatype=json.getString("data_type");
    if (datatype.equals(MessageType.USER_REGISTER)&&json.getJSONObject("info").getInt("status")==2)
    {
      JSONObject res = api.receiveRegisterFromWeb(json);
      client.publish(MQTT_topic, res.toString());
    } else if (datatype.equals(MessageType.USER_LOGIN)&&json.getJSONObject("info").getInt("status")==2)
    {

      JSONObject tmp=api.receiveLoginFromWeb(json);
      //convert the JSONObject to String
      client.publish(MQTT_topic, tmp.toString());
    } else if (datatype.equals(MessageType.FINANCE)&&json.getJSONObject("info").getInt("status")==2)
    {
      JSONObject tmp=api.receiveFinanceFromWeb(json);

      client.publish(MQTT_topic, tmp.toString());
    } else if (datatype.equals(MessageType.RECHARGE)&&json.getJSONObject("info").getInt("status")==2)
    {
      JSONObject tmp=api.receiveRechargeFromWeb(json);
      client.publish(MQTT_topic, tmp.toString());
    } else if (datatype.equals(MessageType.VEHICLE_REGISTER)&&json.getJSONObject("info").getInt("status")==2)
    {
      println(1);
      JSONObject tmp=api.receiveVehicleRegisterFromWeb(json);

      client.publish(MQTT_topic, tmp.toString());
    } else if (datatype.equals(MessageType.VEHICLE_QUERY)&&json.getJSONObject("info").getInt("status")==2)
    {
      JSONObject tmp=api.receiveVehicleQueryFromWeb(json);
      client.publish(MQTT_topic, tmp.toString());
    }else if (datatype.equals(MessageType.VEHICLE_HISTORY)&&json.getJSONObject("info").getInt("status")==2)
    {
      JSONObject tmp=api.receiveVehicleHistoryFromWeb(json);
      client.publish(MQTT_topic, tmp.toString());
    } else if (datatype.equals(MessageType.TRANSMIT))
    {
      api.receiveTransmitFromM5(json);
    }
  }
  refreshDashboardData();
}

void connectionLost() {
  println("connection lost");
}

//Open the barrier
void barrierOpen(int barrierId)
{
  JSONObject json=new JSONObject();
  json.setString("data_type", "m5_receive");
  json.setInt("barrier_id", barrierId);
  json.setString("op_code", "A");
  client.publish(MQTT_topic, json.toString());
}

//Close the barrier
void barrierClose(int barrierId)
{
  JSONObject json=new JSONObject();
  json.setString("data_type", "m5_receive");
  json.setInt("barrier_id", barrierId);
  json.setString("op_code", "B");
  client.publish(MQTT_topic, json.toString());
}
