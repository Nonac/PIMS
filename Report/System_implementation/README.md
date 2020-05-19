# System implementation

## Contents

<a name="_a"></a>

## Breakdown of project into sprints

This project began in February 2020 and ended by the end of May. 
We have set relatively reasonable work plans and sprint goals to. 
We created the GitHub repository for this project on February 6 and 
spent a few days after that to get the team familiar with the GitHub 
platform by completing small practical tasks. We then allocated specific 
time periods for the different 
components and review the plan after each sprint.

![Gantt](Gantt.png)
### Things to note down first
这一段需要重新看


As can be seen from the system design section, the desktop program not only
 acts as the main body of the desktop program but also a server
 for the web and barrier ends. So for the program development, back-end needs 
 to be implemented simultaneously with the web app and M5Stack development. The 
 process went relatively smoothly, as our development team was relatively 
 well-staffed, and the back-end developers could meet the design requirements
  of docking in both directions simultaneously. Because the desktop app 
  uses object-oriented programming ideas,
 it can be constructed separately from the development front-end. 
 
As the requirements were raised first, the data structure and the protocol 
for web-controller communication is mainly designed by the web developer, 
and the controller that deals with web requests is implemented according to
 the data structure. In other words, this part is directed by the data 
 structure that is designed based on the client requirements.

As the data structure may change during the web development and it is absolutely important to use a confirmed and determined data structure to direct the controller's implementation, there is a chronological order in the development of this part of our software -- web development first and controller logics for web must be implemented after the web logics finished.

For the web side (front-end), the process is logic-oriented. HTML pages and CSS styles are designed based on the built up JavaScript logics and communication protocol. 

For the server controller,  it needs to negotiate data structure and 
communication rules with web side as well as the hardware, before determining
 how to design its database. 

### Sprint 1: Initial design
___Objectives___

* Initial sketch of desktop app UI
* A rough JSON data structure satisfying all the requirements
* Simple web pages for running JavaScript  implemented with HTML and CSS

#### Desktop front end
We completed the preliminary design of the entire system prototype between February 24 and February 28. We determined the way MQTT communicated with each component and determined the default form of the data structure within the entire system. Of course, this data form is quite different from the final data structure used, but this prototype has formalized our engineering direction. Based on determining the system prototype and data structure, we completed the front-end UI sketch design for the desktop app between February 28 and March 5.
#### Desktop back end
The first part is to design the Json file format according to the needs of the product. Three Json structures with data type "m5_transmit", "m5_receive", and "parking" are designed to receive data, send data, and store parking records.Because the json format is actually the most important part, we must first determine the system requirements with the M5Stack part, and then unify the format
```
{
    "data_type": "m5_transmit",
    "barrier_info":{
        "barrier_id": 12345,
        "barrier_type": "in"
    },
    "bluetooth_devices":
        [{
            "bluetooth_address": "47:a9:af:d2:63:cd",
            "RSSI": -80
        }]
}
```
```
{
	"data_type": "m5_receive",
	"barrier_id": 12345,
	"op_code": "A"
}
```
```
{
			"data_type":"parking",
			"info": {
				"barrier_type": "out",
				"time_in": "2020-3-20-15-4-21",
				"time_out": "2020-3-20-18-4-21",
				"username": "lea",
				"barrier_id": 12345,
				"vehicle_id": "acdjcidjd",
				"vehicle_type":"car"
				}
}
```

#### Web
In this sprint the web developer designed the initial version of the data structure used for communication with controller, where 5 necessary data types are included. And we implemented the logics for login and register page.

`web_finance`, `web_recharge`, `web_login`, and `web_register` have the 
same name as the final version, while `web_vehicle`was split into 2 parts 
later. This first version was just for displayed because they are not well 
organized and unified.

Initial version of the web data types

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
            "access":0
		}
}

{

	"data_type": "web_vehicle",   
	"vehicle_info": {
            "user_id":"aaaaa",
			"vehicle_id": "acdjcidjd",
			"vehicle_type":"car"
		}
}
{

	"data_type": "web_finance",  
	"finance_info": {
            "user_id":"aaaaa",
			"balance": 21331,
			"currency":"GBP"
		}
}

{

	"data_type": "web_recharge",  
	"recharge_info": {
            "user_id":"aaaaa",
			"balance": 21331,
			"currency":"GBP",
            "card_number":"326173173718",
            "pay_amount":"10"
		}
}
```

In this sprint only the very simple HTML and CSS pages for login and registration were implemented, as the developer was not quite familiar with the Bootstrap and JavaScript yet.

### Sprint 2: Initial back-end development   
___Objectives___

* Negotiate and confirm final data structure for web
* Stable connection established via MQTT
* Module test for the functionality of web stand-alone system
* Server subscribes to, retrieve, parse, store and publish user and vehicle registration information from MQTT and complete the interaction with the web site.
* Server subscribes to, retrieve, parse, store and publish vehicle access information from MQTT and complete the interaction with the M2Stack end.
* Server requests for access and information about users are logically processed and used to modify and save information about new users and required to achieve dynamic refresh.
* Server receives messages from M5Stack.
* Server provides all the data needed to be captured, thus making a back-end API for the software.

#### Web
In this sprint we finally confirmed all data structure for web-controller communication, established the MQTT connection, and did module test for web front side.

For data schema, The changes in [data structure](../../data_structure.json) are quite significant.

Since all of message are transmitted at one topic "PIMS", so each side needs a mechanism to grab package belongs to it. At beginning, we believe just using data type is enough to identify. However, when we test it , we find that if subscribing a topic , a sender will also receive the message sent from itself.  Since each request needs two message, one query message from sender and one confirmed message from receiver, and in communication between web and server, each request's two messages have same data type, both server side and web side grab the package that should not belong to them. So, we need another identifier, which is status to identify whether it should grab. we rule that status =2 must be the message from web to server, and status = 1 and status =0 must be message from server to web, where 1 means query succeeds and 0 means query fails.  Such mechanism successfully solved above problem.



In addition, we added a data type `web_vehicle_history` for fetching the parking history and split the type `web_vehicle` into `web_vehicle_query` and `web_vehicle_register` to fetch vehicle list of the user and to register a new vehicle for the user.

The module test was carried out to see whether functions dealing with coming data can perform properly and correctly. The approach was by publishing messages in broker and observing the response on web pages. By the end of this sprint, we had tested for each data type and confirmed that can work in the stand-alone system of web application.
#### Desktop back end
We transmit messages through the MQTT protocol. Both the M5Stack part and the DeskTop part subscribe to the "PIMS" topic. The goal we need to accomplish is to receive the Json data from the M5Stack that stores the Bluetooth address and signal strength, and give feedback based on different data. First determine whether there is a registered car in Json. If there is, get the current car status from the database. Second, determine whether the car is inside or outside the parking lot. If the pole can be opened outside the parking lot, the pole can be opened inside the parking lot.According to the result of the second step, send a message to open or close the rod to M5Stack. After opening the lever, the system pauses for five seconds and then closes the pole.At the same time, the parking information of this car is generated or updated.

### Sprint 3: Development on front-end and further development on back-end 
___Objectives___
* The style sheets and dynamic rendering of front-end web pages
* Complete the functionality for in server controller side, including updating
customers' account balance.
* Decide which data types need to be stored in database and which do not.
* Electronic sketches for desktop app

#### Web and desktop controller
The controller logics for web was built in this sprint as the data structure for this part had already been confirmed. We also implemented the CSS style sheets for front-end web pages and tested the communication between web and the controller.

Since the functionalities in web side are almost done, we 
start controller's development. In this sprint, we finished all 
functionalities in server's controller, except vehicle history query. 

More specifically, the controller was now able to update the balance in customers'
account. When the car enters the parking garage, record the time of entry. When the car leaves the parking garage, the time of departure needs to be recorded. According to the difference between the time of entering and leaving, the user's parking cost is calculated, and the account balance is obtained from the database and updated.


And meanwhile we decided which of data objects need to be stored into 
file system of database. 

*Evaluation and testing*:

All functionalities developed so far work correctly when connecting web with desktop.


#### Desktop app

Since the previous design was only a paper operation, the first step we should complete the paper design to electronic prototyping excess. The difficulty with this step is that some of the components conceived initially on paper are inherently tricky to electronic. In addition to this, the electronic design script adds a colour element that allows the use of the desktop app to be perceived through colour changes.

##### Layout simple elements to the desktop UI

It is extremely dangerous to fully develop the front-end UI directly before the desktop app back-end is developed. So we take the approach of first developing the UI part of the desktop app that does not involve the underlying logic API.

* The first part we complete is the construction of the title and text section, which only relates to the colours and fonts. It is worth mentioning that we have chosen the font Berlin Sans FB Regular, which is superior to the system default font in terms of display effect.
* Next, we finished laying out the buttons and menus, but did not write the part that interacted with the API, just did some design and tweaking of the page layout based on the art.
* Once again, we have implemented the clock tab at the top of the page. Since this project is highly correlated with time, it was essential to design a clock at the top of the desktop app to display time. While implementing the clock tag, we completed the build of the desktop app refresh mechanism. This is a significant step for follow-up.
* Finally, we finished building the colour pattern. Since this button is a pure display function independent of the system architecture, it is also effortless to implement.

##### Building elements that interact with back-end APIs

When the desktop app backend API was almost done, we started developing the display part that had logical interaction with the backend API. We completed the construction of the following components in order.

* First, we completed the presentation of the vehicle's pass record. Here we call the M5Stack pass log data stream of the backend API. In chronological order, the first entry is the latest pass record, and the display is refreshed in a previously constructed refresh mode.
* Next, we completed the information display of all vehicles present. The data is also updated in real-time by refreshing the system.
* Third, we completed the presentation of the data visualization components. Taking advantage of the PROCESSING data visualization, we effectively represented the ratio of the number of vehicles present to the total number of parking spaces and the real-time revenue from the parking lot. Due to the limited time available for the presentation of this project, we have set the time limit for the refresh mechanism at 100 seconds after group discussion.
* Finally, we have finished manually controlling the M5Stack button in the desktop app, which is done by calling the function in event. Control of the barrier is completed by packaging the JSON data to be sent.

#### Sprint 4: Review, modify and finalise
*Objectives*

* System tests in 3 stages
* Functionality of querying parking history in the controller
We conducted a total of three tests, a single-part effect demonstration, a joint test with the web end and a joint test with M5Stack. The performance and reliability of the system are refined with each test. Compared to the first bug-filled test, the third test was perfect, working smoothly with the other two sets of parts through the MQTT.
* Server receives messages sent from M5Stack and debug the program.

##### Web
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



Controller:

query vehicle parking history is the most complicated parts for controller development since controller need to traverse database to find the corresponding "parking" history and add the parking fee to corresponding day. After that, controller will return this message and web will plot the result  to a  table.

*test communication*:

This  functionality works correctly when connecting web with desktop.
##### Desktop app
In this sprint we finalised the charts and plots on desktop UI to make sure
they are both easy to read and funtioning smoothly.

<a name="_b"></a>

## Technique justification and design evaluation
In this section, we will justify in details why we chose certain techniques
and their pros and cons during evaluations.
### Web
**Bootstrap4 and jQuery**

*Why do we choose them?*

Bootstrap4 is very friendly for beginners of web development because it provides diverse useful HTML and CSS components. Furthermore, Bootstrap4 is perfect in making responsive pages, and as the users might want to view their account info and top in on their mobile device, the responsive design will guarantee that the pages perform well in different scenarios.

jQuery helps to simplify the DOM manipulation, which means that we can choose any elements on a web page more easily. In our situation, it is enough to use jQuery for this lightweight web application.

*Limitation*

There is a limitation that the versions below 10 of Internet Explorer do not support bootstrap 4 because they do not support flex layout. This will cause some unknown problems or program crashing when the users use an IE browser to access to this application. This could potentially be unfriendly to those who use an old browser. However, as we should encourage the users to upgrade their browsers to phase out the old browsers with bad performance and bad supporting to new features, we did not consider using  Bootstrap 3 instead of 4 in this application, although it will have better compatibility towards lower version of browsers.

Directly manipulating on DOM tree is regarded as bad practice in programming nowadays, and programmers are encouraged to use frameworks that hide the DOM manipulation like React and Vue in front-end development. However, for this light-weight application, it seems unnecessary to put the so heavy frameworks in use. But one thing worth considering is that if we want to extend this application to a wider use, it would be better to use a framework such as Vue or React.

****

**Echarts**

*Why do we choose this?*

Echarts enables data visualization and chart dynamic rendering with several easy function calls. This helps us to draw a chart to show the parking time of users.

*Limitations*

To initialize an echarts object we have to provide the constructor function a DOM block element with specified height and width. However, in our responsive web design, the container for the chart has a percentage as height and width, which is not supported by the Echarts. To solve this, we use jQuery to set the width and height of the element dynamically at the stage of page loading, and this can give the element fixed values of length and width according to the actual size of it. 

But this brings another problem: after the jQuery setting a fix value for any attribute, the element keeps the value when even if the page size has been changed by user. This will cause the chart out of edge if the user zoom in the page. To fix this, a solution is to listen the element size using jQuery, however, this will cause a bad performance of the JavaScript logics, and it is not worth implementing because the user can refresh the page to make it a dynamic size.

### Desktop back-end
*Why do we use these [communication rules](../System_design/README.md#_e)?*

1. This design of data schema is different from that recommended by the lecturers.
 10 types of data make each json object is small, and each message do not have redundant information since each data type message has its own specified purpose. The three-sides applications grab the message belongs to them by identify data type and status. Such design reduce the size of package and improve the efficiency of communicating.
2. Duplex way makes our communication robust and crash-free. Since in every request, when a sender sends query message, it expects a confirmed message from receiver. So, even though our package get lost on Internet, our application will not be crashed, and sender will request again for response.

*Why do we choose this [data scheme](../System_design/README.md#_f)?*

1. Each type of json object finish one specified functionality, and no redundant data in each json object.
2. 4 out of 10 data type needs to be stored in database, not all of them, which can save the space for hardware.

#### Details of method implementations
##### 1. Find the registered car from the Bluetooth address list
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
##### 2. Generate new parking records.
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

##### 3. Calculate parking fees.
Here, we need to calculate the cost based on the parking time of the garage
recorded in the date. Therefore, we need to use all the information such 
as year, month, day, hour, minute and second to calculate the cost according
to the different time of parking. Because of the need for display, we set
the tariff to £1 per second, because the time for a testing is only in the scale
of seconds and minutes, instead of hours and dats. 
In this way the revenue chart can more intuitively display the changes in parking 
fees.
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
##### 4. Account register

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

##### 5 . Account login

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

##### 6. Check balance of an account 

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

##### 7. Topping up

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





##### 8. Vehicle register

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

##### 9. Query vehicle list

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

##### 10. Query vehicle parking history

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

#### Desktop front-end

Due to the independent and collaborative nature of desktop app front-end development, the evaluation of desktop app front-end is divided into the following two main aspects.

##### Evaluation and testing of separate components on the front end

This part of the component can run independently on the desktop app side without calling the underlying interface so that it can be tested separately.

*Clock*

![clock](./destop_App_Font-end/clock.png)

The clock text variable calls the system time, and success is marked by the ability to refresh the text with the refresh of the DRAW() function. In other words, the ability of the top clock label to display the system time properly is a successful evaluation criterion. The difficulty with this part is that replacing the clock tag text should be done in the DRAW() method, and not under our own constructed refresh mechanism. Moreover, it is using a content replacement approach rather than an overall replacement approach. This way, we can effectively avoid reporting errors.

*Colour Setting Menu*

The color shift menu is also a display component that does not involve the lower-level API, so the evaluation of the color shift can be done separately when the desktop app undergoes front-end development.The colour shift menu is also a display component that does not involve the underlying API, so the evaluation of the colour shift can be done separately when the desktop app undergoes front-end development. It is easy to understand that once the colour shift menu is used, the desktop application view can be changed from the original default dark colour mode to light colour mode. Of course, we can also go from light to dark mode.

This section is evaluated on the ability to successfully change the colour pattern of all components without reporting errors. Due to the peculiarities of PROCESSING, we have set the colour changing function in operation to control the successful use of this menu.We use a separate method to build the colour transform component. There is also no need to refresh using our own constructed refresh mechanism, but rather to refresh using the DRAW() function. The goal is to allow for faster system response and to avoid label redundancy due to duplicate component rendering.

##### Evaluation and testing of components on the front end that need to call the back end API

All components in this section must work with the API interface provided by the back-end in order to function correctly, so their testing must be done simultaneously with the back-end as well as the web and M5Stack ends in order to be adequately evaluated.

*New cars coming in*

When the barrier end receives a signal from an oncoming car, and the rear end successfully receives the incoming and outgoing logs and saves them on the local hard drive, the display end calls up all JSONArray to pick, sort, reorganize and display. Its evaluation of success is based on the front-end's ability to display the record correctly once the file has been received and saved by the back-end.

We require to arrange backwards, so the difficulty lies in how to sort JSONArray for specific fields. This, of course, falls under the category of algorithmic issues that are no longer discussed in this project.Unfortunately, due to the limitations of controlP5, it is difficult to build an excel-like table to display data details. We can only do our best to recreate the style of the table through the drop down menu. This is something we did not anticipate during the design session.

*Details of cars in the parking lot*

Similar to the above features, this component shows the remaining vehicles in the field. Therefore, it is also necessary to receive the queue of vehicles in the field and select the traffic record as in for display. The following highlights are the criteria for the successful evaluation of this component.

* Once a vehicle enters the parking lot, this component should refresh the record of that vehicle for a limited time and display it on the display end.
* Once a vehicle leaves the parking lot, this component should refresh to delete this vehicle record for a limited time.

In this component, the disadvantages of ControlP5 are even more pronounced. We had a hard time unifying the table for each menu list due to the different name lengths. To do this, we need to make a lot of less cost-effective function choices. So we didn't do that.


*Pie chart*

Data visualization is an advantage of Processing, and we have developed 
data visualization widgets using this feature. The pie chart shows the 
real-time occupancy of the parking spaces. For the purpose of displaying the occupancy
clearly, since we will only have 1 car in the lot at a time (because we only
have 1 M5Stick), the total number spaces in the parking lot is set to 10. 
 
Similar to the two lists above, the pie chart needs to be linked to a remote 
M5Stack. Two requirements need to be met for their successful evaluation.

* When a vehicle enters at the barrier end and is successfully recorded by
 the desktop application backend, the pie chart automatically refreshes the
  record to increase the percentage of vehicles present.
* When a vehicle leaves the barrier end and is successfully recorded by the
 desktop application backend, the pie chart automatically refreshes the 
 record, reducing the percentage of vehicles present.


*Line chart*

Similar to the above components, the line chart shows the parking lot of 
revenue for the day, with hourly interval on the x-axis. The chart should 
refresh every hour to reflect the revenue trend for the past hours of the day.

However, because the testing time only lasts for less than 1 hour, the unit of x-axis
is modified to seconds and the refresh frequency is also changed to every second.
Then, because the limitation of the space where the line chart is at, we can
only fit 100 seconds in the chart, which is a simple, memorizable number. After
100 seconds, the line chart will no longer refresh and reflect the real-time
revenue but all other charts of the UI are not affected

The following criteria can judge its success: when 
the barrier end detects the vehicle leaving the parking lot, the desktop 
program backend monitors the MQTT stream and stores the file to the local 
hard drive. The front-end calls the API interface for receivable 
calculations, which are finally reflected in a line chart that is 
dynamically refreshed in real-time.

The limitation of this design is that during testing, we need to ensure the 
activities all happen within 100 seconds when line chart is still refreshing.
After that we need to manually re-start the desktop app.

*Barrier control buttons*

This widget is the one that communicates directly with M5Stack, so the way he works is to call the underlying API event method to control the barrier by sending a specific data type to MQTT for M5Stack to receive.

The criteria for evaluating the success of this component can be judged in this way. We open the desktop app and M5Stack at the same time, click on the button of this component on the desktop, and the screen of M5Stack shows the barrier state when the control is complete by command. If the transformation is successful, it is evaluated as successful. The way in which the button is used to implement the functionality of this component is very appropriate and meets the expectations of modern humans for manipulating electronic devices. This is the same design as our previous one.

<a name="_c"></a>

## Social and Ethical implications

#### Data Security, privacy and data protection
Due to the time limitation on this project, the data are currently stored on
local hard disks of the manager who uses the desktop application. Because the 
desktop application is acting as a server, all the data from users will be 
processed by the desktop. This means that users' personal information is not
kept confidential at all at this stage. In the future, after deploying a more
comprehensive database, the users' personal data will not be easily visible
to the managers. In turn, the data should be completely protected and shield
from any visualisation of the employees.

#### Social benefits
With this technology, the time of paying before leaving a parking lot is saved.
More car can park in the lots during busy hours as the cars in the lots can leave
quicker. For the users, they can also easily review and keep track of their parking 
records.

#### Environmental impact
Less emission from cars queuing with their engine on to exit the parking lot.
Less usage of physical currencies at the transactions are payment machines.
Less public facilities to fix: without this system, there will be multiple payment
machines dotted around the parking lot and built inside the exit barriers, 
with this system, there will be no payment machines in the parking lot not 
at the exit. The only publically used facility now will be the M5Stack that
controlls the barriers.

#### Employment
The elimination of public facilities results in less maintenance requirements
thus potentially less employment. However, the higher demand of the keys 
(M5Sticks) will induce more employments for the mass production.