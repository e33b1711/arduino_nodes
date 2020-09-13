#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

WiFiClient client;
int conn_error_count    = 0;
String message_buffer   = "";

void init_comm()
{
  
    // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  //WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  int retries=0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    retries++;
    if (retries==10){
        Serial.println("Getting no WIFI, rebooting...");
        ESP.restart();
    }
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

    
   //OTA
   ArduinoOTA.setHostname(unit_name);
   ArduinoOTA.setPassword(password);
   ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
      Serial.println("Start updating " + type);
    });
    ArduinoOTA.onEnd([]() {
      Serial.println("\nEnd");
      ESP.restart();
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) {
        Serial.println("Auth Failed");
      } else if (error == OTA_BEGIN_ERROR) {
        Serial.println("Begin Failed");
      } else if (error == OTA_CONNECT_ERROR) {
        Serial.println("Connect Failed");
      } else if (error == OTA_RECEIVE_ERROR) {
        Serial.println("Receive Failed");
      } else if (error == OTA_END_ERROR) {
        Serial.println("End Failed");
      }
    });
    ArduinoOTA.begin();
    


}


void send_message(String out_messageType, String out_address, int out_value){
  String message='!' + out_messageType + '!' + out_address + '!' + String(out_value, DEC) + "$\n";
  //client.println(message);
  //client.flush();
  message_buffer += message;
}


void handle_comm(){
  //OTA
  ArduinoOTA.handle();
  
  static long last_try=0;               // a string to hold incoming data
  static boolean messageComplete = false;        // whether the string is complete
  String in_messageType, addressString, valueString, inputString;
  int in_value;


  //reconnect Wifi eventually
  //with throttle to avoid blocking the unit!

  //check if still connected, eventually reconnect
  if ((!client.connected())) {
     Serial.println("handel_comm: trying to reconnect...");
      //reset parser state
      inputString = "";
      messageComplete = false;
      //try to reconnect
      if (client.connect(server, port)) {
        Serial.println("     ...success.");
      }else{
        Serial.println("     ...fail.");
      }  
  }

  
  // get a complete message
  while (client.available() && !(messageComplete)) {
    //get the new byte:
    char inChar = client.read(); 
    //add it to the inputString:
    inputString += inChar;
    if (inChar == '$') {
      messageComplete = true;
    } 
  }

  //message parser & handler
  while (messageComplete) {
    //message parser
    Serial.print("handle_comm: parsing message: ");
    Serial.println(inputString);
    
    int index1=inputString.indexOf('!');
    int index2=inputString.indexOf('!', index1+1);
    int index3=inputString.indexOf('!', index2+1);
    int index4=inputString.indexOf('$');

    //Serial.print("  " + inputString + " index1-4: ");
    //Serial.print(index1);
    //Serial.print("  ");
    //Serial.print(index2);
    //Serial.print("  ");
    //Serial.print(index3);
    //Serial.print("  ");
    //Serial.println(index4);
    
    in_messageType=inputString.substring(index1+1, index2);
    //Serial.println("  " + inputString);
    addressString=inputString.substring(index2+1,index3);
    //Serial.println("  " + addressString);
    valueString=inputString.substring(index3+1,index4);
    //Serial.println("  " + valueString);
    in_value=valueString.toInt();
    //Serial.print("  ");
    //Serial.println(in_value);

    //reset parser state
    inputString="";
    messageComplete= false;
    

    //message handler
    if (in_messageType=="post_all" && addressString==unit_name){
      post_all(); 
    }
    if (in_messageType=="r") {
      if (is_my_state(addressString)){
        send_message("i", addressString, address_to_value(addressString));
      }
    }
    if (in_messageType=="w") {   
      write_state_silent(addressString, in_value);
    }
  }

  //send messages
  if(client.connected()  && message_buffer != ""){
    client.print(message_buffer);
    Serial.println("handle_comm: Sending message_buffer...");
    Serial.print(message_buffer);
    message_buffer="";
  }
  
}


void post_all(){
  send_message("info", String(unit_name) + " posting all: start", 0);
  int i;
  for (i=0; i<num_l_states; i++){
    send_message("w", l_address[i], value_l[i]);
  }
  for (i=0; i<num_r_states; i++){
    send_message("w", r_address[i], value_r[i]);
  }
  for (i=0; i<num_s_states; i++){
    send_message("w", s_address[i], value_s[i]);
  }
  for (i=0; i<num_t_states; i++){
    send_message("w", t_address[i], value_t[i]);
    send_message("w", h_address[i], value_h[i]);
  }
  for (i=0; i<num_u_states; i++){
    send_message("w", u_address[i], value_u[i]);
  }
  send_message("info", String(unit_name) + " posting all: end", 0);
}
