# System Desion
# 1.c Requirements of key sub-systems
## Desktop System Requirements

Since this project is a parking management software prototype, we designed the requirements of the desktop client system with full consideration of different client usage situations. Because the desktop client is the bridge between the web client and the barrier end, all data is also received, calculated, stored and sent through the logic of the desktop client, so we think the desktop client should be divided into two main requirements, namely the desktop display management requirements and the desktop side of the back-end communication storage requirements.

### desktop display management requirements


In the casual use environment, the parking lot manager sits in the central control room and uses a desktop computer to operate this desktop management system. So at the operational and display level, the desktop app should meet at least a few requirements.

* Real-time display of information on different vehicles passing through various barriers, with dynamic refreshing. This will be an essential feature of this software. As a management need, managers have the right and duty to know exactly who is driving what car and at what time to enter or leave that parking lot. And this record should be kept locally for managers to view in real-time.
* Real-time display of how many vehicles remain in the parking lot, along with necessary information about those vehicles. This is the second function derived from the first one above. As a management system, it is, of course, essential to have a thorough understanding of the details of the vehicles to be checked in the management area. For example, check the entry time of a vehicle in the parking lot, and if the entry time is too long and the account balance is insufficient, then managers should be prepared to handle the vehicle when it is ready to leave the lot.
* Real-time display of used spaces in the parking lot as a percentage of all areas. This demand is built into the fact that when the parking lot is in rush hour, there will be long lines of entering vehicles waiting to enter the lot. If managers are alerted before the parking lot is about to be filled, there will be plenty of time to clear the entrance without a large number of vehicles clogging the parking lanes and causing hidden traffic hazards.
* Real-time display of parking revenue. Of course, this need is essential to the management system. It is well known that parking lots operate based on customary charges for parking, so it is necessary for managers to understand how much they earn, just like the double verification of the cash register.
* The ability to remotely manipulate the barrier. As shown in the hypothetical scenario above, it is highly likely that managers will use the desktop app in a central control room away from the barrier, so completing the ability to operate the barrier on the software is very necessary. For example, in some special conditions when we have to release the vehicle manually, the remote operation will significantly facilitate the work of the managers.

### desktop side of the back-end communication storage requirements

As mentioned in the IoT design requirements section, the desktop client also carries the role of web client and barrier end bridge, so in this part of the requirements, we mainly designed the following points.

* Receive registration information from web-side users and store the user information on the local hard drive. According to the logic of the web client, the registration section is divided into two main items as follows.
	- User Registration
	- Vehicle registration

	The desktop client parses user information and vehicle information separately by receiving registration packets separated from the MQTT stream, which are then stored on the local hard disk in a disaggregated fashion. At the same time, once the web client requests the relevant registration information, the desktop client also packages this information and sends it to the web client by way of MQTT.

* Receive information from the barrier end about vehicles entering and leaving, and save the information to the local hard drive. All the logic of this project is implemented based on the dynamic behaviour of the vehicle entering and leaving, and it is particularly important that all the information about the vehicle entering and leaving be stored locally. Barrier receives the data and sends the information about the entry and exit packaged by way of MQTT to the desktop client, which needs to parse the data and store it on the local hard disk for subsequent use in various ways.
Disk storage is used to simulate database access. Each time data is retrieved, the in-memory database is refreshed so that the latest data can be obtained every time.
* After receiving the message, the desktop need to determine whether the Bluetooth address with the highest signal strength corresponds to the registered car. If not, continue to search for a Bluetooth address with a slightly weaker signal strength until it find the registered car. According to the parking information of the car in the database to determine the current parking status of the car, so as to determine whether the bar can be opened this time according to the comprehensive situation of the bar and the car. If the car is already in the garage, even if the car's entrance
signal is received, it will not repeatedly open the entry rod.
* The incoming and outgoing information and the user's necessary information are logically calculated and used to modify and save the new user's information required to achieve a dynamic refresh. In this project, we need to use access times and user account balances for calculations so that the parking lot can be profitable. And this underlying methodological logic should be implemented along with the data stored.
* Send control commands to the barrier. Into the desktop app display requirements above says that when using the remote barrier switch function, the post-desktop API should provide the interface to send data request packets for the switching barrier to the barrier via MQTT.When the car enters or exits the parking lot, the desktop must send a message to the rod to control the opening and closing of the rod. After the lever is opened, a 5 second pause is required to give the car a time to pass the lever, and a message to close the lever is sent after 5 seconds.
* To provide all the data to be fetched and thus make a software back-end API. This is also the underlying arithmetic that the software should implement at the desktop app runtime. Various interfaces are provided for other functions.
