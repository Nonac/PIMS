# Desktop_app
This part is about communication between desktop app and m5stack. The aim is to
receive message from M5 Stack and send message to M5 Stack. All data is stored
in Json objects using by using different data types.

## Receiving message from M5Stack
The m5_stack will send “m5_transmit” Json object to the desktop_app when there
are cars near the barrier. There may be a lot of Bluetooth signals near the
barrier, such as the owner's mobile phone, headphones, etc., so we have to
choose the highest intensity from the list of Bluetooth signals. However,there
are some tricky situations such as there are many other Bluetooth devices and
many cars near the barrier. Therefore, we should sort the Bluetooth addresses
and select the one with the strongest signal that has been registered. Even if
some Bluetooth signals are very strong, it is invalid if they are not registered
in the website.
```
{
	"data_type": "m5_transmit",
	"barrier_info": {
		"barrier_id": 12345,
		"barrier_type":"in"
	},
	"bluetooth_devices": [
		{
			"bluetooth_address": "07:9c:99:32:75:ab",
			"RSSI": -91
		},
		{
			"bluetooth_address": "15:e4:ac:a9:36:a2",
			"RSSI": -52
		},
		{
			"bluetooth_address": "4f:fc:d0:83:19:f0",
			"RSSI": -79
		},
	]
}
```
## Calculate parking charges
After finding the registered car in the database, the controller must first
determine whether the car is already in the garage. If the car is already in the
garage, it means that it is out of the garage this time, otherwise it is
entering the garage. This involves the second date type "parking". This is
mainly used to record the time when the car enters and exits, as well as the car
ID and other information. Whether there is “in” type data for each search, if
it exists, it means that the car is in the garage, and controller should change
“in” to “out” and generate the time of “time_out”. If it does not exist,
create a data with barrier_type “out” and generate the time of “time_in”. When
the car leaves the garage, it looks up the finance status in the database
based on the username, and calculates the current charge based on the difference
between the time the car exits and enters, and updates the finance status.We calculate the parking fee at a price of one pound per second, this does not
represent the real situation, just for the convenience of demonstration
```
{
"data_type":"parking",   //parking info
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
```
{
"data_type": "web_finance",  // check the balance
"info": {
    "username":"lea_tong",
    "balance": 21331,
    "currency":"GBP",
    "status": 2
  }
}
```
## Send message to M5Stack
The last part is to send a json object of type "m5_receive" from the desktop app
, which allows the desktop to actively control the switch of a certain barrier
to deal with some special situations.In addition, when a car enters or exits, it
 also sends a signal to control the entry and exit of the lever. The first is to
send the open bar information, after a delay of 10 seconds, send the closed
bar information. The 10-second delay is a simulation of the time it takes for
the car to enter and exit.
```
{
"data_type": "web_finance",  // check the balance
"info": {
    "username":"lea_tong",
    "balance": 21331,
    "currency":"GBP",
    "status": 2
  }
}
```
