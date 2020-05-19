# Desktop App

This part is about the backend of desktop app to deal
with the message from M5Stack and to M5Stack. 

**Sprint1:Design of Json file structure**

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

**Sprint2:Receive the message from M5Stack**

We transmit messages through the MQTT protocol. Both the M5Stack part and the DeskTop part subscribe to the "PIMS" topic. The goal we need to accomplish is to receive the Json data from the M5Stack that stores the Bluetooth address and signal strength, and give feedback based on different data. First determine whether there is a registered car in Json. If there is, get the current car status from the database. Second, determine whether the car is inside or outside the parking lot. If the pole can be opened outside the parking lot, the pole can be opened inside the parking lot.According to the result of the second step, send a message to open or close the rod to M5Stack. After opening the lever, the system pauses for five seconds and then closes the pole.At the same time, the parking information of this car is generated or updated.

**Sprint3:Update account balance**
When the car enters the parking garage, record the time of entry. When the car leaves the parking garage, the time of departure needs to be recorded. According to the difference between the time of entering and leaving, the user's parking cost is calculated, and the account balance is obtained from the database and updated.


**Sprint4:Combine with M5Stack part to debug the code**
Receive messages sent from M5Stack and debug the program.