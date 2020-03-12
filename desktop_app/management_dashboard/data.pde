//Different types of message
static abstract class MessageType{
    static final String PARKING = "parking";
    static final String REGISTER = "reg_info";
    static final String LOGIN = "login_info";
    static final String VEHICLE = "vehicle_reg_info";
    static final String FINANCE = "finance_info";
    static final String RECHARGE= "top_up_info";
    static final String CHARGE= "charge_info";
}

//Database
private class Database{
     int max_message=10000;
     JSONObject[] messages=new JSONObject[max_message];
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
                db.messages[i] = json;
            }
        }
    }
}



//get a JSONObject from disk according to datatype and userId
/*JSONObject getObjWithId(String datatype , String userId){
    File dir;
    File[] files;
    dir = new File(dataPath(""));
    files = dir.listFiles();
    JSONObject json;
    for (int i = 0; i <= files.length - 1; i++) {
      
        String path = files[i].getAbsolutePath();
        if (path.toLowerCase().startsWith(datatype)) {
            json = loadJSONObject(path);
            if(json.get("datatype")==datatype){
              JSONObject message = (JSONObject) json.get("reg_info");
              if(message.get("userId").equals(userId)) return json;
            }
        }
    }
    return null;
}*/
  
//get a JSONObject from disk according to datatype and userId
  JSONObject getObjWithId(String datatype , String userId)
  {
       JSONObject res=new JSONObject();
       for(JSONObject message:db.messages)
       {
         if(message!=null&&message.getString("data_type").equals(datatype))
         {
           if(message.getString("user_id").equals(userId))
             {
               res=message;
             }
         }
       }
       return res;
  }
  

//Provide API for view, m5stack and web
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
  //receive message from web
   void saveMessageToDB(JSONObject message) {
        if (message == null) {
            return;
        } else {
          //file's name should have datatype identifier since it is useful for searching
            saveJSONObject(message, "data/" +message.get("data_type") + makeUUID()+ ".json");
        }
    }
  //send message to web to allow access or not
 /* JSONObject sendComfirmInfoToWeb(JSONObject loginMessage){
    JSONObject comfirmMessage;
    JSONObject temp = (JSONObject)loginMessage.get("reg_info");
    comfirmMessage = getObjWithId("web_register",temp.get("userId").toString());
    if(comfirmMessage == null) return null;
    //and set!!!
    return comfirmMessage;
  }*/
     //send message to web to allow access or not
    JSONObject sendConfirmInfoToWeb(JSONObject loginMessage){
         String userId=loginMessage.getString("user_id");
         String datatype=loginMessage.getString("data_type");
         JSONObject obj= getObjWithId(datatype,userId);
         if(obj.getString("password").equals(loginMessage.getString("password")))
         {
            loginMessage.setBoolean("access",true);
        }
         return loginMessage;
        }
}        
