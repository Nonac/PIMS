## Web Application & Controller Logics for Web

As the requirements were raised first, the data structure and the protocol for web-controller communication is mainly designed by the web developer, and the controller that deals with web requests is implemented according to the data structure. In another word, this part is directed by the data structure that is designed based on the requirements.

As the data structure may change during the web development and it is absolutely important to use a confirmed and determined data structure to direct the controller's implementation, there is a chronological order in the development of this part of our software -- web development first and controller logics for web must be implemented after the web logics finished.

For the web side (front-end), the process is logic-oriented. HTML pages and CSS styles are designed based on the built up JavaScript logics and communication protocol. 

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

**Sprint 2** (Apr 6th - Apr 7th)

_Objectives_

* Negotiated and confirmed final data structure for web
* Stable connection established via MQTT
* Module test for the functionality of web stand-alone system

*Approach & Implementation*

In this sprint we finally confirmed all data structure for web-controller communication, established the MQTT connection, and did module test for web front side.

The changes in [data structure](../../data_structure.json) are quite significant, in which we added a data type `web_vehicle_history` for fetching the parking history and split the type `web_vehicle` into `web_vehicle_query` and `web_vehicle_register` to fetch vehicle list of the user and to register a new vehicle for the user.

The module test was carried out to see whether functions dealing with coming data can perform properly and correctly. The approach was by publishing messages in broker and observing the response on web pages. By the end of this sprint, we had tested for each data type and confirmed that can work in the stand-alone system of web application.

**Sprint 3** (Apr 8th - Apr 10th) ***!!!!!!!!!张震在里面这加你的第一部分内容 因为你是在个时间段实现了除了history之外的功能 然后你还得再说一下你在什么时间实现了hisroty得功能 以及为什么这时候不能实现history得功能***

*Objectives*

* The style sheets and dynamic rendering of front-end web pages



*Approach & Implementation*

The controller logics for web was built in this sprint as the data structure for this part had already been confirmed. We also implemented the CSS style sheets for front-end web pages and tested the communication between web and the controller.

*controller*:

*test communication*:



**Sprint 4** (Apr 30th - May 7th)

*Objectives*

* System tests in 3 stages

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





