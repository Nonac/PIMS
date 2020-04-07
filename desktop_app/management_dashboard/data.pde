//Different types of message
static abstract class MessageType{
    static final String PARKING = "parking";
    static final String REGISTER = "reg_info";
    static final String LOGIN = "login_info";
    static final String VEHICLE = "vehicle_reg_info";
    static final String FINANCE = "web_finance";
    static final String RECHARGE= "web_recharge";
    static final String CHARGE= "charge_info";
    
    
    
    
    static final String USER_REGISTER = "web_register";
    static final String USER_LOGIN = "web_login";
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

void deleteFile(String path){
    File f = new File(path);
    if(f.exists()){
      f.delete();
    }
}
  
//get a JSONObject from disk according to datatype and userId
  JSONObject getObjWithId(String datatype , String userId)
  {
       JSONObject res=null;
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
  
  
  //get a JSONObject from disk according to datatype and username
  JSONObject getObjWithUsername(String datatype , String username)
  {
       JSONObject res=null;
       for(JSONObject message:db.messages)
       {
         if(message!=null&&message.getString("data_type").equals(datatype))
         {
           JSONObject info = message.getJSONObject("info");
           if(info.getString("username").equals(username))
             {
               res=message;
             }
         }
       }
       return res;
  }

//Provide API for view, m5stack and web
public class MessageData{
  /* Do not use makeUUID as a part of name in file, it is useless and impossible to find and delete file
     the file name 's format is  "data/"+message.getString("data_type") +"_"+ message.getJSONObject("info").getString("username") + ".json"
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
  }*/
  //receive register message from web
   JSONObject receiveRegisterFromWeb(JSONObject message) {
        if (message == null) {
            return null;
        }
        JSONObject res = message;
        //file's name should have datatype identifier since it is useful for searching
        if(getObjWithUsername("web_register", message.getJSONObject("info").getString("username"))==null){
            saveJSONObject(message, "data/"+message.getString("data_type") +"_"+ message.getJSONObject("info").getString("username") + ".json");
            refreshData();
            res.getJSONObject("info").setInt("status",1);  
            return res;
        }
        else {
            res.getJSONObject("info").setInt("status",0);  
            return res;
        }
    }
    
   //send message to web to allow access or not
   JSONObject receiveLoginFromWeb(JSONObject loginMessage){
         //String userId=loginMessage.getString("user_id");
         // use reg_info object to check whether password is correct
         String datatype="web_register";
         JSONObject infoFromWeb = loginMessage.getJSONObject("info");
         String username = infoFromWeb.getString("username");
         JSONObject obj= getObjWithUsername(datatype,username);
         if(obj!=null&&obj.getJSONObject("info").getString("password").equals(infoFromWeb.getString("password")))
         {
            //loginMessage.setInt("access",1);
            infoFromWeb.setInt("status",1);
        }
        else {
            //loginMessage.setInt("access",0);
            infoFromWeb.setInt("status",0);
        }
          
         return loginMessage;
       }
       
       //receive finance message from web and sent validation message back
   JSONObject receiveFinanceFromWeb(JSONObject financeMessage){
         if (financeMessage == null) {
            return null;
          }
        if(getObjWithUsername("web_finance", financeMessage.getJSONObject("info").getString("username"))==null){
            JSONObject info = financeMessage.getJSONObject("info");
            info.setInt("balance",0);
            info.setString("currency","GBP");
            info.setInt("status",1);
            saveJSONObject(financeMessage, "data/"+financeMessage.getString("data_type") + "_"+ financeMessage.getJSONObject("info").getString("username") + ".json");
            refreshData();
            return financeMessage;
          }
          JSONObject res = getObjWithUsername("web_finance", financeMessage.getJSONObject("info").getString("username"));
          res.getJSONObject("info").setInt("status",1);

          return res;
        }
       
   JSONObject receiveRechargeFromWeb(JSONObject rechargeMessage){
         JSONObject infoFromWeb = rechargeMessage.getJSONObject("info");
         String username = infoFromWeb.getString("username");
         JSONObject financeMessage= getObjWithUsername(MessageType.FINANCE,username);
         if(financeMessage==null){
           rechargeMessage.getJSONObject("info").setInt("status",0);
           return rechargeMessage;
         }
         
         int rechargeAmount = rechargeMessage.getJSONObject("info").getInt("pay_amount");
         int balance = financeMessage.getJSONObject("info").getInt("balance");
         balance = balance+ rechargeAmount;
         financeMessage.getJSONObject("info").setInt("balance",balance);
   
         String path = dataPath("")+"\\"+MessageType.FINANCE+"_"+financeMessage.getJSONObject("info").getString("username")+".json";
         deleteFile(path);
         saveJSONObject(financeMessage, "data/"+financeMessage.getString("data_type") + "_"+ financeMessage.getJSONObject("info").getString("username") + ".json");
         refreshData();
         rechargeMessage.getJSONObject("info").setInt("balance",balance);
         rechargeMessage.getJSONObject("info").setInt("status",1);
         return rechargeMessage;
       }
}        
