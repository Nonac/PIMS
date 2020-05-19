# System implementation
## Details of how you evaluated your designs
### Desktop App Font-end

Due to the independent and collaborative nature of desktop app front-end development, the evaluation of desktop app front-end is divided into the following two main aspects.

#### Evaluation and testing of separate components on the front end

This part of the component can run independently on the desktop app side without calling the underlying interface so that it can be tested separately.

##### Clock

![clock](./destop_App_Font-end/clock.png)

The clock text variable calls the system time, and success is marked by the ability to refresh the text with the refresh of the DRAW() function. In other words, the ability of the top clock label to display the system time properly is a successful evaluation criterion. The difficulty with this part is that replacing the clock tag text should be done in the DRAW() method, and not under our own constructed refresh mechanism. Moreover, it is using a content replacement approach rather than an overall replacement approach. This way, we can effectively avoid reporting errors.

##### Colour Setting Menu

The color shift menu is also a display component that does not involve the lower-level API, so the evaluation of the color shift can be done separately when the desktop app undergoes front-end development.The colour shift menu is also a display component that does not involve the underlying API, so the evaluation of the colour shift can be done separately when the desktop app undergoes front-end development. It is easy to understand that once the colour shift menu is used, the desktop application view can be changed from the original default dark colour mode to light colour mode. Of course, we can also go from light to dark mode.

This section is evaluated on the ability to successfully change the colour pattern of all components without reporting errors. Due to the peculiarities of PROCESSING, we have set the colour changing function in operation to control the successful use of this menu.

#### Evaluation and testing of components on the front end that need to call the back end API

All components in this section must work with the API interface provided by the back-end in order to function correctly, so their testing must be done simultaneously with the back-end as well as the web and M5Stack ends in order to be adequately evaluated.

##### New cars coming in

When the barrier end receives a signal from an oncoming car, and the rear end successfully receives the incoming and outgoing logs and saves them on the local hard drive, the display end calls up all JSONArray to pick, sort, reorganize and display. Its evaluation of success is based on the front-end's ability to display the record correctly once the file has been received and saved by the back-end.

We require to arrange backwards, so the difficulty lies in how to sort JSONArray for specific fields. This, of course, falls under the category of algorithmic issues that are no longer discussed in this project.

##### Details of cars in the parking lot

Similar to the above features, this component shows the remaining vehicles in the field. Therefore, it is also necessary to receive the queue of vehicles in the field and select the traffic record as in for display. The following highlights are the criteria for the successful evaluation of this component.

* Once a vehicle enters the parking lot, this component should refresh the record of that vehicle for a limited time and display it on the display end.
* Once a vehicle leaves the parking lot, this component should refresh to delete this vehicle record for a limited time.

##### Pie chart

Data visualization is an advantage of PROCESSING, and we have developed data visualization widgets using this feature. The pie chart shows the ratio of vehicles now for the total number of spaces in the car park. Similar to the two lists above, this one needs to be linked to a remote M5Stack. Two requirements need to be met for their successful evaluation.

* When a vehicle enters at the barrier end and is successfully recorded by the desktop application backend, the pie chart automatically refreshes the record to increase the percentage of vehicles present.
* When a vehicle leaves the barrier end and is successfully recorded by the desktop application backend, the pie chart automatically refreshes the record, reducing the percentage of vehicles present.

##### Line chart

Similar to the above components, the line chart shows the parking lot of revenue for the day. The following criteria can judge its success. When the barrier end detects the vehicle leaving the parking lot, the desktop program backend monitors the MQTT stream and stores the file to the local hard drive. The front-end calls the API interface for receivable calculations, which are finally reflected in a line chart that is dynamically refreshed in real-time.

##### Barrier control button

This widget is the one that communicates directly with M5Stack, so the way he works is to call the underlying API event method to control the barrier by sending a specific data type to MQTT for M5Stack to receive.

The criteria for evaluating the success of this component can be judged in this way. We open the desktop app and M5Stack at the same time, click on the button of this component on the desktop, and the screen of M5Stack shows the barrier state when the control is complete by command. If the transformation is successful, it is evaluated as successful.