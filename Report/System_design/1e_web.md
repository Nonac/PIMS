# Communication Protocol Used by Web

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

## "web_login"

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

## "web_register"

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

## "web_vehicle_register"

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

## "web_vehicle_query"

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

## "web_vehicle_history"

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

## "web_finance"

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

## "web_recharge"

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

