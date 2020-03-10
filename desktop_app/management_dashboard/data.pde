//Different types of message
static abstract class MessageType{
    static final String PARKING = "parking";
    static final String REGISTER = "web_register";
    static final String LOGIN = "web_login";
    static final String VEHICLE = "web_vehicle";
    static final String FINANCE = "web_finance";
    static final String RECHARGE= "web_recharge";
}

//Database
private class Database{
     int max_massage=10000;
     JSONObject[] massage=new JSONObject[max_massage];
     Database(){}
     int max_massage(){
       return max_massage;
     }
}



public class MessageData{
  String makeUUID()
  {
    String result="";
    int strLen=32;
    String characters="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    int charactersLength=characters.length();
    for(int i=0;i<strLen;i++)
    {
      result += characters.charAt((int)Math.floor(Math.random() * charactersLength));
    }
     return result;
  }
  
   void saveMessageToDB(JSONObject message) {
        if (message == null) {
            return;
        } else {
            saveJSONObject(message, "data/" +makeUUID()+ ".json");
        }
    }
}
