# Desktop App Backend

### 1.Find the registered car from the Bluetooth address list
Our requirement is to sort from a series of Bluetooth addresses first according to the Bluetooth signal, and select the car with the strongest Bluetooth signal which is registered.
So we used the data structure ArrayList to store the class Device.
```
ArrayList<Device> list=new ArrayList<Device>();

private class Device {
  String address;
  int rssi;
  public Device(String address, int rssi)
  {
    this.address=address;
    this.rssi=rssi;
  }
}
```
Every time the device with the strongest signal is found, then by traversing the database to determine whether the car has been registered.
```
for (JSONObject message : db.messages)
      {
        if (message!=null&&message.getString("data_type").equals(MessageType.VEHICLE_REGISTER))
        {
          JSONObject info = message.getJSONObject("info");
          if (info.getString("bluetooth_address").equals(list.get(k).address))
          {
            flag=1;
            // flag=1 means find the target vehicle which is registered
            targetUser=info.getString("username");
            targetId=info.getString("vehicle_id");
            targetType=info.getString("vehicle_type");
            break;
          }
        }
      }
```
### 2.Generate new parking records.
 Use Processing's year (), month (), day () and other APIs to record the parking time.A delay () API is also used here, which is used to simulate the time of car storage, which is fixed for 5 seconds. Unfortunately, the dealy () function is a single-threaded function that blocks the entire program. Therefore, multi-threading will be adopted in the future. Even if a certain thread delay (), it will not affect other threads.
```
if(barrier_type.equals("in"))
      {
      JSONObject json=new JSONObject();
      JSONObject newInfo=new JSONObject();
      json.setString("data_type", MessageType.PARKING);
      newInfo.setString("username", targetUser);
      String date=year()+"-"+month()+"-"+day()+"-"+hour()+"-"+minute()+"-"+second();
      newInfo.setString("time_in", date);
      newInfo.setString("time_out", null);
      newInfo.setInt("barrier_id", transmitMessage.getJSONObject("barrier_info").getInt("barrier_id"));
      newInfo.setString("vehicle_id", targetId);
      newInfo.setString("vehicle_type", targetType);
      newInfo.setString("barrier_type", "in");
      json.setJSONObject("info", newInfo);
      saveJSONObject(json, "data/"+json.getString("data_type") + "_"+ json.getJSONObject("info").getString("username") +"_"+ json.getJSONObject("info").getString("time_in")+".json");
      refreshData();
       barrierOpen(transmitMessage.getJSONObject("barrier_info").getInt("barrier_id"));
       //to allow cars in
      delay(5000);
      barrierClose(transmitMessage.getJSONObject("barrier_info").getInt("barrier_id"));
      
      }
```

### 3.Calculate parking fees.
Here, we need to calculate the cost based on the parking time of the garage recorded in the date. Therefore, we need to use all the information such as year, month, day, hour, minute and second to calculate the cost according to the different time of parking. Because of the need for display, we set tariff to 1, which can more intuitively display the changes in parking fees.
```
int calcParkingFee(String time_in, String time_out)
{
  int tariff=1; // 1 pound per hour/ per second for presentation purpose
  String[] in_fee=time_in.split("-");
  String[] out_fee=time_out.split("-");
  /*Using seconds here because it's easy to demonstrate, it's not the real thing*/
  int seconds=int((float(out_fee[0])-float(in_fee[0]))*365*24*3600+(float(out_fee[1])-float(in_fee[1]))*30*24*3600+(float(out_fee[2])-float(in_fee[2]))*24*3600+
    (float(out_fee[3])-float(in_fee[3]))*3600+(float(out_fee[4])-float(in_fee[4]))*60+(float(out_fee[5])-float(in_fee[5])));
  //println("charge is "+(int)(seconds*tariff/(60*60)+tariff));
  return (int)(seconds*tariff); // Due to the limited time during presentation, we are charging by seconds.
}
```
