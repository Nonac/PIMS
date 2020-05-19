[![codebeat badge](https://codebeat.co/badges/3307228a-f684-4d27-9665-0ccef96cbdef)](https://codebeat.co/projects/github-com-nonac-pims-master)
![](https://img.shields.io/badge/language-JavaScript-blue.svg)
![](https://img.shields.io/badge/language-HTML-blue.svg)
![](https://img.shields.io/badge/language-C++-blue.svg)
![](https://img.shields.io/badge/language-Processing-blue.svg)
![](https://img.shields.io/badge/platform-M5Stack|PC|Web-lightgrey.svg)
![](https://img.shields.io/cocoapods/l/Alamofire.svg?style=flat)
![Logo](Report/logo.png)
![Hi](Hi.png)

<a name="_advert"></a>

*Are you tired of queuing at the payment machine in the parking lot?*\
*Are you annoyed some payment machines only take cash?*\
*Are you frustrated that you left the ticket in the car and have to walk all that distance twice?*\
*Are you annoyed someone exiting the parking lot lost their ticket so the lane is clogged?*\
*Do you sometimes go on the kerb accidentally just because you want to stop closer to the barrier ticket machine?*\
*Are you unhappy that the automatic number-plate recognition doesn't always work?*\
*Do you have too many parking membership cards for different parking lots?*\
*Have you ever actually found or received a refund on a lost parking membership card?*

***PIMS, solves it all!***

<a name="_intro"></a>
## Introduction
Parking Iot Management System is designed for easy access to public
parking lots. It is like a toll-pass (FasTrak or E-ZPass) in the US 
or ETC in China, but used on parking lots.
Each driver wishing to use this system needs to obtain a
key (M5Stick) and register their personal and vehicle information on
the PIMS website. With a key in hand, the driver can park in any
participating parking lot, all they need to do is to press the key (Stick)
to enter, and press to exit. The barrier, controlled by M5 Stack, receives
the request from the Stick together with the Stick's bluetooth address, and 
verifies the user's account via the broker
(MQTT). If successful, the barrier (Stack) will lift up to let the car 
in/out. The system will track the car's parking time and calculate with the 
tariff of the parking lot's, then charge the payment from user's account.

With this system, the drivers can save the time of paying at the machine.
With cars leaving the parking lot quicker, more cars are able to park in
the lot during a busy day.

<a name="_stakeholders"></a>
## Stakeholders
Our product is designed for 3 different end users: customers, drivers, and parking lot managers.
To make the picture clear, we will introduce 3 typical user stories.
### Customers
Leo is sick and tired of the situations mentioned [above](#_advert), he saw 
the advertisement of PIMS and decides to make his life easier by starting using it.
He goes onto PIMS website and orders a key (M5Stick). After the key arrives,
he opens an account with PIMS. He finds its bluetooth sequence in the 
instruction booklet and enters it along with his car's vehicle identification
number. Ta-da! account created, now whoever drives his car can use PIMS service
for participating parking lots. He can also review how many hours and where he
has parked for the past 7 days.

### Drivers
Cathy is a University of Bristol employee. The university has a business membership with
PIMS, which means all the cars registered under the university's account has
a M5Stick and the expense will be taken from the university's business account.
Today Professor Smith and her research group need to attend a conference in Bath, representing
the School of Engineering. The meeting takes place in a conference center with
a public parking lot. They are given a university owned minivan to for the trip.

When approaching the entrance, she presses the parking lot key (M5Stick) that
was already mounted in the car. Even without stopping, the barrier opens
to welcome them in.

After the 5 hours conference, Prof Smith annd her group are really tired
and don't want to queue up
at the payment machine - the queue looks really long, it's a busy parking lot after all.
Luckily, they don't need to! They go straight back to the minivan and start driving back to Bristol.
When approaching the exit, without even stopping and opening the window,
Prof Smith simply presses the key (M5Stick) and the barrier lifts up to let them out!

### Parking lot managers
Rachel works for Cabot Circus shopping mall. Her office pretty close to the parking
lot so she can report any abnormal activies via the app. She also works from home from time to time.
Wherever she is, she can open the desktop application to see how many cars 
are currently in the parking lot and how much revenue they are currently making.

Her boss has asked her to plan for a 2-hour car wash product promotion in
the parking lot's designated area next 
Saturday, so she needs to choose a time when the number of targeted customers
are at its peak.
She opens the desktop app and pulls up the hourly record of the number of cars
in the parking lot for the past month. Then she chooses 1-3pm next Saturday
for the promotion.

<a name="_portfolio"></a>

## Product report

### [1. System Design](Report/System_design/README.md)
### [2. System implementation](Report/System_implementation/README.md)
### [3. Project Evaluation](Report/Project_evaluation/README.md)

<a name="_video"></a>

## Pitch video
Here is the video of us demonstrating how the M5Stack (barrier)
, M5Stick (key), desktop application (for managers) and webpage (for customers) 
work together via the communication on MQTT.

[![Video thumbnail](Report/Video%20thumbnail.png)](https://youtu.be/kaCjAmnIsRY "PIMS")
