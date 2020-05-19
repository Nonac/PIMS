[![codebeat badge](https://codebeat.co/badges/3307228a-f684-4d27-9665-0ccef96cbdef)](https://codebeat.co/projects/github-com-nonac-pims-master)
![](https://img.shields.io/badge/language-JavaScript-blue.svg)
![](https://img.shields.io/badge/language-HTML-blue.svg)
![](https://img.shields.io/badge/language-C++-blue.svg)
![](https://img.shields.io/badge/language-Processing-blue.svg)
![](https://img.shields.io/badge/platform-M5Stack|PC|Web-lightgrey.svg)
![](https://img.shields.io/cocoapods/l/Alamofire.svg?style=flat)
![Logo](Report/logo.png)
![Hi](Hi.png)





*Are you tired of queuing at the payment machine in the parking lot?*\
*Are you annoyed some payment machines only take cash?*\
*Are you frustrated that you left the ticket in the car and have to walk all that distance twice?*\
*Are you annoyed someone exiting the parking lot lost their ticket so the lane is clogged?*\
*Do you sometimes go on the curb accidentally just because you want to stop closer to the barrier ticket machine?*\
*Are you unhappy that the automatic number-plate recognition doesn't always work?*\
*Do you have too many parking membership cards for different parking lots?*\
*Have you ever found or get refunded on a lost parking membership card?*


***PIMS, solves it all!***

## Contents
* [Introduction](#_intro)
* [Report](#_portfolio)
* [Pitch (Video demonstration)](#_video)

<a name="_intro"></a>
## Introduction
Parking Iot Management System is designed for easy access to public
parking lots. It is like a toll-pass (FasTrak or E-ZPass) in the US 
or ETC in China, but used on parking lots.
Each driver wishing to use this system needs to obtain a
key (M5Stick) and register their personal and vehicle information on
the PIMS website. With a key in hand, the driver can park in any
participating parking lot. All they need to do is to press the key (Stick)
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

## Report

### [1. System Design](Report/System_design/README.md)
### [2. System implementation](Report/System_implementation/README.md)
### [3. Project Evaluation](Report/Project_evaluation/README.md)

<a name="_video"></a>

## Pitch
Here is the video of us demonstrating how the M5Stack (barrier)
, M5Stick (key), desktop application (for managers) and webpage (for customers) 
work together via the communication on MQTT.

[![Video thumbnail](Report/Video%20thumbnail.png)](https://youtu.be/kaCjAmnIsRY "PIMS")
