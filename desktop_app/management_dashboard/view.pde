//Todo:
//1. bulid two tableView class.
//2. bulid a dinamic reflesh tableView for last ten records I/O
//3. bulid a class for data visualization.

//color mode
final int darkModeBackground=color(41, 75, 140);
final int lightModeBackground=color(213, 223, 241);
final int darkModeFontColor=color(213, 223, 241);
final int lightModeFontColor=color(41, 75, 140);
final int darkModeInfoWindowColor=color(132, 160, 215);
final int lightModeInfoWindowColor=color(171, 192, 227);
final int titleBackground=color(87, 106, 195);
final int settingsBackground=color(185, 195, 230);
final int darkModeInfoBackground=color(150,165,230);
final int lightModeInfoBackground=color(185,195,230);
//dark/light mode switch
boolean colorModeSwitch=true;
boolean firstBoot=true;
Chart pieChart;
Chart biChart;
ScrollableList list,newCarsComingList,detailList;
Textlabel timer, accountLabel, title, subtitle, detailLabel, newCarsComingInLabel, barrierControlLabel,newCarsComingInFirstLine
,detailFirstLine;
Button accountIcon,settingsIcon,liftControl,closeControl,customerAccountButton;
Textarea infoTextarea;

final int barrierId=12345;

final int totalSpaces = 50; // The total parking spaces in the parking lot
int accuProfit = 0; // Accumulated profit of the day

JSONArray newCarsComingArray;

void refreshDashboardData() {
  // We just rebuild the view rather than updating existing
  updateDashboardData();
}

void updateDashboardData() {
  refreshData();
  surface.setTitle("Parking IOT Management System");
  
  //view.test();
  view.bulidNewCarsComingList();
  view.buildDetailList();
  firstBoot=false;
  view.buildCharts();

}

public class Dashboard_view {
  
  //build() is for those complement won't change color with color switch
  void build() {
    //set Berlin Sans FB as Font
    PFont p = createFont("Berlin Sans FB", 20); 
    ControlFont font = new ControlFont(p);
    cp5.setFont(font);

    view.buildTitle();
    view.buildSettingSwitch();
    view.buildButton();
    view.buildLabelText();
    view.buildTimer();
  }

  void buildBackground() { 
    if (colorModeSwitch) {
      background(darkModeBackground);
    } else {
      background(lightModeBackground);
    }
    circle(250, 825, 245);
    stroke(255);
    strokeWeight(8);
  }

  //color switch
  void buildSettingSwitch() {
    list = cp5.addScrollableList("Settings")
              .setPosition(1638, 10)
              .setSize(120, 120)
              .setItemHeight(40)
              .setBarHeight(20)
              .setColorForeground((colorModeSwitch)?darkModeBackground:lightModeBackground)
              .setColorBackground((colorModeSwitch)?darkModeBackground:lightModeBackground)
              .addItem("Light Mode", 0)
              .addItem("Dark Mode", 1)
              .close()
              ;

    if (colorModeSwitch) {
      list.setColorLabel(darkModeFontColor)
      .setColorValue(darkModeFontColor);
    } else {
      list.setColorLabel(lightModeFontColor).setColorValue(lightModeFontColor);
    }
  }
  void changeColorSettingSwitch(){
    list.setColorForeground((colorModeSwitch)?darkModeBackground:lightModeBackground)
        .setColorBackground((colorModeSwitch)?darkModeBackground:lightModeBackground);
     if (colorModeSwitch) {
      list.setColorLabel(darkModeFontColor)
      .setColorValue(darkModeFontColor);
    } else {
      list.setColorLabel(lightModeFontColor).setColorValue(lightModeFontColor);
    }
  }
  

  //Timer on the middle head
  void buildTimer() {   
    timer= cp5.addTextlabel("timer")
              .setFont(createFont("Berlin Sans FB", 20))
              .setPosition(840, 0);

    if (colorModeSwitch) {
      timer.setColorValue(darkModeFontColor);
    } else {
      timer.setColorValue(lightModeFontColor);
    }
  }
  void changeColorTimer(){
    if (colorModeSwitch) {
      timer.setColorValue(darkModeFontColor);
    } else {
      timer.setColorValue(lightModeFontColor);
    }
  }

  //All the image are saved in ~/data/.. This function is for all the image button.
  void buildButton() {
    accountIcon =cp5.addButton("accountIcon")
                            .setPosition(5, 5)
                            ;
    settingsIcon=cp5.addButton("settingsIcon")
                            .setPosition(1615, 5)
                            ;
    liftControl=cp5.addButton("liftControl")
                          .setPosition(1330, 750)
                          .setSize(132,130)
                          .updateSize();

    closeControl=cp5.addButton("closeControl")
                           .setPosition(1500, 750)
                           .setSize(132,130)
                           .updateSize();

    customerAccountButton=cp5.addButton("customerAccountButton")
                                    .setPosition(1325, 900);

    if (colorModeSwitch) {
      accountIcon.setImages(loadImage("darkAccountIcon.png"), loadImage("darkAccountIcon.png"), loadImage("darkAccountIcon.png"));
      settingsIcon.setImages(loadImage("darkSettingsIcon.png"), loadImage("darkSettingsIcon.png"), loadImage("darkSettingsIcon.png"));
      liftControl.setImages(loadImage("darkLiftControl.png"), loadImage("darkLiftControl.png"), loadImage("lightLiftControl.png"));
      closeControl.setImages(loadImage("darkCloseControl.png"), loadImage("darkCloseControl.png"), loadImage("lightCloseControl.png"));
      customerAccountButton.setImages(loadImage("darkCustomerAccount.png"), loadImage("darkCustomerAccount.png"), loadImage("lightCustomerAccount.png"));
    } else {
      settingsIcon.setImages(loadImage("lightSettingsIcon.png"), loadImage("lightSettingsIcon.png"), loadImage("lightSettingsIcon.png"));
      accountIcon.setImages(loadImage("lightAccountIcon.png"), loadImage("lightAccountIcon.png"), loadImage("lightAccountIcon.png"));
      liftControl.setImages(loadImage("lightLiftControl.png"), loadImage("lightLiftControl.png"), loadImage("darkLiftControl.png"));
      closeControl.setImages(loadImage("lightCloseControl.png"), loadImage("lightCloseControl.png"), loadImage("darkCloseControl.png"));
      customerAccountButton.setImages(loadImage("lightCustomerAccount.png"), loadImage("lightCustomerAccount.png"), loadImage("darkCustomerAccount.png"));
    }
    
  }
  
  void changeColorButton(){
    if (colorModeSwitch) {
      accountIcon.setImages(loadImage("darkAccountIcon.png"), loadImage("darkAccountIcon.png"), loadImage("darkAccountIcon.png"));
      settingsIcon.setImages(loadImage("darkSettingsIcon.png"), loadImage("darkSettingsIcon.png"), loadImage("darkSettingsIcon.png"));
      liftControl.setImages(loadImage("darkLiftControl.png"), loadImage("darkLiftControl.png"), loadImage("lightLiftControl.png"));
      closeControl.setImages(loadImage("darkCloseControl.png"), loadImage("darkCloseControl.png"), loadImage("lightCloseControl.png"));
      customerAccountButton.setImages(loadImage("darkCustomerAccount.png"), loadImage("darkCustomerAccount.png"), loadImage("lightCustomerAccount.png"));
    } else {
      settingsIcon.setImages(loadImage("lightSettingsIcon.png"), loadImage("lightSettingsIcon.png"), loadImage("lightSettingsIcon.png"));
      accountIcon.setImages(loadImage("lightAccountIcon.png"), loadImage("lightAccountIcon.png"), loadImage("lightAccountIcon.png"));
      liftControl.setImages(loadImage("lightLiftControl.png"), loadImage("lightLiftControl.png"), loadImage("darkLiftControl.png"));
      closeControl.setImages(loadImage("lightCloseControl.png"), loadImage("lightCloseControl.png"), loadImage("darkCloseControl.png"));
      customerAccountButton.setImages(loadImage("lightCustomerAccount.png"), loadImage("lightCustomerAccount.png"), loadImage("darkCustomerAccount.png"));
    }
  }

  //This is for all the text label.
  void buildLabelText() {
    accountLabel=cp5.addTextlabel("accountLabel")
                    .setText("My manager account")
                    .setPosition(26, 0)
                    .setFont(createFont("Berlin Sans FB", 25))
                    ;

    detailLabel=cp5.addTextlabel("detailLabel")
                   .setText("Details of cars in the parking lot")
                   .setPosition(150, 120)
                   .setFont(createFont("Berlin Sans FB", 30))
                   ;

    newCarsComingInLabel=cp5.addTextlabel("newCarsComingInLabel")
                            .setText("New cars coming in")
                            .setPosition(1350, 120)
                            .setFont(createFont("Berlin Sans FB", 30))
                            ;

    barrierControlLabel=cp5.addTextlabel("barrierControlLabel")
                           .setText("Barrier Control")
                           .setPosition(1380, 700)
                           .setFont(createFont("Berlin Sans FB", 30))
                           ;
                           
    newCarsComingInFirstLine=cp5.addTextlabel("newCarsComingInFirstLine")
                                  .setText("Entrance               |Time")
                                  .setPosition(1250, 180)
                                  .setFont(createFont("Berlin Sans FB", 22))
                                  ;
                                  
    detailFirstLine=cp5.addTextlabel("detailFirstLine")
                                  .setText("ID                 |Username                |Car     ")
                                  .setPosition(150, 180)
                                  .setFont(createFont("Berlin Sans FB", 22))
                                  ;
    
    infoTextarea=cp5.addTextarea("info")
                  .setPosition(725,250)
                  .setSize(400,400)
                  .setFont(createFont("Berlin Sans FB", 25))
                  .setLineHeight(22)
                  .setColor(color(1));
                  
                                  

    if (colorModeSwitch) {
      accountLabel.setColor(darkModeFontColor);
      detailLabel.setColor(darkModeFontColor);
      newCarsComingInLabel.setColor(darkModeFontColor);
      barrierControlLabel.setColor(darkModeFontColor);
      newCarsComingInFirstLine.setColor(darkModeFontColor);
      infoTextarea.setColorBackground(darkModeInfoBackground);
      infoTextarea.setColor(darkModeFontColor);
      detailFirstLine.setColor(darkModeFontColor);
    } else {
      accountLabel.setColor(lightModeFontColor);
      detailLabel.setColor(lightModeFontColor);
      newCarsComingInLabel.setColor(lightModeFontColor);
      barrierControlLabel.setColor(lightModeFontColor);
      newCarsComingInFirstLine.setColor(lightModeFontColor);
      infoTextarea.setColor(lightModeInfoBackground);
      infoTextarea.setColor(lightModeFontColor);
      detailFirstLine.setColor(lightModeFontColor);
    }
  }

  //is for title and subtitle
  void buildTitle() {
    cp5.addBang("")
       .setPosition(0, 30)
       .setSize(1760, 80)
       .setColorBackground(titleBackground)
       .setLock(true)
       ;

    title=cp5.addTextlabel("title")
             .setText("PIMS")
             .setFont(createFont("Berlin Sans FB", 80))
             .setPosition(790, 25)
             .setColor(darkModeFontColor)
             ;

    subtitle=cp5.addTextlabel("subtitle")
                .setText("Parking lot Management System")
                .setFont(createFont("Berlin Sans FB", 30))
                .setPosition(970, 65)
                .setColor(darkModeFontColor)
                ;
                
  }
  
  void changeColorLabel(){
    if (colorModeSwitch) {
      accountLabel.setColor(darkModeFontColor);
      detailLabel.setColor(darkModeFontColor);
      newCarsComingInLabel.setColor(darkModeFontColor);
      barrierControlLabel.setColor(darkModeFontColor);
      newCarsComingInFirstLine.setColor(darkModeFontColor);
      infoTextarea.setColorBackground(darkModeInfoBackground);
      infoTextarea.setColor(darkModeFontColor);
      detailFirstLine.setColor(darkModeFontColor);
    } else {
      accountLabel.setColor(lightModeFontColor);
      detailLabel.setColor(lightModeFontColor);
      newCarsComingInLabel.setColor(lightModeFontColor);
      barrierControlLabel.setColor(lightModeFontColor);
      newCarsComingInFirstLine.setColor(lightModeFontColor);
      infoTextarea.setColor(lightModeInfoBackground);
      infoTextarea.setColor(lightModeFontColor);
      detailFirstLine.setColor(lightModeFontColor);
    }
  }
  
  void buildCharts() {
    view.buildPieChart();
    view.buildLineChart();
    
  }
  
  void buildPieChart() {
    int allCarsInLotCount = getAllCarsInLotCount();
    pieChart = cp5.addChart("Occupancy")
                .setPosition(125, 700)
                .setSize(250, 250)
                .setRange(-20, 20)
                .setView(Chart.PIE)
                ;
  
    pieChart.addDataSet("world");
    pieChart.setColors("world", 
                       color(255, 0, 0), 
                       (colorModeSwitch)?darkModeBackground:lightModeBackground, 
                       (colorModeSwitch)?lightModeBackground:darkModeBackground);
    
    
    pieChart.setData("world", allCarsInLotCount, totalSpaces-allCarsInLotCount);
    println("number of cars "+allCarsInLotCount);
  }
  
  void buildLineChart() {
    totalProfit();
  }
  
  /*void test(){
     JSONArray array=getNewCarsComingListFromDb();
     if(array.size()>0){
       for(int i=0;i<array.size();i++){
         JSONObject o=array.getJSONObject(i).getJSONObject("info");
         println(o.getString("time_in"));
       }
     }
     
  }*/
 
  
  
  void bulidNewCarsComingList(){
    newCarsComingArray=getNewCarsComingListFromDb();
    newCarsComingList=cp5.addScrollableList("newRecord")
                         .setPosition(1250,211)
                         .setSize(450, 500)
                         .setItemHeight(40)
                         .setBarHeight(30)
                         .open()
                         .show()
                         .bringToFront()
                         .unregisterTooltip();
                                
     if(newCarsComingArray.size()>0){
       for(int i=0;i<newCarsComingArray.size();i++){
         JSONObject o=newCarsComingArray.getJSONObject(i).getJSONObject("info");
         newCarsComingList.addItem(o.getString("barrier_type")+((o.getString("barrier_type").equals("in"))?"    ":"")
         +"                         |"+o.getString("time_in"),i);
       }
     }
  }
  
  void buildDetailList(){
    detailList=cp5.addScrollableList("detailList")
                           .setPosition(150,211)
                           .setSize(450, 500)
                           .setItemHeight(40)
                           .setBarHeight(30)
                           .open()
                           .show()
                           .bringToFront()
                           .unregisterTooltip();
    
  }
  
}

void newRecord(int theValue){
    JSONObject o=newCarsComingArray.getJSONObject(theValue).getJSONObject("info");
    Pattern p=Pattern.compile("-");
    String[] entryTime=null;
    String[] exitTime=null;
    int balance=0;
    
    if(o.getString("time_in")!=null){
      entryTime=p.split(o.getString("time_in"));
    }
    if(o.getString("time_out")!=null){
      exitTime=p.split(o.getString("time_out"));
    }
    if(getObjWithUsername("web_finance",o.getString("username"))!=null)
    {
      balance= getObjWithUsername("web_finance",o.getString("username")).getJSONObject("info").getInt("balance");
    }
    infoTextarea.setText("Vehicle details (hover over to see)\n\n"
                    +"Entrance #:                  "+o.getString("")+"\n"
                    +"ID                                  "+o.getString("vehicle_id")+"\n"
                    +"Entry date:                   "+entryTime[2]+"\\"+entryTime[1]+"\\"+entryTime[0]+"\n"
                    +"Entry time:                   "+entryTime[3]+":"+entryTime[4]+":"+entryTime[5]+"\n"
                    +"Exit date:                      "+((o.getString("time_out")!=null)?(exitTime[2]+"\\"+exitTime[1]+"\\"+exitTime[0]):"")+"\n"
                    +"Exit time:                      "+((o.getString("time_out")!=null)?(exitTime[3]+"\\"+exitTime[4]+"\\"+exitTime[5]):"")+"\n"
                    +"Account owner:            "+o.getString("username")+"\n"
                    +"Car:                               "+o.getString("vehicle_id")+"\n"
                    +"Account balance:         £"+balance+"\n"
                    +"Annual membership: "+"\n"                   
                    );
              //<>//
}
  

//color switch function        
void Settings(int theValue) {
  switch(theValue) {
  case 1: 
    colorModeSwitch=true;
    view.changeColorSettingSwitch();
    view.changeColorButton();
    view.changeColorLabel();
    view.changeColorTimer();
    break;
  case 0:
    colorModeSwitch=false;
    view.changeColorSettingSwitch();
    view.changeColorButton();
    view.changeColorLabel();
    view.changeColorTimer();
    break;
  }
}


public void liftControl(){
    if(!firstBoot){
      barrierOpen(barrierId);
      //print(barrierId);
    }
}

public void closeControl(){
    if(!firstBoot){
      barrierClose(barrierId);
      //print(barrierId);
    }
}
