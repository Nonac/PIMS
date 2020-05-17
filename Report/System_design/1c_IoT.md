## Requirements of key sub-systems
### Barriers
An M5Stack and an Arduino MKR WiFi 1010 make up a barrier at a car park. Each barrier has a unique id and a type (either 'in' or 'out'). Barriers of type 'in' are those which are located at the entrances of car parks, while 'in' typed barriers are situated at the exits of parking yards.
Each barrier scans the Bluetooth (BT) devices in its vicinity every five seconds and reports their BT addresses and their Received Signal Strength Indicators (RSSIs) to our server. 
Meanwhile, the barrier continuously listens to commands sent from our server and hardware interrupts (button presses) triggered on the barrier itself to execute corresponding operations such as lifting and dropping.

A barrier must meet the following requirements:
1. Has a unique id.
2. Able to connect to the Internet via a WiFi access point.
3. Able to retrieve the BT address and the RSSI for every BT device that is detectable.
4. Able to perform (2) regularly (every 5 seconds) and package the information into a JSON object for a session of scanning.
5. Able to send the JSON object created in (3) to our server right after its creation.
6. Allowing remote control by receiving and executing commands sent from our server.
7. Allowing manual control by responding to hardware interrupts.

### Keys
Each M5Stick-C is a key that is recognizable by any barrier in our system. These keys are used to identify registered users. 
Since our barriers seek BT devices, the keys are in essence BT advertising devices.

Following requirements must be satisfied for the keys:
1. Has a unique id.
2. Powered by rechargeable batteries so users can use them outside freely.
3. Can advertise itself as a BT device
4. The BT advertising state can be switched on and off for using and saving energy, respectively.
