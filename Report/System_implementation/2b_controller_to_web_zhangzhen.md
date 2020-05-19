### Desktop App Back end 



### Evaluation 

+ communication rule

1. This design of data schema is different from that tutors recommend. 10 types of data make each json object is small, and each message **do not have redundant information** since each data type message has its own specified purpose. The three-sides applications grab the message belongs to them by identify data type and status. **Such design reduce the size of package and improve the efficiency of communicating.** 
2. Duplex way makes our communication robust and crash-free. Since in every request, when a sender sends query message, it expects a **confirmed  message** from receiver. So, even though our package get lost on Internet, our application will not be crashed, and sender will request again for response.

+ data scheme

1. each type of json object finish one specified functionality, and no redundant data in each json object.
2. 4 out of 10 data type needs to be stored in database, not all of them, which can save the space for hardware. 







**接着红杰的写**

#### 4. Account register

```java
  JSONObject receiveRegisterFromWeb(JSONObject message) {
    if (message == null) {
      return null;
    }
    JSONObject res = message;
    //file's name should have datatype identifier since it is useful for searching
    if (getObjWithUsername("web_register",                                        			message.getJSONObject("info").getString("username"))==null) {
      saveJSONObject(message, "data/"+message.getString("data_type") +"_"+      			message.getJSONObject("info").getString("username") + ".json");
      refreshData();
      res.getJSONObject("info").setInt("status", 1);  
      return res;
    } else {
      res.getJSONObject("info").setInt("status", 0);  
      return res;
    }
  }
```

when an user registers an account from web side, its register information will be stored into database, which can be used to check validation when user wants to login. the file is name by `"message.getString("data_type") +"_"+message.getJSONObject("info").getString("username") + ".json"` because it is read-friendly for manager to manually manage to databases. When connecting with web size, this functionality can work correctly.

#### 5 . Account login

```java
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
```

When controller receives the login message from web, controller will  traverse the database to find the matched username and check whether its password is matched.When connecting with web size, this functionality can work correctly.

#### 5. Check balance of an account 

```java
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
```

When a user register an account, it will automatically query for balance.  So, if it is the first time to query for balance, the controller will create a new finance entity to store user's balance. After that, every time, a user queries for balance, controller will  send back this entity to web side. note, this entity's balance amount can be changed when a user tops up. When connecting with web size, this functionality can work correctly.

#### 6. Top up

```java
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
```

When a user top up its parking credit, controller will retrieve the recharge amount from sender's message, and add it into user balance in "finance" account . After that, controller set the new balance to "recharge" message and sent it back to web side. When connecting with web size, this functionality can work correctly.





#### 7. vehicle register

```java
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
```

when user registers for a new car, this object will be stored into database. When connecting with web size, this functionality can work correctly.

#### 8. query vehicle list

```java
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
    infoFromWeb.setJSONArray("vehicle_list", values);
    infoFromWeb.setInt("status", 1);
    
    return queryMessage;
  }
```

This method is used to query car's information of an user, and one user can bind one or more cars for one account, so return message will include a jsonarray to contain car's information. If no car binded, the array will be empty. When connecting with web size, this functionality can work correctly.

#### 9.query vehicle parking history

```java
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
        int seconds=int((float(out_array[0])-float(in_array[0]))*365*24*3600
        +(float(out_array[1])-float(in_array[1]))*30*24*3600
        +(float(out_array[2])-float(in_array[2]))*24*3600
        +(float(out_array[3])-float(in_array[3]))*3600
        +(float(out_array[4])-float(in_array[4]))*60
        +(float(out_array[5])-float(in_array[5])));
        println(""+seconds);
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
      infoFromWeb.setInt("status", 1);
      infoFromWeb.setInt("0", amount0);
      infoFromWeb.setInt("1", amount1);
      infoFromWeb.setInt("2", amount2);
      infoFromWeb.setInt("3", amount3);
      infoFromWeb.setInt("4", amount4);
      infoFromWeb.setInt("5", amount5);
      infoFromWeb.setInt("6", amount6);
    return historyMessage;
  }
```

When a user wants to check last 7 days vehicle parking history,  controller will traverse database to find the corresponding "parking" history and add the parking fee to corresponding day. After that, controller will return this message and web will plot the result  to a  table. When connecting with web size, this functionality can work correctly.

