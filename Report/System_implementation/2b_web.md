## Evaluation

### Web Application

**Bootstrap4 and jQuery**

*Why we choose them?*

Bootstrap4 is very friendly for beginners of web development because it provides diverse useful HTML and CSS components. Furthermore, Bootstrap4 is perfect in making responsive pages, and as the users might want to view their account info and top in on their mobile device, the responsive design will guarantee that the pages perform well in different scenarios.

jQuery helps to simplify the DOM manipulation, which means that we can choose any elements on a web page more easily. In our situation, it is enough to use jQuery for this lightweight web application.

*Limitation*

There is a limitation that the versions below 10 of Internet Explorer do not support bootstrap 4 because they do not support flex layout. This will cause some unknown problems or program crashing when the users use an IE browser to access to this application. This could potentially be unfriendly to those who use an old browser. However, as we should encourage the users to upgrade their browsers to phase out the old browsers with bad performance and bad supporting to new features, we did not consider using  Bootstrap 3 instead of 4 in this application, although it will have better compatibility towards lower version of browsers.

Directly manipulating on DOM tree is regarded as bad practice in programming nowadays, and programmers are encouraged to use frameworks that hide the DOM manipulation like React and Vue in front-end development. However, for this light-weight application, it seems unnecessary to put the so heavy frameworks in use. But one thing worth considering is that if we want to extend this application to a wider use, it would be better to use a framework such as Vue or React.

****

**Echarts**

*Why we choose this?*

Echarts enables data visualization and chart dynamic rendering with several easy function calls. This helps us to draw a chart to show the parking time of users.

*Limitations*

To initialize an echarts object we have to provide the constructor function a DOM block element with specified height and width. However, in our responsive web design, the container for the chart has a percentage as height and width, which is not supported by the Echarts. To solve this, we use jQuery to set the width and height of the element dynamically at the stage of page loading, and this can give the element fixed values of length and width according to the actual size of it. 

But this brings another problem: after the jQuery setting a fix value for any attribute, the element keeps the value when even if the page size has been changed by user. This will cause the chart out of edge if the user zoom in the page. To fix this, a solution is to listen the element size using jQuery, however, this will cause a bad performance of the JavaScript logics, and it is not worth implementing because the user can refresh the page to make it a dynamic size.

