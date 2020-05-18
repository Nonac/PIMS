# Requirements of key subsystems

## Web Application
Users use the web application to log up/log in an account, register their information, and see their account status. In order to enable users to use the web application to manage their accounts smoothly, it must fullfil the following requirements:
### Log up and log in
* Allow users to log up a new account -> inform the desktop app that there is a new user registration.
* Allow users to log in to have access to their own accounts -> validate username and password with desktop app and keep the logged status with cookies.
* Prevent the illegal access to user account page -> filter access by validating cookies.
### User Information Registration
* Users can register a new vehicle with a key delivered to them -> inform the desktop app and re-render the vehicle list.
* Users can top up their balance -> inform the desktop app a recharge made by users and send re-render the balance value.
### Display user info and statistical data
* Show the user's username, the vehicle they own, their account balance, and the vehicle they -> fetch user account info from the desktop app and render it on the account page.
* Display the statistical data of parking time in the last 7 days, showing in chart -> fetch a list of parking time and render the chart.



