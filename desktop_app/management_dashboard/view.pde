//Todo:
//1. bulid a couple of button to[add new],[edit],[delete],[find] user's message.
//2. bulid a tableView class.
//3. bulid a dinamic reflesh tableView for last ten records I/O
//4. bulid a class for data visualization.

Textlabel timer;
ListBox list;
boolean colorModeSwitch=true;
int darkModeBackGround=color(41,75,140);
int lightModeBackGround=color(213,223,241);
int darkModeFontColor=color(213,223,241);
int lightModeFontColor=color(41,75,140);
int darkModeInfoWindowColor=color(132,160,215);
int lightModeInfoWindowColor=color(171,192,227);


void refreshDashboardData() {
    // We just rebuild the view rather than updating existing
    updateDashboardData();
}

void updateDashboardData() {
    refreshData();
    surface.setTitle("Parking IOT Management System");
    view.build();
    
    
}

public class Dashboard_view {
    
    
    void build(){
      PFont p = createFont("Berlin Sans FB",20); 
      ControlFont font = new ControlFont(p);
      cp5.setFont(font);
      view.buildSettingSwitch();
      
    }
    
    void buildBackGround(){ 
      if(colorModeSwitch){
        background(darkModeBackGround); 
      }else{
        background(lightModeBackGround);
      }
    }
    
    void buildSettingSwitch(){
       list = cp5.addListBox("Settings")
       .setPosition(1638,10)
       .setSize(120,80)
       .setItemHeight(22)
       .setBarHeight(22)
       .setBackgroundColor((colorModeSwitch)?darkModeInfoWindowColor:lightModeInfoWindowColor)
       .setColorBackground((colorModeSwitch)?darkModeBackGround:lightModeBackGround)
       .addItem("Light Mode",0)
       .addItem("Dark Mode",1)
       .setColorActive((colorModeSwitch)?lightModeInfoWindowColor:darkModeInfoWindowColor)
       ;
    }
    
    
    void buildTimer(){
      int s = second();  // Values from 0 - 59
      int m = minute();  // Values from 0 - 59
      int h = hour();    // Values from 0 - 23
      
      timer= cp5.addTextlabel("label")
      .setText(""+h+":"+m+":"+s)
      .setFont(createFont("Berlin Sans FB",20))
      .setPosition(880,0);
    }
    
    void buildIcon(){
       Button accountIcon =cp5.addButton("accountIcon")
       .setPosition(0,0)
       .setSize(10,10)
       ;
       Button settingsIcon=cp5.addButton("settingsIcon")
       .setPosition(1600,0)
       .setSize(10,10)
       ;
       if(colorModeSwitch){
         accountIcon.setImages(loadImage("darkAccountIcon.png"),loadImage("darkAccountIcon.png"),loadImage("darkAccountIcon.png"));
         settingsIcon.setImages(loadImage("darkSettingsIcon.png"),loadImage("darkSettingsIcon.png"),loadImage("darkSettingsIcon.png"));
       }else{
         settingsIcon.setImages(loadImage("lightSettingsIcon.png"),loadImage("lightSettingsIcon.png"),loadImage("lightSettingsIcon.png"));
         accountIcon.setImages(loadImage("lightAccountIcon.png"),loadImage("lightAccountIcon.png"),loadImage("lightAccountIcon.png"));
       }
    }
    
}
         
         
void Settings(int theValue){
      switch(theValue){
         case 1: 
           colorModeSwitch=true;
           view.buildBackGround();
           break;
         case 0:
           colorModeSwitch=false;
           view.buildBackGround();
           break;
      }
    }
