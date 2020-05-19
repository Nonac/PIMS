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