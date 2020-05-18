# System Design
## 1b OBJECT-ORIENTED DESIGN OF KEY SUBSYSTEMS
## destop Design

![destopAppUML] (desktopApp_UML.png)

The basic object-oriented model for this project is shown in the figure. The DashboardView part of the picture is the view part of the MVM model. This section is dominated by the build program view section, so it is mainly for the construction of the program's graphical interface, with buildUI functions dominating. The Database section shows the Model section. The GETS and RECEIVES functions are the main ones. where MessageType is the important message storage abstract class that defines all data types. Event is the main controller part. Mainly for the interaction of the desktop app with the barrier and the interaction of the desktop app with the web client. mqtt is also mainly applied in this part as a means of communication.