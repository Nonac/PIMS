# System Design

##Contents
* [Architecture of the entire system](#_a)
    * [Schemetic diagram](#_diagram)
    * [Individual roles](#_roles)
* [Requirements of key sub-systems](#_b)
* [Object-Oriented design of key sub-systems](#_c)
* [The evolution of UI wireframes for key sub-systems](#_d)
* [Details of the communication protocols in use](#_e)

<a name="a"></a>
## Architecture of the entire system
<a name="diagram"></a>
### Schemetic diagram
![architecture](architecture.png)

There are 6 elements in this design:
* 1 broker - HiveMQ MQTT
* 3 online elements
    * Web application
    * Desktop application
    * Hardware - M5Stack
* 2 offline elements
    * Hardware - M5Stick
    * Local database
    
<a name="roles"></a>
### Individual roles
#### MQTT
HiveMQ MQTT is the mandatory broker given by the lecturers to connect the online elements, which are publishers
and/or subscribers. The messages published on MQTT can also be seen by going on
to its website.

#### Online elements
##### Web
The website is developed by Bootstrap and is designed for customers to access
their personal account. Once the customer
wishes to use this service, they should open an account with us and register their
vehicles on the website. The website also provides records of their usage histories.


##### Desktop application
The desktop application is developed by Processing. It is designed for managers
at the parking lots to monitor and manage the parking lot. It visualises the 
occupany of the parking lot and monitors the vehicles using this parking lot.

##### M5Stack
The M5Stack is a small IoT (Internet of things) device compatible with Arduino and
has built-in sensors (e.g. accelerometer, gyroscope), WIFI, bluetooth, 
colorful screen and buttons. In our design, it acts as the parking lot 
entrance/exit barrier. It is mounted at the parking lot entrance/exit.
The screen will display the barrier's status (open or close).

#### Offline elements
##### M5Stick
M5Stick comes with M5Stack and is even smaller than M5Stack. It also has a screen,
a button and bluetooth. In our design, the M5Stick acts as a key to open the
parking lot barrier (M5Stack). This means that the M5Stick is kept by the driver
in the car. During the driver's registration process, he/she must link the M5Stick's
bluetooth address with his/her PIMS account. When the driver wishes to enter/exit
the parking lot, he/she only needs to press the button on M5Stick
when the M5Stick and M5Stack (barrier) are in the vicinity.

##### Local database
There is a simple data storage system in this design since it is not the
focus of the project. The data of the customers' from out website are passed 
through MQTT and saved on the local hard disk in the manager's desktop. Each 
parking lot should have at one manager that keeps the application open all 
the time, so the customers' data can be saved on the hard disk.  

<a name="b"></a>
## Requirements of key sub-systems
### M5Stack (Barriers)
An M5Stack and an Arduino MKR WiFi 1010 make up a barrier at a car park. 
Each barrier has a unique id and a type (either 'in' or 'out'). Barriers of 
type 'in' are those which are located at the entrances of car parks, while 
'out' typed barriers are situated at the exits of parking lots.
Each barrier scans the Bluetooth (BT) devices in its vicinity every **five 
seconds解释原因** and reports their BT addresses, and their Received Signal Strength 
Indicators (RSSIs) to our server. Meanwhile, the barrier continuously listens
to commands sent from our server and hardware interrupts (button presses) 
triggered on the barrier itself to execute corresponding operations such as
lifting and descending.

A barrier must meet the following requirements:
1. Has a unique id.
2. Able to connect to the Internet via a WiFi access point.
3. Able to retrieve the BT address and the RSSI for every BT device that is
 detectable.
4. Able to perform (2) regularly (every 5 seconds) and package the information
 into a JSON object for a session of scanning.
5. Able to send the JSON object created in (3) to our server right after its
 creation.
6. Allowing remote control by receiving and executing commands sent from our
 server.
7. Allowing manual control by responding to hardware interrupts.

### M5Stick (Keys)
Each M5Stick-C is a key that is recognizable by any barrier in our system. 
These keys are used to identify registered users. 
Since our barriers seek BT devices, the keys are in essence BT advertising devices.

Following requirements must be satisfied for the keys:
1. Has a unique id.
2. Powered by rechargeable batteries so users can use them outside freely.
3. Can advertise itself as a BT device
4. The BT advertising state can be switched on and off for using and saving 
energy, respectively.

### Web application
Users use the web application to sign up/log in an account, register their information, and see their account status. In order to enable users to use the web application to manage their accounts smoothly, it must fullfil the following requirements:
#### Sign up and log in
* Allow users to sign up a new account -> inform the desktop app that there is a new user registration.
* Allow users to log in to have access to their own accounts -> validate username and password with desktop app and keep the logged status with cookies.
* Prevent the illegal access to user account page -> filter access by validating cookies.
#### User Information Registration
* Users can register a new vehicle with a key delivered to them -> inform the desktop app and re-render the vehicle list.
* Users can top up their balance -> inform the desktop app a recharge made by users and send re-render the balance value.
#### Display user info and statistical data
* Show the user's username, the vehicle they own, their account balance, and the vehicle they -> fetch user account info from the desktop app and render it on the account page.
* Display the statistical data of parking time in the last 7 days, showing in chart -> fetch a list of parking time and render the chart.

### Desktop application

Since this project is a parking management software prototype, we designed 
the requirements of the desktop client system with full consideration of 
different client usage situations. Due to the limitation of this project, 
we can only store the customer's data on local hard disk on the manager's desktop.
This makes the desktop app a bridge between the web app (customer end) and
the hardware side (the barriers). 

To elaborate, customers' registration data from the website must be sent through the 
broker to desktop app for storage and customers' enter request must also
be sent by the barrier (M5Stack) through the broker to the desktop app for
verification.

The above communication happens automatically at the back end, whereas the front
end is only for data visualisation so parking lot managers can closely monitor
the parking lot. 

#### Back end requirement

As mentioned above, the desktop back end acts as a bridge between web app
and the barriers, so the following points are required in the design.

* Subscribe and retrieve users' and their vehicles' registration information
 from MQTT (published by web app).
* Parse the messages from MQTT, separate them into specific topics (e.g.
user registration, vehicle registration, user account top up etc) and store 
them on the local hard drive as JSON files. 
* Pack up the locally stored data and publish onto MQTT per web app's request.
* Subscribe and retrieve users' enter/exit request at the parking lot
   from MQTT (published by M5Stack).
* Parse the messages from MQTT, separate them into specific topics, namely the
  entrance request and exit request, and store 
  them on the local hard drive as JSON files. Note that when the car enters
  the parking lot, the type "in" JSON file is generated, but it will be replaced
  by type "out" JSON file once the car leaves, for the "out" JSON file
   covers more abundant information (the time on exit).
* Send receipt to web app or M5Stack via MQTT after receiving their messages.
* After receiving the entrance request, the desktop app determines whether 
the bluetooth address with the highest signal strength corresponds to the 
registered car. If not, it continues to search for a bluetooth address with a 
slightly weaker signal strength until it find the registered car. According
 to the parking information of the car in the database to determine the 
 current parking status of the car, so as to determine whether the barrier can 
 be opened this time according to the comprehensive situation of the bar and
  the car. If the car is already in the garage, even if the car's entrance
signal is received, it will not repeatedly open the entry rod.
* The entrance and exit requests and the user's essemtial information are 
logically processed and used to modify and save the new user's information 
required to achieve a dynamic refresh. In this project, we need to use access
 times and user account balances for calculations so that the parking lot can
  be profitable. And this underlying methodological logic should be 
  implemented along with the data stored.
* Send control commands to the barrier. Into the desktop app display 
requirements above says that when using the remote barrier switch function, 
the post-desktop API should provide the interface to send data request 
packets for the switching barrier to the barrier via MQTT. When the car enters
 or exits the parking lot, the desktop must send a message to the M5Stack to 
 control the barrier's behaviour. After the barrier opens, a 5-second pause 
 is required to give the car a time to pass the barrier, and a 
  message to close the barrier is sent after 5 seconds.
* To provide all the data to be fetched and thus make a software back-end API.
 It is also the underlying arithmetic that the software should implement at
  the desktop app runtime. Various interfaces are provided for other functions.

#### Front end requirement
In the regular usage of the desktop app, the parking lot manager sits in the central 
control room of the parking lot and uses a desktop computer for the desktop app. 
So at the operational and display level, the desktop app should meet
 at least a few requirements.

* Real-time display of information on different vehicles passing through 
various barriers, with dynamic refreshing. This will be an essential feature
 of this software. As a management need, managers have the right and duty to
  know exactly who is driving what car and at what time to enter or leave that
   parking lot. This record should be kept locally for managers to view in
    real-time.
* Real-time display of the vehicles remain in the parking lot, along with
 necessary information about those vehicles. This is the second function 
 derived from the first one above. As a management system, it is, of course,
  essential to have a thorough understanding of the details of the vehicles 
  to be checked in the management area. For example, check the entry time of
   a vehicle in the parking lot, and if the parking time is too long and the 
   account balance is insufficient, then managers should be prepared to handle
    the vehicle when it is ready to leave the lot.
* Real-time display of used spaces in the parking lot as a percentage of all 
spaces in the form of a pie chart. This feature is designed on the fact that
 when the parking lot is busy during rush
 hour, there will be long lines of entering vehicles waiting to enter the lot. 
 If the managers are alerted before the parking lot is about to be filled, there
  will be plenty of time to clear the entrance without a large number of 
  vehicles clogging the parking lanes and causing potential traffic hazards.
* Real-time display of parking revenue. This feature is essential to
 the management system because parking lots operate based on
  customary charges for parking, so it is necessary for managers to understand
   how much they earn. This can be then checked against the revenue in the PIMS
   company bank account.
* The ability to manipulate the barrier. There are likely technical issues of broker
communication, verification process, or special occasions that the manager
has to lift up or lower the barrier themselves. 


<a name="c"></a>
## Object-Oriented design of key sub-systems
### Desktop app
![desktopAppUML](desktopApp_UML.png)

The basic object-oriented model for this project is shown in the figure. The
 DashboardView part of the picture is the view part of the MVM model. This 
 section is dominated by the build program view section, so it is mainly for
  the construction of the program's graphical interface, with buildUI 
  functions dominating. The Database section shows the Model section. The 
  GETS and RECEIVES functions are the main ones. where MessageType is the 
  important message storage abstract class that defines all data types. 
  Event is the main controller part. Mainly for the interaction of the 
  desktop app with the barrier and the interaction of the desktop app with 
  the web client. mqtt is also mainly applied in this part as a means of 
  communication.
  
### Web app


<a name="d"></a>
## The evolution of UI wireframes for key sub-systems
这里是要写我们一开始的设计然后在develop过程中怎么改进和演变到现在的design的。

<a name="e"></a>
## Details of the communication protocols in use
__(placeholder for bragging about the MQTT)__ <br>
__(placeholder for bragging about JSON)__
看这里！！
1. Rules for communication
   - All of message communicated must go through Broker.
   - Three-aspect application must connect to and subscribe the same topic on Broker ("PIMS").
   - Each session connected must be duplex. In other words, when sender send a message to receiver, it also expects to receive an validation message from receiver to tell it such request is valid or not.
   - In the connection between web and server, because of the feature of broker, (which is when one application subscribe a topic, it will receive all messages in this topic including message sending by itself),  using status to identify where message is from. status=2 means it is from web side and the server side will control status =1 and status =0 to show whether web request is valid. And status=1 means the request is valid and status=2 means request is failed.'


Since different parts of our system send information asynchronously, we 
decided to let them publish different JSON objects to the MQTT topic 'PIMS' 
but all with an attribute named 'data_type' to signal the receivers to pick 
up the right messages. <br>
We could have packaged them into a single JSON array and designated an index
 of that array for each recipient. However, that would have introduced another
  level of complexity for the JSON objects and reduced their overall 
  readability. After careful consideration, we abandoned this approach. 

### Iot
#### "m5_transmit"
Sender: __Barriers__ <br>
Receiver: __the Desktop App__ (currently acts as a server)

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

This JSON object contains information about the BT devices that were present during the last scan period and the barrier that sent this message. <br>
The keys are self-explanatory by their names.<br>
One thing worth mentioning is that each cell in the "bluetooth_devices" array corresponds to a single BT device and the size of this array is dynamic (equals to the number of devices that were detectable by that barrier in the last scan).

#### "m5_receive"
Sender: __the Desktop App__ <br>
Receiver: __Barriers__ 

```
{
	"data_type": "m5_receive", 
	"barrier_id": 12345,
	"op_code": "A" 
}
```

This JSON object acts as a command from our server to a specific barrier. <br>
"barrier_id" indicates which barrier should react to this command. <br>
"op_code" dictates what operation to perform, as defined in [Barrier_orders.h](/M5Stack_bluetooth_detector/Barrier_orders.h).

### Web app

[Data type](../../data_structure.json) used in the communications between this web application and the controller contains `web_login`, `web_register`, `web_vehicle_register`, `web_vehicle_query`, `web_finance`, `web_recharge` and `web_vehicle_history`.

All of the data is sent in the format of JSON. The queries sent by web app have the status code `2`, while the response from the controller has the status code `1` or `0`, represent success or failure respectively.

If the query result is fail (e.g. the username the user intended to register already exists, password validation failed, the M5 stick key has already been reigistered, or some other things went wrong), the controller will send a failure message. The failure messages are in a unified format:

```
{
	"data_type": "example", 
	"info": {
			"username": "lea_tong",
		    "status": 0
		}
}
```

The data_type field specifies the data type of the original query message that caused this failure while the username field shows the username that the original query message was made by, and the status field indicates that this is a failure message.

All messages received by the web application will be checked the ownership -- whether this message belongs to the current user.

#### "web_login"

**Query** (status = 2):

```json
{
	"data_type": "web_login",
	"info": {
			"username": "lea_tong",
			"password": "password",
		    "status": 2
		}
}
```

**Success** (status = 1)

```json
{
	"data_type": "web_login", 
	"info": {
			"username": "lea_tong",
		    "status": 1
		}
}
```

**Failure** (status = 0)

```json
{
	"data_type": "web_login", 
	"info": {
			"username": "lea_tong",
		    "status": 0
		}
}
```

Send a query message for validation of user login. Send query with status = 0 to broker and the desktop application will receive this message and return a response containing a status code showing whether the user login validation is successful. The field of username is used for logging cookies to keep the logged-in state.

#### "web_register"

**Query**

```json
{
	"data_type": "web_register",  
	"info": {
			"username": "lea_tong",
			"password": "password",
			"status": 2
		}
}
```

**Success**

```json
{
	"data_type": "web_register",  
	"info": {
			"username": "lea_tong",
			"status": 1
		}
}
```

Send this query to the broker to register a new account. The desktop application will return 1 if succeeded to register this new user while 0 if the user already exists. The username field in the response messages (success and failure) is used for web application checking whether this message is supposed to be received by the current user.

#### "web_vehicle_register"

**Query**

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

**Success**

```
{

	"data_type": "web_vehicle_register",   
	"info": {
			"status": 1,
			"username":"lea_tong"
		}
}
```

Register a new vehicle for current user. The desktop application returns status 1 and username if this registration for vehicle succeeds, otherwise will return the failure message.

#### "web_vehicle_query"

**Query**

```
{
	"data_type": "web_vehicle_query",    
	"info": {
		"username": "lea_tong",
		"status": 2
	}
}
```

**Success**

```
{
	"data_type": "web_vehicle_query",
	"info": {
		"username": "lea_tong",
		"vehicle_list": [
			{
				"vehicle_id": "example01",
				"vehicle_type": "car"
			},
			{
				"vehicle_id": "example02",
				"vehicle_type": "lorry"
			}
		],
		"status": 1
	}
}
```

The web application sends this query to fetch a list of vehicles that are owned by the current user.

If there is no vehicle owned by the user, the controller will send the success message with an empty array in vehicle_list field.

Get failure status if there is something goes wrong in the back-end.

#### "web_vehicle_history"

**Query**

```
{
	"data_type": "web_vehicle_history",
	"info": {
		"username": "lea_tong",
		"status": 2
	}
}
```

**Success**

```
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
		"status": 1
	}
}
```

Send this query to fetch a list of statistical data of one specific vehicle's history parking time in the last 7 days.

#### "web_finance"

**Query**

```
{

	"data_type": "web_finance", 
	"info": {
			"username":"lea_tong",
			"status": 2
		}
}
```

**Success**

```
{

	"data_type": "web_finance",
	"info": {
			"username":"lea_tong",
			"balance": 21331,
			"currency":"GBP",
			"status": 1
		}
}
```

To get the current user's balance in their account.

#### "web_recharge"

**Query**

```
{

	"data_type": "web_recharge", 
	"info": {
			"username":"lea_tong",
			"card_number":"326173173718",
			"pay_amount":"10",
			"status": 2
		}
}
```

**Success**

```
{

	"data_type": "web_recharge",  
	"info": {
			"username":"lea_tong",
			"balance": 21331,
			"currency":"GBP",
			"status": 1
		}
}
```

Send the query to broker so that the controller can receive this message to recharge for the user. If success, the controller will send back to the user the message containing the user's current balance and the type of currency.




## f. Details of the data persistence mechanisms in use

1. there are 10 types of data structure  totally but not all of them need to be stored in filesystem
2. only parking, web_register, web_vehicle_register and web_finance, 4 data types,  are stored in file system. all others can be got from or change content in  origin 4 data types.
3. for these 4 data type, its file is named as datatype+useid.json or datatype+vehicleid.json. And each of them is relatively independent
4. So, every time when we need to create a new one or update a existed one, we need to fresh database file system to make data persistent.
