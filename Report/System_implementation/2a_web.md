## Web Application & Controller Logics for Web

As the requirements were raised first, the data structure and the protocol for web-controller communication is mainly designed by the web developer, and the controller that deals with web requests is implemented according to the data structure. In another word, this part is directed by the data structure that is designed based on the requirements.

As the data structure may change during the web development and it is absolutely important to use a confirmed and determined data structure to direct the controller's implementation, there is a chronological order in the development of this part of our software -- web development first and controller logics for web must be implemented after the web logics finished.

For the web side (front-end), the process is logic-oriented. HTML pages and CSS styles are designed based on the built up JavaScript logics and communication protocol. 



**//by zhangzhen**

**!!!!**

**For the server controller,  it needs to negotiate data structure and communication rules with web side, and also determine how to design its database.** 

**!!!!!**

**Sprint 1** (Apr 5th)

_Objectives_

* A rough data structure satisfying all the requirements
* Simple web pages for running JavaScript  implemented with HTML and CSS

*Approach & Implementation*

In this sprint the web developer designed the first version of the data structure used for communication with controller, where 5 necessary data types are included. And we implemented the logics for login and register page.

`web_finance`, `web_recharge`, `web_login`, and `web_register` have the same name as the final version, while `web_vehicle`was split into 2 parts later. The first version was just for displayed because they are not well organized and unified.

Version #1 of the web data types

```json
{
	"data_type": "web_register",  
	"login_info": {
			"username": "lea_tong",
			"password": "*******",
            "userid":"aaaaa"
		}
}

{

	"data_type": "web_login", 
	"login_info": {
			"username": "lea_tong",
			"password": "*******",
            "access":0;
		}
}

{

	"data_type": "web_vehicle",   
	"vehicle_info": {
            "user_id":"aaaaa"
			"vehicle_id": "acdjcidjd"
			"vehicle_type":"car"
		}
}
{

	"data_type": "web_finance",  
	"finance_info": {
            "user_id":"aaaaa"
			"balance": 21331,
			"currency":"GBP"
		}
}

{

	"data_type": "web_recharge",  
	"recharge_info": {
            "user_id":"aaaaa"
			"balance": 21331,
			"currency":"GBP",
            "card_number":"326173173718",
            "pay_amount":"10"
		}
}
```

In this sprint only the very simple HTML and CSS pages for login and registration were implemented, as the developer was not quite familiar with the Bootstrap and JavaScript :)

**Sprint 2** **(Apr 6th - Apr 18th)           !!!写久一点** **byzhangzhen**

_Objectives_

* Negotiated and confirmed final data structure for web
* Stable connection established via MQTT
* Module test for the functionality of web stand-alone system

*Approach & Implementation*

In this sprint we finally confirmed all data structure for web-controller communication, established the MQTT connection, and did module test for web front side.

**//by zhangzhen**

**!!!!** 

**说 为什么要有status的存在**

**Since all of message are transmitted at one topic "PIMS", so each side needs a mechanism to grab package belongs to it. At beginning, we believe just using data type is enough to identify. However, when we test it , we find that if subscribing a topic , a sender will also receive the message sent from itself.  Since each request needs two message, one query message from sender and one confirmed message from receiver, and in communication between web and server, each request's two messages have same data type, both server side and web side grab the package that should not belong to them. So, we need another identifier, which is status to identify whether it should grab. we rule that status =2 must be the message from web to server, and status = 1 and status =0 must be message from server to web, where 1 means query succeeds and 0 means query fails.  Such mechanism successfully solved above problem.**



**In addition,** 之后的内容

**!!!!!**

The changes in [data structure](../../data_structure.json) are quite significant, in which we added a data type `web_vehicle_history` for fetching the parking history and split the type `web_vehicle` into `web_vehicle_query` and `web_vehicle_register` to fetch vehicle list of the user and to register a new vehicle for the user.

The module test was carried out to see whether functions dealing with coming data can perform properly and correctly. The approach was by publishing messages in broker and observing the response on web pages. By the end of this sprint, we had tested for each data type and confirmed that can work in the stand-alone system of web application.

**Sprint 3** (Apr 8th - Apr 10th) ***!!!!!!!!!张震在里面这加你的第一部分内容 因为你是在个时间段实现了除了history之外的功能 然后你还得再说一下你在什么时间实现了hisroty得功能 以及为什么这时候不能实现history得功能***

*Objectives*

* The style sheets and dynamic rendering of front-end web pages
* **Completed the functionality for in server controller side.**
* **Decided which data types need  to be stored in database and which do not.**

*Approach & Implementation*

The controller logics for web was built in this sprint as the data structure for this part had already been confirmed. We also implemented the CSS style sheets for front-end web pages and tested the communication between web and the controller.

\**\byzhangzhen**

**!!!!!!**

***controller*:**

+ account register. 

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

when an user registers an account from web side, its register information will be stored into database, which can be used to check validation when user wants to login. 

+ account login

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

When controller receives the login message from web, controller will  traverse the database to find the matched username and check whether its password is matched.

+ check balance of an account 

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

When a user register an account, it will automatically query for balance.  So, if it is the first time to query for balance, the controller will create a new finance entity to store user's balance. After that, every time, a user queries for balance, controller will  send back this entity to web side. note, this entity's balance amount can be changed when a user tops up.

+ top up

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

When a user top up its parking credit, controller will retrieve the recharge amount from sender's message, and add it into user balance in "finance" account . After that, controller set the new balance to "recharge" message and sent it back to web side.





+ vehicle register

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

when user registers for a new car, this object will be stored into database

+ query vehicle list

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

This method is used to query car's information of an user, and one user can bind one or more cars for one account, so return message will include a jsonarray to contain car's information. If no car binded, the array will be empty.





*test communication*:

+ all of above functionalities achieved can work correctly when connecting web with desktop.

**!!!!!!!!**





**Sprint 4** (Apr 30th - May 7th)

*Objectives*

* System tests in 3 stages
* **Functionality of querying parking history** 

*Approach & Implementation*

In this sprint we carried out the system tests for 3 stages and fixed bugs. We did found a significant bug that could cause the system cashing in this sprint: while all three parts of our software running, the web page crashes because different types of data was wrongly accepted by web JavaScript logics in `client.onMessageArrived` in [account.js](../../web_application/user_account.js)

```javascript
    if (respondObj.info.username === username && respondObj.info.status === 1) {
        let message_type = respondObj.data_type;
        switch (message_type) {
            case 'web_finance': renderUserBalance(respondObj); break;
            case 'web_recharge': renderRecharge(respondObj); break;
            case 'web_vehicle_query': renderVehicleList(respondObj); break;
            case 'web_vehicle_register': renderVehicleRegister(); break;
            case 'web_vehicle_history': renderChart(respondObj); break;
            default: return;
        ...
```

In the code above we did not filter the data type at the entrance of the function, which will allow other data type entering the branch logics causing an access to the field of undefined (respondObj.info is undefined)

We changed the code to prevent other types entering this logic:

```javascript
    if (respondObj.data_type.startsWith('web')) {
        if (respondObj.info.username === username && respondObj.info.status === 1) {
            let message_type = respondObj.data_type;
            switch (message_type) {
                case 'web_finance': renderUserBalance(respondObj); break;
                case 'web_recharge': renderRecharge(respondObj); break;
                case 'web_vehicle_query': renderVehicleList(respondObj); break;
                case 'web_vehicle_register': renderVehicleRegister(); break;
                case 'web_vehicle_history': renderChart(respondObj); break;
                default: return;
            }
        }
        ...
```

**!!!!!!!!!!!!!!!!!!!!!!**

Controller:

+ query vehicle parking history

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

When a user wants to check last 7 days vehicle parking history,  controller will traverse database to find the corresponding "parking" history and add the parking fee to corresponding day. After that, controller will return this message and web will plot the result  to a  table.

*test communication*:

+ this  functionality works correctly when connecting web with desktop.

**!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!**



