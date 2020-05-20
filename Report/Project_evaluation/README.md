# Project Evaluation
## Contents

* [Success of the project](#_sucess)
* [Future work](#_future)
* [Working practices of the group](#_practice)
* [Coronavirus impact](#_coronavirus)

<a name="_success"></a>

## Success of the project
在这里写主要的成就和**学到了什么（这一点非常重要！不要怕重复前面的）**
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

### Web
### Desktop
* Front end

* Back end

<a name="_future"></a>

## Future work
在这里写如果这个产品日后用于商业用途，还有什么功能可以加。

There are many features to be added in order for this product to launch for public use.

### Short-term goals

#### IoT
* More secure identification

When a barrier at the entrance of a car park (likewise for the one at the exit) sends the ambient BT information to our server with a registered key having the greatest RSSI and if the status for that key is not in the car park, our server should instruct the barrier to pair the key instead of lifting the bar directly. After pairing, the key sends an authentication key (determined when the key was manufactured and stored in our server) to the barrier. The barrier then relays the authentication key to our server. If the authentication key acquired matches that stored in our database, then the server will command the barrier to lift. Otherwise, the lifting operation is abandoned and the incident is recorded.

* More compact messages to send

Since only the nearest key to a barrier will be considered by our server, we could omit devices with RSSIs below a certain value. Experimental tests will be carried out to determine a sensible value.<br>
Since the M5Stack has a TF card reader, we could also let each barrier store a blacklist for BT addresses that are not keys. The BT devices detected in a scan will first be filtered by the blacklist, only the information about our keys will be sent to our server.
The blacklist in cleared and reconstructed every day to keep it concise and up-to-date. The blacklist will be built with the help of our server: if our server finds a device in the scan result sent from a barrier that is not a key, a command will be sent to that barrier including the BT address of this device. On receival of the command, the barrier will add the BT address to its blacklist.


#### Web
比如可以在网站上实时附近（支持这个系统的）停车场有多少空位。database可以转移到网上,这样可以保护data privacy， 因为目前所有的user data都不保密，manager全都可以看到。购买stick的页面。注销账户选项。
#### Desktop
* Admin account page

In the top left corner we have included a button for the access to the manager/admin's account. This means every staff using this application needs to log in to make sure they have the rights to use this application. This will then induce a login screen before this main application page. In the manager/admin's account page there will be information such as their work phone number, how many hours have they worked/used this application, and a link to submit tickets should there be any technical
issues, the records of the customer issues they have helped to solve, etc.
不同的停车场需要有不同的total space count

* Customer account page

Admins should have the rights to access any customer's account, with confidential information hidden, of course. This is useful when customers encounter technical issues, e.g. cannot top up, cannot see parking history, cannot create account, cannot add vehicles etc. Under these circumstances, the admin/manager can access their account to complete those tasks for them, or create an account for them. After completing those tasks, there should be a record of it showing in the admin's account as their accomplishments. 

### Long-term goals

Ultimately and ideally, the M5Stick can be purchased along with the car, so all new-generation cars can start using this system immediately. Sooner or later, it can cover all the parking lots and all the cars registered
in the UK.

While the above is hard to achieve, something easier to expand this service on is the drive-thru restaurants. The payment can be completed by the similar method: Stick sends user information (no need for car's information this time, but because the Stick is kept in the car, this can be convenient for drive-thru service), Stack at the restaurant detects the Bluetooth and sends the user information and payment amount to the broker, then desktop and web ends can, respectively, charge the payment and records the user's activity.   

<a name="_practice"></a>

### Working practices of the group
#### Agile development

#### Daily communication
We mainly used WeChat, a Chinese main stream online chatting platform, to communicate on a daily basis. Everyone has been very friendly and respective to others and reachable. WeChat supports unlimited free group video and voice calls, and offers stable internet connection in the UK and China. Thus, whenever we
needed clarifications that were harder to explain over messaging, we arrange WeChat voice calls, which saves the tariff that would have occurred on international phone calls. During the first few project testing periods, we also used WeChat video/voice calls.

#### GitHub
We used GitHub for all the code development and file sharing, except for video recordings we chose Google Drive because of their large sizes. We have 2 remote branches on the repository: master and dev. Everyone developed their codes locally and pushed them to dev branch. One group member was in charge of merging dev into master whenever our codes were sophisticated enough to be a release version. That group member was also mainly in charge of creating issues and allocating specific tasks on group members. Then the issues got closed when they were solved.

#### Self-reflection 
When we formed the group, we were not close friends as we are now. There were trust issues, disagreements, and doubts. With the project rolling on, we built up trust and functioned as one. We discovered each other's strengths - everyone has a unique strength that benefitted the project. 

Yinan - full of creative ideas, without him we would not have come up with this design.

Tao - IT genius,  just leave him with the M5Stack and M5Stick and he will come back with a complete implementation in a matter of days.

Fuzhou - his intelligent brain handled the entire website design and implementation all by himself in 5 days.

Hongjie - brilliant developer, just tell him what result we want and he will deliver well-functioning codes promptly.

Zhen - smart programmer. He and Hongjie wrote the core server controller that all other elements depend on.

Lea - charismatic leader, who is seasoned to coordinate the work of the team. Without her, we are just a mass of sand.

Outstanding front-end designer and team leader. Through her communication, every part of us is brought closer together. Excellent language and video editing skills made for an impressive team presentation.--from Yinan



Now it's the end of the project. Looking back on the past 3 months, not only did we gain knowledge from the lectures, workshops, and online resources, but also from each other. We talked through our own codes to each other and offered suggestions on techniques to use. We discuss frequently and figured out together how to collaborate under this disturbing situation. We have earned precious experience of group collaboration.


<a name="_coronavirus"></a>

### Coronavirus impact
This work has been impacted by the global pandemic COVID-19, as 3 out of 6 group members went back to China. The time lost greatly on not only the long-haul flight but also: trying to constantly refresh airline pages to get precious tickets that might be cancelled by others, keeping an eye on the airline announcements on the flight status, queuing up for temperature tests after landing, and 14-day isolation (some were even longer due to close-by passengers on the flight tested positive) in a hotel that only offers basic facilities which means not ideal internet connection nor ideal hardware facilities.

Besides that, the 7-hour time difference made communication harder in 2 ways: 1 We could only effectively communicate when we were all awake. 2 Only the members remaining in the UK could participate in lecturers' video meeting because the meetings were held after midnight in China.

Also, due to the censorship in mainland China, the group members have had episodes of hard times to access github, which in return delayed development.

One other thing to mention is that due to the COVID-19 influence, the marking criteria changed to 100% dependent on the portfolio, which is not the top strength of our group. As a group of foreigners whose first language isn't English, it was a shame that we couldn't deliver the well-functioning product that we are proud of face to face, but to write it down. We needed twice or more time than others on the writing to make sure our language is clear and straight forward, because we feared that no matter how much effort we put in to produce this software, if we couldn't deliver it clearly in the report, which is the only chance we have, all the effort would be wasted.
