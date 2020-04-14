//Todo:
//1. bulid two tableView class.
//2. bulid a dinamic reflesh tableView for last ten records I/O
//3. bulid a class for data visualization.

//dark/light mode switch
boolean colorModeSwitch=true;


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
  //color mode
    int darkModeBackGround=color(41,75,140);
    int lightModeBackGround=color(213,223,241);
    int darkModeFontColor=color(213,223,241);
    int lightModeFontColor=color(41,75,140);
    int darkModeInfoWindowColor=color(132,160,215);
    int lightModeInfoWindowColor=color(171,192,227);
    int titleBackGround=color(87,106,195);
    int settingsBackGround=color(184,197,229);
    
    Textlabel timer,accountLabel,title,subtitle,detailLabel,newCarsComingInLabel,barrierControlLabel;
    ScrollableList list;
    
    //build() is for those complement won't change color with color switch
    void build(){
      //set Berlin Sans FB as Font
      PFont p = createFont("Berlin Sans FB",20); 
      ControlFont font = new ControlFont(p);
      cp5.setFont(font);
      
      view.buildTitle();
      view.buildSettingSwitch();      
    }
    
    void buildBackGround(){ 
      if(colorModeSwitch){
        background(darkModeBackGround); 
      }else{
        background(lightModeBackGround);
      }
    }
    
    //color switch
    void buildSettingSwitch(){
       list = cp5.addScrollableList("Settings")
       .setPosition(1638,10)
       .setSize(120,120)
       .setItemHeight(40)
       .setBarHeight(20)
       .setColorForeground((colorModeSwitch)?darkModeBackGround:lightModeBackGround)
       .setColorBackground((colorModeSwitch)?darkModeBackGround:lightModeBackGround)
       .addItem("Light Mode",0)
       .addItem("Dark Mode",1)
       .close()
       ;
       
       if(colorModeSwitch){
        list.setColorLabel(darkModeFontColor).setColorValue(darkModeFontColor);
       }else{
       list.setColorLabel(lightModeFontColor).setColorValue(lightModeFontColor);
       }
    }
    
    //Timer on the middle head
    void buildTimer(){
      int s = second();  // Values from 0 - 59
      int m = minute();  // Values from 0 - 59
      int h = hour();    // Values from 0 - 23
      
      timer= cp5.addTextlabel("timer")
      .setText(""+h+":"+m+":"+s)
      .setFont(createFont("Berlin Sans FB",20))
      .setPosition(840,0);
      
      if(colorModeSwitch){
         timer.setColorValue(darkModeFontColor); 
      }else{
         timer.setColorValue(lightModeFontColor);
      }
    }
    
    //All the image are saved in ~/data/.. This function is for all the image button.
    void buildButton(){
       Button accountIcon =cp5.addButton("accountIcon")
       .setPosition(5,5)
       ;
       Button settingsIcon=cp5.addButton("settingsIcon")
       .setPosition(1615,5)
       ;
       Button liftControl=cp5.addButton("liftControl")
       .setPosition(1330,750);
       
       Button closeControl=cp5.addButton("closeControl")
       .setPosition(1500,750);
       
       Button customerAccountButton=cp5.addButton("customerAccountButton")
       .setPosition(1325,900);
 
       if(colorModeSwitch){
         accountIcon.setImages(loadImage("darkAccountIcon.png"),loadImage("darkAccountIcon.png"),loadImage("darkAccountIcon.png"));
         settingsIcon.setImages(loadImage("darkSettingsIcon.png"),loadImage("darkSettingsIcon.png"),loadImage("darkSettingsIcon.png"));
         liftControl.setImages(loadImage("darkLiftControl.png"),loadImage("darkLiftControl.png"),loadImage("lightLiftControl.png"));
         closeControl.setImages(loadImage("darkCloseControl.png"),loadImage("darkCloseControl.png"),loadImage("lightCloseControl.png"));
         customerAccountButton.setImages(loadImage("darkCustomerAccount.png"),loadImage("darkCustomerAccount.png"),loadImage("lightCustomerAccount.png"));
       }else{
         settingsIcon.setImages(loadImage("lightSettingsIcon.png"),loadImage("lightSettingsIcon.png"),loadImage("lightSettingsIcon.png"));
         accountIcon.setImages(loadImage("lightAccountIcon.png"),loadImage("lightAccountIcon.png"),loadImage("lightAccountIcon.png"));
         liftControl.setImages(loadImage("lightLiftControl.png"),loadImage("lightLiftControl.png"),loadImage("darkLiftControl.png"));
         closeControl.setImages(loadImage("lightCloseControl.png"),loadImage("lightCloseControl.png"),loadImage("darkCloseControl.png"));
         customerAccountButton.setImages(loadImage("lightCustomerAccount.png"),loadImage("lightCustomerAccount.png"),loadImage("darkCustomerAccount.png"));
       }
    }
    
    //This is for all the text label.
    void buildLabelText(){
      accountLabel=cp5.addTextlabel("accountLabel")
      .setText("My manager account")
      .setPosition(26,0)
      .setFont(createFont("Berlin Sans FB",25))
      ;
      
      detailLabel=cp5.addTextlabel("detailLabel")
      .setText("Details of cars in the parking lot")
      .setPosition(150,120)
      .setFont(createFont("Berlin Sans FB",30))
      ;
      
      newCarsComingInLabel=cp5.addTextlabel("newCarsComingInLabel")
      .setText("New cars coming in")
      .setPosition(1350,120)
      .setFont(createFont("Berlin Sans FB",30))
      ;
      
      barrierControlLabel=cp5.addTextlabel("barrierControlLabel")
      .setText("Barrier Control")
      .setPosition(1380,700)
      .setFont(createFont("Berlin Sans FB",30))
      ;
 
      if(colorModeSwitch){
        accountLabel.setColor(darkModeFontColor);
        detailLabel.setColor(darkModeFontColor);
        newCarsComingInLabel.setColor(darkModeFontColor);
        barrierControlLabel.setColor(darkModeFontColor);
      }else{
        accountLabel.setColor(lightModeFontColor);
        detailLabel.setColor(lightModeFontColor);
        newCarsComingInLabel.setColor(lightModeFontColor);
        barrierControlLabel.setColor(lightModeFontColor);
      }
    }
    
    //is for title and subtitle
    void buildTitle(){
      cp5.addBang("")
        .setPosition(0,30)
        .setSize(1760,80)
        .setColorBackground(titleBackGround)
        .setLock(true)
        ;
      
       title=cp5.addTextlabel("title")
        .setText("PIMS")
        .setFont(createFont("Berlin Sans FB",80))
        .setPosition(790,25)
        .setColor(darkModeFontColor)
        ;
      
      subtitle=cp5.addTextlabel("subtitle")
        .setText("Parking lot Management System")
        .setFont(createFont("Berlin Sans FB",30))
        .setPosition(970,65)
        .setColor(darkModeFontColor)
        ;
    } 
}
         
//color switch function        
void Settings(int theValue){
      switch(theValue){
         case 1: 
           colorModeSwitch=true;
           view.buildBackGround();
           view.buildSettingSwitch();
           break;
         case 0:
           colorModeSwitch=false;
           view.buildBackGround();
           view.buildSettingSwitch();
           break;
      }
    }
