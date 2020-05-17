# PIMS - Parking Iot Management System

## Contents
* [Group memebr list](#_group)
* [Introduction](#_intro)
* [Portfolio](#_portfolio)
    - [System Design](#_design)
    - [System Implementation](#_implementation)
    - [Project Evaluation](#_evaluation)
* [Pitch (Video demonstration)](#_video)

<a name="_group"></a>

## Group members
* __Fuzhou Wang__ (Web)\
  cy19158@bristol.ac.uk
* __ao Xu__ (M5Stack & M5Stick)\
  si19010@bristol.ac.uk
* __Hongjie Yuan__ (Desktop back end)\
  ii19142@bristol.ac.uk
* __Zhen Zhang__ (Desktop back end)\
  ad19014@bristol.ac.uk
* __Yinan Yang__ (Desktop front end)\
  ff19085@bristol.ac.uk
* __Shuang (Lea) Tong__ (Desktop front end)\
  nj19182@bristol.ac.uk

<a name="_intro"></a>

## Introduction
Parking Iot Management System is designed for easy access to public
parking lots. It is like a toll-pass (FasTrak or E-ZPass) in the US 
or ETC in China, but used on parking lots.
Each driver wishing to use this system needs to obtain a
key (M5Stick) and register their personal and vehicle information on
the PIMS website. With a key in hand, the driver can park in any
supporting parking lot. All they need to do is to press the key (Stick)
to enter, and press to exit. The barrier, controlled by M5 Stack, receives
the request from the Stick together with the Stick's bluetooth address, and 
verifies the user's account via the broker
(MQTT). If successful, the barrier (Stack) will lift up to let the car 
in/out. The system will track the car's parking time and calculate with the 
tariff of the parking lot's, then charge the payment from user's account.

With this system, the drivers can save the time of paying at the machine.
With cars leaving the parking lot quicker, more cars are able to park in
the lot during a busy day.

<a name="_portfolio"></a>

## Portoflio

<a name="_design"></a>

### 1. System Design
[Click here](Report/System_design/README.md) to see the document for:\
a. Architecture of the entire system\
b. Object-Oriented design of key sub-systems\
c. Requirements of key sub-systems\
d. The evolution of UI wireframes for key sub-systems\
e. Details of the communication protocols in use\
f. Details of the data persistence mechanisms in use\
g. Details of web technologies in use

<a name="_implementation"></a>

### 2. System implementation
[Click here](Report/System_implementation/README.md) to see the document for:\
a. Breakdown of project into sprints\
b. Details of how we evaluated our designs\
c. Discussion of Social and Ethical implications of the work

<a name="_evaluation"></a>

### 3. Project Evaluation
[Click here](Report/Project_evaluation/README.md) to see the document for:\
a. Reflective discussion of the success of the project\
b. Discussion of future work\
c. Reflection on the working practices of our group\
d. How Coronavirus has affected our project

<a name="_video"></a>

## Pitch
Here is the video of us demonstrating how the M5Stack (barrier)
, M5Stick (key), desktop application (for managers) and webpage (for customers) 
work together via the communication on MQTT.

[![Video thumbnail](Report/Video%20thumbnail.png)](https://youtu.be/KG43DJtVy-I "PIMS")
