//Different types of message
static abstract class MessageType {
  static final String PARKING = "parking";
  static final String REGISTER = "reg_info";
  static final String LOGIN = "login_info";


  static final String VEHICLE_REGISTER = "web_vehicle_register";
  static final String VEHICLE_QUERY = "web_vehicle_query";
  static final String VEHICLE_HISTORY= "web_vehicle_history";

  static final String TRANSMIT="m5_transmit";

  static final String FINANCE = "web_finance";
  static final String RECHARGE= "web_recharge";
  static final String USER_REGISTER = "web_register";
  static final String USER_LOGIN = "web_login";
}

//Database
private class Database {
  int max_message=10000;
  JSONObject[] messages=new JSONObject[max_message];
  Database() {
  }
  int max_message() {
    return max_message;
  }
}

//Record a bluetooth address and its strength
private class Device {
  String address;
  int rssi;
  public Device(String address, int rssi)
  {
    this.address=address;
    this.rssi=rssi;
  }
}

// copy any JSON objects on disk into working memory
void refreshData() {
  File dir;
  File[] files;
  dir = new File(dataPath(""));
  files = dir.listFiles();
  JSONObject json;

  for (int i = 0; i <= files.length - 1; i++) {

    String path = files[i].getAbsolutePath();

    if (path.toLowerCase().endsWith(".json")) {
      json = loadJSONObject(path);
      if (json != null) {
        db.messages[i] = json;
      }
    }
  }
}

void deleteFile(String path) {
  File f = new File(path);
  if (f.exists()) {
    f.delete();
  }
}


//traverse the database and return a integer
int getAllCarsInLotCount() {
  //JSONArray array = new JSONArray();
  int count = 0;
  for (JSONObject message : db.messages)
  {
    if (message!=null && message.getString("data_type").equals(MessageType.PARKING)) 
    {
      if (message.getJSONObject("info").getString("barrier_type").equals("in"))
      {
        //array.setJSONObject(count, message);
        count++;
      }
    }
  }
  return count;
}

JSONArray getNewCarsComingListFromDb() {
  JSONArray array = new JSONArray();
  int count = 0;
  for (JSONObject message : db.messages)
  {
    if (message!=null)
    {
      if (message.getString("data_type").equals(MessageType.PARKING)) {
        array.setJSONObject(count, message);
        count++;
      }
    }
  }
  return array;
}

JSONArray getRechargeListFromDb() {
  JSONArray array = new JSONArray();
  int count = 0;
  for (JSONObject message : db.messages)
  {
    if (message!=null)
    {
      if (message.getString("data_type").equals("web_recharge")) {
        array.setJSONObject(count, message);
        count++;
      }
    }
  }
  return array;
}

int totalProfit() {
  int fee=0;
  String time_in, time_out;
  for (JSONObject message : db.messages) {
    if (message!=null && message.getString("data_type").equals(MessageType.PARKING)) {
      JSONObject info = message.getJSONObject("info");
      if (info.getString("barrier_type").equals("out")) {
        time_in = info.getString("time_in");
        time_out = info.getString("time_out");
        // might need to modify the function
        fee += calcParkingFee(time_in, time_out);
      }
    }
  }
  return fee;
}

int calcParkingFee(String time_in, String time_out)
{
  int tariff=1; // 1 pound per hour
  String[] in_fee=time_in.split("-");
  String[] out_fee=time_out.split("-");
  /*Using seconds here because it's easy to demonstrate, it's not the real thing*/
  int seconds=int((float(out_fee[0])-float(in_fee[0]))*365*24*3600+(float(out_fee[1])-float(in_fee[1]))*30*24*3600+(float(out_fee[2])-float(in_fee[2]))*24*3600+
    (float(out_fee[3])-float(in_fee[3]))*3600+(float(out_fee[4])-float(in_fee[4]))*60+(float(out_fee[5])-float(in_fee[5])));
  //println("charge is "+(int)(seconds*tariff/(60*60)+tariff));
  return (int)(seconds*tariff/(60*60)+tariff); // Assume no one parks exactly 1 hour
}

//get a JSONObject from disk according to datatype and userId
JSONObject getObjWithId(String datatype, String userId)
{
  JSONObject res=null;
  for (JSONObject message : db.messages)
  {
    if (message!=null&&message.getString("data_type").equals(datatype))
    {
      if (message.getString("user_id").equals(userId))
      {
        res=message;
      }
    }
  }
  return res;
}


//get a JSONObject from disk according to datatype and username
JSONObject getObjWithUsername(String datatype, String username)
{
  JSONObject res=null;
  for (JSONObject message : db.messages)
  {
    if (message!=null&&message.getString("data_type").equals(datatype))
    {
      JSONObject info = message.getJSONObject("info");
      if (info.getString("username").equals(username))
      {
        res=message;
      }
    }
  }
  return res;
}
//get a JSONObject from disk according to datatype and identifier
JSONObject getObjFromDb(String datatype, String identifier, String str)
{
  JSONObject res=null;
  for (JSONObject message : db.messages)
  {
    if (message!=null&&message.getString("data_type").equals(datatype))
    {
      JSONObject info = message.getJSONObject("info");
      if (info.getString(identifier).equals(str))
      {
        res=message;
      }
    }
  }
  return res;
}

//Provide API for view, m5stack and web
public class MessageData {
  /* Do not use makeUUID as a part of name in file, it is useless and impossible to find and delete file
   the file name 's format is  "data/"+message.getString("data_type") +"_"+ message.getJSONObject("info").getString("username") + ".json"
   String makeUUID()
   {
   String result="";
   int strLen=32;
   String characters="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
   int charactersLength=characters.length();
   for(int i=0;i<strLen;i++)
   {
   result += characters.charAt((int)Math.floor(Math.random() * charactersLength));
   }
   return result;
   }*/
  //receive register message from web
  JSONObject receiveRegisterFromWeb(JSONObject message) {
    if (message == null) {
      return null;
    }
    JSONObject res = message;
    //file's name should have datatype identifier since it is useful for searching
    if (getObjWithUsername("web_register", message.getJSONObject("info").getString("username"))==null) {
      saveJSONObject(message, "data/"+message.getString("data_type") +"_"+ message.getJSONObject("info").getString("username") + ".json");
      refreshData();
      res.getJSONObject("info").setInt("status", 1);  
      return res;
    } else {
      res.getJSONObject("info").setInt("status", 0);  
      return res;
    }
  }

  //send message to web to allow access or not
  JSONObject receiveLoginFromWeb(JSONObject loginMessage) {
    //String userId=loginMessage.getString("user_id");
    // use reg_info object to check whether password is correct
    String datatype="web_register";
    JSONObject infoFromWeb = loginMessage.getJSONObject("info");
    String username = infoFromWeb.getString("username");
    JSONObject obj= getObjWithUsername(datatype, username);
    if (obj!=null&&obj.getJSONObject("info").getString("password").equals(infoFromWeb.getString("password")))
    {
      //loginMessage.setInt("access",1);
      infoFromWeb.setInt("status", 1);
    } else {
      //loginMessage.setInt("access",0);
      infoFromWeb.setInt("status", 0);
    }

    return loginMessage;
  }

  //receive finance message from web and sent validation message back
  JSONObject receiveFinanceFromWeb(JSONObject financeMessage) {
    if (financeMessage == null) {
      return null;
    }
    if (getObjWithUsername("web_finance", financeMessage.getJSONObject("info").getString("username"))==null) {
      JSONObject info = financeMessage.getJSONObject("info");
      info.setInt("balance", 0);
      info.setString("currency", "GBP");
      info.setInt("status", 1);
      saveJSONObject(financeMessage, "data/"+financeMessage.getString("data_type") + "_"+ financeMessage.getJSONObject("info").getString("username") + ".json");
      refreshData();
      return financeMessage;
    }
    JSONObject res = getObjWithUsername("web_finance", financeMessage.getJSONObject("info").getString("username"));
    res.getJSONObject("info").setInt("status", 1);

    return res;
  }

  JSONObject receiveRechargeFromWeb(JSONObject rechargeMessage) {
    JSONObject infoFromWeb = rechargeMessage.getJSONObject("info");
    String username = infoFromWeb.getString("username");
    JSONObject financeMessage= getObjWithUsername(MessageType.FINANCE, username);
    if (financeMessage==null) {
      rechargeMessage.getJSONObject("info").setInt("status", 0);
      return rechargeMessage;
    }

    int rechargeAmount = rechargeMessage.getJSONObject("info").getInt("pay_amount");
    int balance = financeMessage.getJSONObject("info").getInt("balance");
    balance = balance+ rechargeAmount;
    financeMessage.getJSONObject("info").setInt("balance", balance);

    String path = dataPath("")+"\\"+MessageType.FINANCE+"_"+financeMessage.getJSONObject("info").getString("username")+".json";
    deleteFile(path);
    saveJSONObject(financeMessage, "data/"+financeMessage.getString("data_type") + "_"+ financeMessage.getJSONObject("info").getString("username") + ".json");
    refreshData();
    rechargeMessage.getJSONObject("info").setInt("balance", balance);
    rechargeMessage.getJSONObject("info").setInt("status", 1);
    return rechargeMessage;
  }

  JSONObject receiveVehicleRegisterFromWeb(JSONObject message) {
    if (message == null) {
      return null;
    }
    JSONObject res = message;
    //file's name should have datatype identifier since it is useful for searching
    if (getObjFromDb("web_vehicle_register", "vehicle_id", message.getJSONObject("info").getString("vehicle_id"))==null) {
      saveJSONObject(message, "data/"+message.getString("data_type") +"_"+ message.getJSONObject("info").getString("username")+
        "_"+ message.getJSONObject("info").getString("vehicle_id") + ".json");
      refreshData();
      res.getJSONObject("info").setInt("status", 1);  
      return res;
    } else {
      res.getJSONObject("info").setInt("status", 0);  
      return res;
    }
  }

  //send message to web to allow access or not
  JSONObject receiveVehicleQueryFromWeb(JSONObject queryMessage) {

    String datatype="web_vehicle_register";
    JSONObject infoFromWeb = queryMessage.getJSONObject("info");
    String username = infoFromWeb.getString("username");


    JSONArray values = new JSONArray();
    int count = 0;
    for (JSONObject message : db.messages)
    {
      if (message!=null&&message.getString("data_type").equals(datatype)
        && message.getJSONObject("info").getString("username").equals(username)
        )
      {
        JSONObject vehicleInfo = new JSONObject();
        JSONObject infoFromDb = message.getJSONObject("info");
        String vehicle_id =  infoFromDb.getString("vehicle_id");
        String vehicle_type =  infoFromDb.getString("vehicle_type");
        vehicleInfo.setString("vehicle_id", vehicle_id );
        vehicleInfo.setString("vehicle_type", vehicle_type );
        values.setJSONObject(count, vehicleInfo);
        count++;
      }
    }
    if (count==0) {
      infoFromWeb.setInt("status", 0);
    } else {
      infoFromWeb.setJSONArray("vehicle_list", values);
      infoFromWeb.setInt("status", 1);
    }
    return queryMessage;
  }

  JSONObject receiveVehicleHistoryFromWeb(JSONObject historyMessage) {

    String datatype="parking";
    JSONObject infoFromWeb = historyMessage.getJSONObject("info");
    String username = infoFromWeb.getString("username");
    String vehicle_id = infoFromWeb.getString("vehicle_id");
    int currentYear = year();
    int currentMonth = month();
    int currentDay = day();
    int flag=0;
    int amount0=0;
    int amount1=0;
    int amount2=0;
    int amount3=0;
    int amount4=0;
    int amount5=0;
    int amount6=0;

    for (JSONObject message : db.messages)
    {
      if (message!=null&&message.getString("data_type").equals(datatype)
        && message.getJSONObject("info").getString("username").equals(username)
        && message.getJSONObject("info").getString("vehicle_id").equals(vehicle_id)
        )
      {
        JSONObject infoFromParking = message.getJSONObject("info");
        String time_in = infoFromParking.getString("time_in");
        String time_out = infoFromParking.getString("time_out");
        String[] in_array=time_in.split("-");
        String[] out_array=time_out.split("-");
        flag =1;
        if (currentYear!= Integer.valueOf(in_array[0])
          ||currentMonth!=Integer.valueOf(in_array[1])
          ||currentDay-Integer.valueOf(in_array[2])>6
          ) {
          continue;
        }
        int amount = currentDay-Integer.valueOf(in_array[2]);
        int seconds=int((float(out_array[0])-float(in_array[0]))*365*24*3600+(float(out_array[1])-float(in_array[1]))*30*24*3600+(float(out_array[2])-float(in_array[2]))*24*3600+
          (float(out_array[3])-float(in_array[3]))*3600+(float(out_array[4])-float(in_array[4]))*60+(float(out_array[5])-float(in_array[5])));
        switch(amount) {
        case 0:
          amount0 = amount0 + seconds;
          break;
        case 1:
          amount1 = amount1 + seconds;
          break;  
        case 2:
          amount2 = amount2 + seconds;
          break; 
        case 3:
          amount3 = amount3 + seconds;
          break;
        case 4:
          amount4 = amount4 + seconds;
          break; 
        case 5:
          amount5 = amount5 + seconds;
          break; 
        case 6:
          amount6 = amount6 + seconds;
          break; 
        default:
          break;
        }
      }
    }

    if (flag==0) {
      infoFromWeb.setInt("status", 0);
    } else {
      infoFromWeb.setInt("status", 1);
      infoFromWeb.setInt("0", amount0);
      infoFromWeb.setInt("1", amount1);
      infoFromWeb.setInt("2", amount2);
      infoFromWeb.setInt("3", amount3);
      infoFromWeb.setInt("4", amount4);
      infoFromWeb.setInt("5", amount5);
      infoFromWeb.setInt("6", amount6);
    }
    return historyMessage;
  }


  void receiveTransmitFromM5(JSONObject transmitMessage)
  {
    JSONArray arr=transmitMessage.getJSONArray("bluetooth_devices");
    //Store all the bluetooth address and strengths
    ArrayList<Device> list=new ArrayList<Device>();
    String targetUser=null;
    String targetType=null;
    String targetId=null;
    for (int i=0; i<arr.size(); i++)
    {
      JSONObject item=arr.getJSONObject(i);
      String address=item.getString("bluetooth_address");
      int rssi=item.getInt("RSSI");
      list.add(new Device(address, rssi));
    }
    int flag=0;
    while (list.size()>0)
    {
      int max=-1000;
      int k=-1;
      //Find the bluetooth address with the strongest signal
      for (int i=0; i<list.size(); i++)
      {
        if (list.get(i).rssi>max)
        {
          max=list.get(i).rssi;
          k=i;
        }
      }
      refreshData();
      //find a registerd vehicle
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
      if (flag==1)
        break;
      else
      {
        list.remove(k);
      }
    }

    if (flag==1)
    {
      for (JSONObject message : db.messages)
      {
        if (message!=null&&message.getString("data_type").equals(MessageType.PARKING))
        {
          JSONObject info = message.getJSONObject("info");
          if (info.getString("username").equals(targetUser)&&info.getString("barrier_type").equals("in"))
          {

            String path = dataPath("")+"\\"+MessageType.PARKING+"_"+message.getJSONObject("info").getString("username")+"_"+message.getJSONObject("info").getString("time_in")+".json";
            deleteFile(path);
            String date=year()+"-"+month()+"-"+day()+"-"+hour()+"-"+minute()+"-"+second();
            info.setString("time_out", date);
            info.setString("barrier_type", "out");
            message.setJSONObject("info", info);
            saveJSONObject(message, "data/"+message.getString("data_type") + "_"+ message.getJSONObject("info").getString("username") +"_"+ message.getJSONObject("info").getString("time_in")+".json");
            refreshData();
            parkingCharge(info.getString("time_in"), date, targetUser);
            return;
          }
        }
      }
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
      return;
    }
  }
  /*Charge the fee after parking*/
  void parkingCharge(String time_in, String time_out, String username) {

    JSONObject financeMessage= getObjWithUsername(MessageType.FINANCE, username);

    int balance = financeMessage.getJSONObject("info").getInt("balance");
    balance = balance- calcParkingFee(time_in, time_out);
    financeMessage.getJSONObject("info").setInt("balance", balance);
    String path = dataPath("")+"\\"+MessageType.FINANCE+"_"+financeMessage.getJSONObject("info").getString("username")+".json";
    deleteFile(path);
    saveJSONObject(financeMessage, "data/"+financeMessage.getString("data_type") + "_"+ financeMessage.getJSONObject("info").getString("username") + ".json");
    refreshData();
  }

  
}        
