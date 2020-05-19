# System Implementation
## Breakdown fo project into sprints

This project began in February and officially ended at the end of May. We have set relatively reasonable work assignments and milestones for this purpose. We created the GitHub repository for this project on February 6 and spent a few days after that getting the team familiar with the GitHub operating model. We then comb through the timeline of the different components, reviewing the development of each Sprint.

![Gantt](Gantt.png)

## Destop App

### Sprint 1: Requirements and Design
We completed the preliminary design of the entire system prototype between February 24 and February 28. We determined the way MQTT communicated with each component and determined the default form of the data structure within the entire system. Of course, this data form is quite different from the final data structure used, but this prototype has formalized our engineering direction. Based on determining the system prototype and data structure, we completed the front-end UI sketch design for the desktop app between February 28 and March 5.

### Sprint 2: Desktop App Back-end Develoment
As can be seen from the system design section, the desktop program not only acts as the main body of the desktop program but also acts as a communication bridge between the web and barrier ends. So as a program development back-end needs to be implemented simultaneously with web and m5stack development. The process went relatively smoothly, as our development team was relatively well-staffed, and the back-end developers could meet the design requirements of docking in both directions simultaneously. The back end of the program design was developed from March 19 until April 16 when the docking was completed. Because the desktop app uses object-oriented programming ideas, it can be constructed separately from the development front-end. By the end of development, the back-end app accomplished the following goals.

* Subscribe to, retrieve, parse, store and publish user and vehicle registration information from MQTT and complete the interaction with the web site.
* Subscribe, retrieve, parse, store and publish vehicle access information from MQTT and complete the interaction with the M2Stack end.
* Requests for access and information about users are logically processed and used to modify and save information about new users and required to achieve dynamic refresh.
* Send control commands to the barrier.
* Provides all the data needed to be captured, thus making a back-end API for the software.

### Sprint 3: Desktop App Font-end Develoment
#### Electronic design of sketches

Since the previous design was only a paper operation, the first step we should complete the paper design to electronic prototyping excess. The difficulty with this step is that some of the components conceived initially on paper are inherently tricky to electronic. In addition to this, the electronic design script adds a colour element that allows the use of the desktop app to be perceived through colour changes.

#### Layout simple elements to the desktop UI

It is extremely dangerous to fully develop the front-end UI directly before the desktop app back-end is developed. So we take the approach of first developing the UI part of the desktop app that does not involve the underlying logic API.

* The first part we complete is the construction of the title and text section, which only relates to the colours and fonts. It is worth mentioning that we have chosen the font Berlin Sans FB Regular, which is superior to the system default font in terms of display effect.
* Next, we finished laying out the buttons and menus, but did not write the part that interacted with the API, just did some design and tweaking of the page layout based on the art.
* Once again, we have implemented the clock tab at the top of the page. Since this project is highly correlated with time, it was essential to design a clock at the top of the desktop app to display time. While implementing the clock tag, we completed the build of the desktop app refresh mechanism. This is a significant step for follow-up.
* Finally, we finished building the colour pattern. Since this button is a pure display function independent of the system architecture, it is also effortless to implement.

#### Building elements that interact with back-end APIs

When the desktop app backend API was almost done, we started developing the display part that had logical interaction with the backend API. We completed the construction of the following components in order.

* First, we completed the presentation of the vehicle's pass record. Here we call the M5Stack pass log data stream of the backend API. In chronological order, the first entry is the latest pass record, and the display is refreshed in a previously constructed refresh mode.
* Next, we completed the information display of all vehicles present. The data is also updated in real-time by refreshing the system.
* Third, we completed the presentation of the data visualization components. Taking advantage of the PROCESSING data visualization, we effectively represented the ratio of the number of vehicles present to the total number of parking spaces and the real-time revenue from the parking lot. Due to the limited time available for the presentation of this project, we have set the time limit for the refresh mechanism at 100 seconds after group discussion.
* Finally, we have finished manually controlling the M5Stack button in the desktop app, which is done by calling the function in event. Control of the barrier is completed by packaging the JSON data to be sent.

### Sprint 4: Test in conjunction with the rest of the system and fix bugs in the desktop app

We conducted a total of three tests, a single-part effect demonstration, a joint test with the web end and a joint test with M5Stack. The performance and reliability of the system are refined with each test. Compared to the first bug-filled test, the third test was perfect, working smoothly with the other two sets of parts through the MQTT.

### Development and Review

When we have done all the testing, we branch out to integrate, release and reflect on the project. We finished building the desktop widget, which taught us a lot, including the use of development tools. However, what we learned was more about how to collaborate in teams to enable software development.