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
     int max_message=10000;
     JSONObject[] message=new JSONObject[max_message];
     Database(){}
     int max_message(){
       return max_message;
     }
}


// copy any JSON objects on disk into working memory
void refreshData() {
    File dir;
    File[] files;
    dir = new File(dataPath(""));
    files = dir.listFiles();
    JSONObject json;
    for (int i = 0; i <= files.length - 1; i++) {
      
        String path = files[i].getAbsolutePath();
        if (path.toLowerCase().endsWith(".json")) {
            json = loadJSONObject(path);
            if (json != null) {
                db.message[i] = json;
            }
        }
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
