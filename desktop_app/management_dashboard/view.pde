//Todo:
//1. bulid a couple of button to[add new],[edit],[delete],[find] user's message.
//2. bulid a tableView class.
//3. bulid a dinamic reflesh tableView for last ten records I/O
//4. bulid a class for data visualization.


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
    int colorModeSwitch=0;
    int darkModeBackGround=color(41,75,140);
    int lightModeBackGround=color(213,223,241);
    int darkModeFontColor=color(213,223,241);
    int lightModeFontColor=color(41,75,140);
    int darkModeInfoWindowColor=color(132,160,215);
    int lightModeInfoWindowColor=color(171,192,227);
    
    void build(){
      PFont p = createFont("Berlin Sans FB",16); 
      ControlFont font = new ControlFont(p);
      cp5.setFont(font);
      view.buildBackGround();
      view.buildSettingSwitch();
    }
    
    void buildBackGround(){ 
      if(colorModeSwitch==0){
        background(darkModeBackGround); 
      }else if(colorModeSwitch==1){
        background(lightModeBackGround);
      }
    }
    
    void buildSettingSwitch(){
       ScrollableList list = cp5.addScrollableList("Settings")
       .setPosition(1170,10)
       .setSize(100,60)
       .setItemHeight(20)
       .setBarHeight(20)
       .setBackgroundColor((colorModeSwitch==0)?darkModeInfoWindowColor:lightModeInfoWindowColor)
       .setColorBackground((colorModeSwitch==0)?darkModeBackGround:lightModeBackGround)
       .addItem("Light Mode",0)
       .addItem("Dark Mode",1)
       .setColorActive((colorModeSwitch==0)?lightModeInfoWindowColor:darkModeInfoWindowColor)
       ;
       list.open();
    }
    
   
}
