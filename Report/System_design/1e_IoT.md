## Details of the communication protocols in use
__(placeholder for bragging about the MQTT)__ <br>
__(placeholder for bragging about JSON)__

Since different parts of our system send information asynchronously, we decided to let them publish different JSON objects to the MQTT topic 'PIMS' but all with an attribute named 'data_type' to signal the receivers to pick up the right messages. <br>
We could have packaged them into a single JSON array and designated an index of that array for each recipient. However, that would have introduced another level of complexity for the JSON objects and reduced their overall readability. After careful consideration, we abandoned this approach. 


## "m5_transmit"
Sender: __Barriers__ <br>
Receiver: __the Desktop App__ (currently acts as a server)

```
{
    "data_type": "m5_transmit",
    "barrier_info":{
        "barrier_id": 12345,
        "barrier_type":"out"
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

## "m5_receive"
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
