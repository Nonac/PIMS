# Project Evaluation
## Contents

* [Success of the project](#_sucess)
  * [Web](#_aweb)
  * [Desktop](#_adesktop)
  * [IoT](#_aiot)
* [Future work](#_future)
  * [Short-term goals](#_future-short)
    * [Web](#_short-web)
    * [Desktop](#_short-desktop)
    * [IoT](#_short-iot)
  * [Long-term goals](#_future-long)
* [Working practices of the group](#_practice)
  * [Agile development](#_agile)
  * [Daily communication](#_daily)
  * [GitHub coordination](#_github)
  * [Self-reflection](#_self)
* [Coronavirus impact on the project](#_coronavirus)

<a name="_success"></a>

## Success of the project

<a name="_aweb"></a>

### Web

| Achievement                               | What we learnt from it                                       |
| ----------------------------------------- | ------------------------------------------------------------ |
| Built up a complete front-end application | In order to implement the web application, we learned the basic techniques that are used in front-end development, such as HTML5, CSS, and JavaScript. We also learned from more advanced techniques like Bootstrap 4 and establishing connections with MQTT broker. This gives us a good view of how front-end applications work and how to build web pages from scratch -- there was no team member had the experience in web development, while after doing this project, we are quite confident that we are able to build up web pages in a short time. |
| Responsive design                         | The web pages are designed to be responsive. We have learned a lot about how to design the layout and the position of elements. We also have had a better understanding of the approaches to make the web pages responsive. |
| Met all requirements                      | All requirements set at the beginning of this project have been met. The data schema designed according to the requirements perfectly allow us to meet the requirements by implementing functions for the data types one by one. |
| Good communication and teamwork           | The communication between the web application and the desktop application is somewhat sophisticated. There are 6 types of data with different meaning, which requires good communication and collaboration skills to demonstrate and understand. Fortunately, our programmers carried out good communications and teamwork to implement this web application meeting all the requirements. This is the most valuable thing that we learned from this project, as it is also very important in real-world software development. |

<a name="_adesktop"></a>

### Desktop
* Front end

  Front-end engineering for desktop applications, like all view work, is a combination of fine art and dynamic technology. How to create a user interface that corresponds to the application scenario the user is in is the main thing we should consider. For example, we should consider what colour patterns to apply under what circumstances, or what data visualization tools to use to achieve the best results under what circumstances. Thinking logically like this can be challenging but necessary for front-end.

  The desktop app view framework we applied in this project is Processing's controlP5 repository. Of course, this is our best option for completing class requirements. Although we had several alternative options, it was the framing conditions that allowed us to adapt to similar work later. To develop the framework, the front-end working group fully read the developer documentation, including all the samples and even the source code of some repositories. We also found bugs in the sample architecture, which is why we skipped the controlP5 multi-page refresh development.

  Of course, it's not just technology that we've learned. We also learned how to work as front-end engineers on large projects to coordinate with back-end engineers and flexibly adjust our design and code as the back-end functions evolve. This builds the foundation for more significant work.

* Back end

  The desktop server controller successfully communicated with M5Stack through the MQTT protocol. Even though the desktop front-end side and the M5Stack side use different language for development (Processing and C++, respectively), data exchange using JSON file format is still smooth. We took into account the needs of both the M5Stack end and web end, and controlled the task logic well: the controller can accept and process the requests from M5Stack and send back receipts at the same time.

  We have never been exposed to the software Processing before, so we were very unfamiliar with the syntax. By reading the documentation of Processing, understanding the APIs provided by it, and performing unit testing, we used the library functions provided by Processing well and achieved our goal. It is often troublesome to collaborate and test with multiple group members at the same time, so we used the MQTT client to manually publish the formatted JSON file to simulate the M5Stack message. In this way, everyone in the group can complete the entire system testing independently.

<a name="_aiot"></a>

### IoT

<table id = "3aIoTTb">
  <tr>
    <th>Achievement</th>
    <th>What we have learnt from it</th>
  </tr>
  <tr>
    <td>Established achievable goals.</td>
    <td>Setting up reasonable objectives before fulfilling them will not only help you understand your tasks better but also saves your time.<br>
      In our case, our task was to develop an IoT smart barrier. Instead of working with a real barrier, we chose to simulate it on the M5Stack. Had we purchased a real barrier, it could have cost us an iPhone 11 in 2020. Had we built a miniature barrier from small motors and plastic bars that could be found in the robotic kits that are popular among 5 years olds, our programmers could have spent twice the time they did to finish their jobs without making a real difference to our project<del>, during which time they were still getting paid</del>.<br>
      Therefore, knowing how to utilise time and resources is of vital importance.
    </td>
  </tr>
  <tr>
  <td>Successfully made the IoT devices work</td>  
    <td>Surprisingly, our programmers actually finished their jobs. <del>It's certainly worth every penny of the 50 pounds. </del>
      Thanks to working for us, they have learnt:
      <ul>
        <li> How easy life is when exploiting existing libraries compared to reinventing the wheel.</li>
        <li> How rules and format plays an important role if we want to make messages sent from one device to be understood by another </li>
        <li> How important collaboration and coordination are to keep working on the right track. </li>
        <li> And some insignificant things that no one cares about, such as:
          <ul>
            <li> How to program in embeded c++ and the Arduino IDE.</li>
            <li> How BlueTooth works.</li>
            <li> What Serial communication is and how to choose a resonable baud rate.</li>
            <li> What JSON is and its serialisation/deserialisation.</li>
            <li> What MQTT is and how it facilitates IoT development.</li>
            </il>
        </li>
        </ul>
    </td>
  </tr>

</table>

<a name="_future"></a>

## Future work
There are many features to be added in order for this product to launch for public use.

<a name="_future-short"></a>

### Short-term goals

<a name="_short-web"></a>

#### Web

The web application we implemented in this project only has some simple functionality for demonstration, such as user registration, vehicle registration, topping up, and some simple data visualization. However, as a user client, the web application is the most important part to attract new users, and it determines how user-friendly our product is. Most functionality related to users should be displayed on the web application.

There are absolutely more features could be implemented in the future:

* Order a stick

  Now the user could register their vehicles and stick id on the web page, but what if they do not have a stick (?!) This is a problem because currently the only way for a user to get the stick is that they know us and ask us for the stick (which you can do). So there will be a button for users to order sticks from the website. Put an order, and get the stick in 5 days -- this will also help develop the express delivery industry!

* Parking history & Fee charging history

  In the current web application, users can only see their parking time in the last seven days. However if you are a user, you may want to know how long you parked and how much you were charged on Sep. 9th, 2019. It is quite necessary to make a page for users to view and search their parking data.

* More data visualization!!

  Data visualization might be an important aspect of a web application. In our case, users might want to see some other kinds of charts, such as the statistics of their parking fee and parking time in different parking lots.

* Quick top up on mobile device

  "What? Balance not enough?" Simon shouts at the barrier because he does not have enough balance to pay his parking fee when he wants to leave the parking lot. Then he has to open up his browser on the phone, input the url, log in, and finally he tops up! This would be quite inconvenient and will potentially cause a low efficiency at parking lot, and the drivers after Simon might whistle at him angrily. To solve this problem, in the future we will develop mobile apps on Android device and iOS to enable quick top up, and the users can choose whether they want to automatically top up by binding their bank accounts with PIMS.

* Map API

  After PIMS is widely deployed in the UK ~~which is impossible~~ we will add a map to the front-end application (web or mobile apps) to enable users to find the participating parking lots in the vicinity, and how many available parking spaces they have. Moreover, they can also mark down where they parked their cars as some of them might forget it after a long time of parking. 

These features could be beneficial because they will promote the user experience. However, as the web application is just the front-end app, more efforts should be made in the desktop application (or a complete back-end architecture in the future) and in the IoT device.

<a name="_shot-desktop"></a>

#### Desktop

##### Front end

* Admin account page

  In the top left corner we have included a button for the access to the manager/admin's account. This means every staff using this application needs to log in to make sure they have the rights to use this application. This will then induce a login screen before this main application page. In the manager/admin's account page there will be information such as their work phone number, how many hours have they worked/used this application, and a link to submit tickets should there be any technical
  issues, the records of the customer issues they have helped to solve, etc.

* Customer account page

  Admins should have the rights to access any customer's account, with confidential information hidden, of course. This is useful when customers encounter technical issues, e.g. cannot top up, cannot see parking history, cannot create account, cannot add vehicles etc. Under these circumstances, the admin/manager can access their account to complete those tasks for them, or create an account for them. After completing those tasks, there should be a record of it showing in the admin's account as their accomplishments. It was originally planned to accomplish, however, due to the related methods in ControlP5 library being broken, we had to skip it.

* Multiple parking lots handling

  For now, the desktop app only manages one parking lot, it should be extended to handle difference ones. The manager can request to create new parking lots (that correspond the ones in real world, of course) and manually insert the parameters, e.g. total parking space count, tariff etc.

##### Back end

* Multithreading

  In this project, for the process of opening and closing the barriers when the car enters the parking lot, we just set a time of 5 seconds to simulate the car entering. To make matters worse, we only used the delay (5000) API to block the system. At this time, if other messages came, the system could not receive it. So in the future we will use multithreading to receive requests, so that even if one thread is blocked, it will not affect other threads. At the same time, the duration of the barriers staying up cannot be a fixed 5-second,. Instead, we should use an intelligent identification to determine whether the car has successfully entered the parking lot, and then lower the barrier.

* Data storage

  At the database level, we only used the local hard disk to store data. Each time we store the data, we must traverse all the files in the folder to refresh them. This method is very unsafe and inefficient because in reality, there will be a great amount of user thus a great amount of files to iterate through. Therefore, we should use MySQL, SQLServer or other advanced databases to store the data, and improve the query efficiency by setting IDs. For confidential information, such as passwords and customers' names that they provide on the website, we will not directly store them in plain text, but apply encryption methods such as MD5 to store them in cipher text.

* Session keeping

  As is demonstrated in [System Implementation](../System_implementation/README.md), there is actually no real session system in our front-end and back-end communications. In the future a session system must be used to implement this feature, in order to achieve a higher level of security.

<a name="_short-iot"></a>

#### IoT

* More secure identification

  When a barrier at the entrance of a car park (likewise for the one at the exit) sends the ambient BT information to our server with a registered key having the greatest RSSI and if the status for that key is not in the car park, our server should instruct the barrier to pair the key instead of lifting the bar directly. After pairing, the key sends an authentication key (determined when the key was manufactured and stored in our server) to the barrier. The barrier then relays the authentication key to our server. If the authentication key acquired matches that stored in our database, then the server will command the barrier to lift. Otherwise, the lifting operation is abandoned and the incident is recorded.

* More compact messages to send

  Since only the nearest key to a barrier will be considered by our server, we could omit devices with RSSIs below a certain value. Experimental tests will be carried out to determine a sensible value.<br>
  Since the M5Stack has a TF card reader, we could also let each barrier store a blacklist for BT addresses that are not keys. The BT devices detected in a scan will first be filtered by the blacklist, only the information about our keys will be sent to our server.
  The blacklist in cleared and reconstructed every day to keep it concise and up-to-date. The blacklist will be built with the help of our server: if our server finds a device in the scan result sent from a barrier that is not a key, a command will be sent to that barrier including the BT address of this device. On receival of the command, the barrier will add the BT address to its blacklist.

<a name="_future-long"></a>

### Long-term goals

Let's look into a greater future. With the development of IoT technology, people will no longer be satisfied with simply tracking real objects, but to interact with them. More specific than IoT (Internet of Things), PIMS promotes the idea of IoV (Internet of Vehicles), the core of which lies in the interaction between M5Stick-C and M5Stack, in other words, the communication between the key and the door. If we continue to develop this project, we can predict some pictures of the future. Ultimately and ideally, the M5Stick can be purchased along with the car, so all new-generation cars can start using this system immediately. Sooner or later, it can cover all the parking lots and all the cars registered in the UK. Then, the car itself can become an electronic wallet, and it won't be a dream for us to apply such a convenient system at any drive-through restaurants.

Let's come back to this project, the key-door interaction relies on the Bluetooth technology we have now. Of course, for security purposes, it is certainly not possible to use protocols that are so open for a system that involves so many people. The project will then have the opportunity to advance the development and implementation of a proximity vehicle interaction encryption protocol. The advantages offered by it are significant compared to the identification technologies, such as automatic license plate recognition, image recognition, and IC card radio communication, that exist today. The new wireless protocol is fundamentally different from the existing techniques in terms of both the speed and accuracy of recognition.

What if we think outside the box regarding "vehicles"? Then we will get a door that can be opened from a distance, and to go through this door, we need tickets. By extending this idea, we get office area management systems, amusement park management systems, cinema management systems and even import/export logistics management systems for ports. All in all, the system will be rich in extensions in the long-term, and all these extensions will make our lives more convenient.

<a name="_practice"></a>

### Working practices of the group

<a name="_agile"></a>

#### Agile development



<a name="_daily"></a>

#### Daily communication

We mainly used WeChat, a Chinese main stream online chatting platform, to communicate on a daily basis. Everyone has been very friendly and respective to others and reachable. WeChat supports unlimited free group video and voice calls, and offers stable internet connection in the UK and China. Thus, whenever we
needed clarifications that were harder to explain over messaging, we arrange WeChat voice calls, which saves the tariff that would have occurred on international phone calls. During the first few project testing periods, we also used WeChat video/voice calls.

<a name="_github"></a>

#### GitHub coordination

We used GitHub for all the code development and file sharing, except for video recordings we chose Google Drive because of their large sizes. We have 2 remote branches on the repository: master and dev. Everyone developed their codes locally and pushed them to dev branch. One group member was in charge of merging dev into master whenever our codes were sophisticated enough to be a release version. That group member was also mainly in charge of creating issues and allocating specific tasks on group members. Then the issues got closed when they were solved.

<a name="_self"></a>

#### Self-reflection

When we formed the group, we were not close friends as we are now. There were trust issues, disagreements, and doubts. With the project rolling on, we built up trust and functioned as one. We discovered each other's strengths - everyone has a unique strength that benefitted the project. 

Yinan - a natural born leader, full of creative ideas. Without him we would not have come up with this design.

Tao -  an IT genius,  just leave him with the M5Stack and M5Stick and he will come back with a complete implementation in a matter of days.

Fuzhou - his intelligent brain handled the entire website design and implementation all by himself in 5 days.

Hongjie - a brilliant developer, just tell him what result we want and he will deliver well-functioning codes promptly.

Zhen - a smart programmer. He and Hongjie wrote the core server controller that all other elements depend on.

Lea - a charismatic leader, who is seasoned to coordinate the work of the team. Without her, we are just a mass of sand.

Now it's the end of the project. Looking back on the past 3 months, not only did we gain knowledge from the lectures, workshops, and online resources, but also from each other. We talked through our own codes to each other and offered suggestions on techniques to use. We discuss frequently and figured out together how to collaborate under this disturbing situation. We have earned precious experience of group collaboration.


<a name="_coronavirus"></a>

### Coronavirus impact on the project
This work has been impacted by the global pandemic COVID-19, as 3 out of 6 group members went back to China. The time lost greatly on not only the long-haul flight but also: trying to constantly refresh airline pages to get precious tickets that might be cancelled by others, keeping an eye on the airline announcements on the flight status, queuing up for temperature tests after landing, and 14-day isolation (some were even longer due to close-by passengers on the flight tested positive) in a hotel that only offers basic facilities which means not ideal internet connection nor ideal hardware facilities.

Besides that, the 7-hour time difference made communication harder in 2 ways: 1 We could only effectively communicate when we were all awake. 2 Only the members remaining in the UK could participate in lecturers' video meeting because the meetings were held after midnight in China.

Also, due to the censorship in mainland China, the group members have had episodes of hard times to access github, which in return delayed development.

One other thing to mention is that due to the COVID-19 influence, the marking criteria changed to 100% dependent on the portfolio, which is not the top strength of our group. As a group of foreigners whose first language isn't English, it was a shame that we couldn't deliver the well-functioning product that we are proud of face to face, but to write it down. We spent twice or more time than others on writing to make sure our language is clear and straight forward, because we feared that no matter how much effort we put on the product, if we couldn't deliver it clearly in the report, which is the only chance we have, all the effort would be wasted.
