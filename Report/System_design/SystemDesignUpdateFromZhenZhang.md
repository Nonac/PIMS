# 1. System Design

## a. Architecture of the entire system

There is three-aspect development for a parking management system

+ Web development for vehicle owner (by bootstrap)
+ Desktop development for management system (by processing)
+ Hardware development for M5Stick and M5Stack

The three components of development need to work together as a whole set of system.  The following sections will give a overview about how each part designed and the communication protocols used to communicate.

#### A overview of application scenario:

After vehicle owner register for an account and car(s), our company will deliver a M5Stick to him/her.  The owner just need to type in blue-tooth address in web and put this IoT device into car. Every time, the car moves in or out parking, the M5Stick will automatically connect with M5Stack in gate by blue-tooth. Then, M5Stack send message to the Desktop, and receive an validation message to authorize  it action (open the gate or not).Users can easily top up parking credit, check account balance, query their car's parking history  on our website. The parking manager can access to a view of various information used for managing system and data analysis.

#### Motivation

 In the modern parkings, there are two main ways to allow car in or out parkings, swiping card and car license plate recognition. Compared with former, our IoT solution saved more time and improve the capacity of each gate.   Compared with latter, our IoT has higher accuracy. Thinking big, if we have ability to cooperate with automobile manufacturers to embed IoT device into cars, and meanwhile we have sufficient money to buyout existing parking management companies and build up new parkings all over the world to occupy market, we will build up ecosystem of parking and change the pattern in this niche industry.

## b. Object-Oriented design of key sub-systems

### IoT

IoT development includes two parts, M5Stick and M5Stack. M5Stick will be put into car and M5Stack is used as a sensor on gate. M5Stick only communicates with M5Stack by blue-tooth to notify the willing of car to move in or move out. M5Stack has an another responsibility which is  sending message to server to check whether this car (M5Stick) is registered and whether its user has enough balance to pay the parking fee.  M5Stack should have the functionality to allow manager from server view to have the authorization to open or close gate by force **(强制开关)**



### Desktop Application

<font color=red>关于这部分，请参阅1b_desktopApp.md 来自杨一楠的论述进行整合。--杨一楠</font>

The desktop application is developed by the thought of MVC (model, view and controller).  Model is used to handle our databases, which includes several types of json data structure. Controller is used to handle the message received from web and IoT device, update the changes to model and sent response message back to web or IoT.  For view, it filters the data from database  and build up a read-friendly interface  for mangers to monitor the status on parkings and data analysis.   



### Web Application

The web application provides a nice-looking interface for users to access. User can register an account, login with his/her account and bind one or more cars to his/her account. Also user can how many cars binded with his/her account and query the parking history of each car. In addition, user can check the balance status for his/her account and top up parking credit for his/her account.  The web application need to communicate with server's controller to update information for server side  and obtain information from server side.  

### Overall Design

**如果A 中的A overview of application scenario 与爽姐写的Intro重复，可以把 A overview of application scenario放到这里。 用来概括我们的整个系统如何运行**



## c.Requirements of Key Subsystems



### IoT

M5Stick(car)

<font color=red>关于这部分，请参阅1c_IoT.md 来自徐涛的论述进行修改。--杨一楠</font>

+ When M5Stick get close to M5Stack (gate), they must automatically build up real-time communication by blue-tooth. **详细说明在Part2 system implement？爽姐，是每部分的详细说明在system implement中的b. Details of how we evaluated our designs吧？**
+ M5Stick must let M5Stack know whether it wants to move in or move out the parking.
+ each M5Stick must have a unique ID to identify themselves and this ID allows server's controller to check which user is binded with it.

M5Stack(gate)

+ M5Stack need to subscribe the HiveMQTT topic,"PIMS" to build up real-time communication.
+ M5Stack need to send messages to  and receive messages from server by "PIMS" topic.
+ M5Stack need to capture the communication request from M5Stick and send message to server to obtain gate instruction (open or close, or do nothing).
+ M5Stack need to listen the message from server to allow gate forcedly opens or closes 



### Desktop Application

<font color=red>关于这部分，请参阅1c_desktopAPP.md 来自杨一楠的论述进行整合。--杨一楠</font>

+ Server need to subscribe the HiveMQTT topic,"PIMS" to build up communication with web side and IoT side. 
+ Server's model need to build up database for the whole system, including 10 types of json data structure. 
+ Server's controller need to handle all the communication with web and IoT.
+ Server's controller need to update database when a new user account or a new car registered or a user top up parking credit or a parking fee is charged and send receipt message back to web or IoT. **也就是， 当新用户注册，用户注册新小车，用户充值，停车费用扣除时，需要更新数据库，并发送回执单给web或者iot端**
+ Server's controller need to search database when M5Stack (gate) queries for gate instruction or a user wants to login in or a user want to check account balance or query how many cars binded with this account or query for parking history. **也就是， 当用户登录，用户查询余额，用户查询自己绑定了多少车，以及某一辆车的停车记录时，需要遍历数据库，并发送回执单给web或者iot端。**
+ Server's view need to filter the database and obtain the useful data to build up nice-looking interface for manager to show the current status of parking such as how many cars parked in the parking, **the in and out time for each cars**, total revenue for this day and **so on**.**还有哪些功能？**



### Web application

+ Web application need to subscribe the HiveMQTT topic,"PIMS" to build up communication with server.
+ Web application need to provide a nice-looking interface to users.
+ Web application need to enable users to register accounts and register for cars.
+ Web application need to enable users to login with username and password and maintain the login status by setting cookies.
+ Web application need to enable users to check account balance and top up.
+ Web application need to enable users to query the information of binded car and parking history for each car. 





## e. Details of the communication protocols in use

<font color=red>关于这部分，请参阅1e_IoT.md 来自杨一楠的论述进行整合。--杨一楠</font>

#### 1. General rules

+ communication between M5Stick (car) and M5Stack (gate) goes through blue-tooth, and all of other communication goes through Broker.
+ Three-aspect applications must connect to and subscribe the same HiveMQTT topic,"PIMS".
+ There are 10 different type of json data structure used for different purposes. The data types of them are "m5_transmit", "m5_receive", "parking", "web_register",  "web_login", "web_vehicle_register",  "web_vehicle_query", "web_vehicle_history", "web_finance" and "web_recharge". The details of their purposes and usages are present in System Implementation. **是这部分吗? 加一个链接**
+ Applications use data type and status (some of them have) to identify the message needed them to deal with. 
+ Each session connected must be duplex. In other words, when sender send a message to receiver, it also expects to receive an callback message from receiver.

#### 2. JSON data format

+ **m5_transmit**  is sent from M5Stack to Server and "RSSI" is received signal strength indication. The closer the value is to 0, the stronger the received signal has been.

```json
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

+ **m5_receive** is sent from Server to M5Stack (gate) and "op_code": "A" means open the gate and "op_code": "B" means close the gate

```json
{
	"data_type": "m5_receive",   
	"barrier_id": 12345,
	"op_code": "A" 
}
```

+ **parking** stores detailed information for a parking record.

```json
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

+ **web_register** is used to register for a new account. and status here is used as a flag. 
  + status = 2 means this message is sent from web to server, 
  + status = 1 means this message is from server to web and last query from web side is valid.
  + status = 1 means this message is from server to web and last query from web side is invalid.
  + this rule of status is universally valid for all json object used "status" key. 

```json
{
	"data_type": "web_register", 
	"info": {
			"username": "lea_tong",
			"password": "*******",
			"status": 2
		}
}
```

+ **web_login** is just used for run-time communication and is not stored in database system.

```json
{
    "data_type": "web_login"
    "info": {
    "username": "lea_tong",
    "password": "*******",
    "status": 2
	}
}
```

+ **web_vehicle_register** is similar to **web_register**, but it registers for vehicles, not accounts.

```json
{
    "data_type": "web_vehicle_register",  
    "info": {
        "username":"lea_tong",
        "vehicle_id": "acdjcidjd",
        "vehicle_type":"car",
        "status": 2,
        "bluetooth_address" : "47:a9:af:d2:63:cd"
    }
}
```

+ **web_vehicle_query** is used to check how many cars a user have registered for an account

```json
{
    "data_type": "web_vehicle_query",
    "info": {
        "username": "lea_tong",
        "vehicle_list": [
            {
                "vehicle_id": "A007",
                "vehicle_type": "car"
            },
            {
                "vehicle_id": "AOO8",
                "vehicle_type": "lorry"
            }
        ],
        "status": 2
    }
}
```

+ **web_vehicle_history** is used for user to check the one car's consumption record. the key "0" to "6" means today to last 6 days, and the value of these keys means the expense amount for each day.

```json
{
	"data_type": "web_vehicle_history",
	"info": {
		"username": "lea_tong",
		"vehicle_id": "A007",
		"0": 4.3,
		"1": 15,     
		"2": 10.9,
		"3": 11.4,
		"4": 18.7,
		"5": 0,
		"6": 7,
		"status": 2
	}
}
```

+ **web_finance** is used to store the balance of an account.

```json
{
	"data_type": "web_finance",  
	"info": {
			"username":"lea_tong",
			"balance": 21331,
			"currency":"GBP",
			"status": 2
	}
}
```

+ **web_recharge** is used for user to top up his/her parking credit.

```json
{
    "data_type": "web_recharge", 
    "info": {
        "username":"lea_tong",
        "balance": 21331,
        "currency":"GBP",
        "card_number":"326173173718",
        "pay_amount":"10",
        "status": 2
    }
}
```



## f. Details of the data persistence mechanisms in use

1. There are 10 types of data structure totally but not all of them need to be stored in file system.
2. Only parking, web_register, web_vehicle_register and web_finance, these 4 data types,  are stored in file system. the other 6  can be derived from or change content in original 4 data types.
3. for these 4 data type, its file is named as datatype+useid.json or datatype+vehicleid.json. And each of them is relatively independent
4. So, every time when we need to create a new one or update a existed one, we need to fresh database file system to make data persistent.

# 2. System implementation

## a. Breakdown of project into sprints

+ Tao is responsible for M5Stick development.
+ Fuzhou is responsible for web development
+ Shuang, Yinan, Hongjie and Zhen is for desktop development
  +  Shuang and Yinan work on view development.
  + Hongjie and Zhen work on model and controller.
+ how to show our team work and pair programming~

## b. Details of how we evaluated our designs

1. Data structure 

+ each jsonobject has two attribute "data_type" and "info". data_type refers to the aim of this message and is used by all of applications to check whether they need to pick up this package. info refers to a sub jsonobject which include the information that applications need to handle.

2. Details in session between web and desktop controller.
   + Again. In the connection between web and server, because of the feature of broker, (which is when one application subscribe a topic, it will receive all messages in this topic including message sending by itself),  using status to identify where message is from. status=2 means it is from web side and the server side will control status =1 and status =0 to show whether web request is valid. And status=1 means the request is valid and status=2 means request is failed.'
   + functionality finished
     + web_register 
       + web side send message with "data_type" = "web_register", username, password and status=2
       + server side compares the coming message with database to check whether this username does not exist and send message back with status=0 or status=1;
     + web_login
       + web side send message with "data_type" = "web_login", username, password and status=2
       + server side compares the coming message with database to check whether this username does exist and password is correct  and send message back with status=0 or status=1;
     + web_vehicle_register and web_vehicle_query
       + after users registers account and then they need to register his vehicle in web
       + it is similar to web_register and web_login. But the different is that one user can register for one or many vehicles. web_vehicle query will return an embedded sub-jsonarray.
     + web_finance and web_recharge
       + web_finance is used to check user's balance
       + web_recharge is used to top up 
     + web_vehicle_history
       + users can specify one of their car and check the last 7 days parking history, how much parking costs. 







