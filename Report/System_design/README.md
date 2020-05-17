# System Design

## a. Architecture of the entire system
![architecture](architecture.png)

There is three-aspect development for a parking management system

+ Web page development (for customers) by bootstrap
+ Desktop application development (for managers) by processing
+ Hardware development: M5Stack (for barrier control) and M5Stick (key) 

Context of use:

After vehicle owner registers for account, our company will deliver a M5Stack for him/her.  The owner just need to type in blue-tooth address in web and put it into car. Every time, the car moves in or out parking, the M5Stack will automatically connect with M5Stick in gate by blue-tooth. Then, M5Stick send message to the Desktop, and receive an validation message to decide it action.







## e. Details of the communication protocols in use



1. Rules for communication
   - All of message communicated must go through Broker.
   - Three-aspect application must connect to and subscribe the same topic on Broker ("PIMS").
   - Each session connected must be duplex. In other words, when sender send a message to receiver, it also expects to receive an validation message from receiver to tell it such request is valid or not.
   - In the connection between web and server, because of the feature of broker, (which is when one application subscribe a topic, it will receive all messages in this topic including message sending by itself),  using status to identify where message is from. status=2 means it is from web side and the server side will control status =1 and status =0 to show whether web request is valid. And status=1 means the request is valid and status=2 means request is failed.'

## f. Details of the data persistence mechanisms in use

1. there are 10 types of data structure  totally but not all of them need to be stored in filesystem
2. only parking, web_register, web_vehicle_register and web_finance, 4 data types,  are stored in file system. all others can be got from or change content in  origin 4 data types.
3. for these 4 data type, its file is named as datatype+useid.json or datatype+vehicleid.json. And each of them is relatively independent
4. So, every time when we need to create a new one or update a existed one, we need to fresh database file system to make data persistent.

# 2. System implementation

## a. Breakdown of project into sprints

+ Tao is responsible for M5Stack development.
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
