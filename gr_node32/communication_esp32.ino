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

void send_command(String out_address, int out_value){
  String message="!c!" + out_address + '!' + String(out_value, DEC) + "$\n";
  //client.println(message);
  //client.flush();
  message_buffer += message;
}

void send_state(String out_address, int out_value){
  String message="!s!" + out_address + '!' + String(out_value, DEC) + "$\n";
  //client.println(message);
  //client.flush();
  message_buffer += message;
}

void send_state(String out_address, String out_value){
  String message="!s!" + out_address + '!' + out_value + "$\n";
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
  static int wifi_retry = 0;
  if(WiFi.status() != WL_CONNECTED) {
      wifi_retry++;
      Serial.println("WiFi not connected. Try to reconnect");
      WiFi.disconnect();
      WiFi.mode(WIFI_OFF);
      WiFi.mode(WIFI_STA);
      WiFi.begin(ssid, pass);
      delay(100);
  }
  if(WiFi.status() != WL_CONNECTED && wifi_retry >= 5) {
      Serial.println("\nReboot");
      ESP.restart();
  }
  if(WiFi.status() == WL_CONNECTED){
      wifi_retry = 0;
  }
 

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
    if (valueString=="STOP")   {in_value=50;};
    if (valueString=="UP")     {in_value=0;};
    if (valueString=="DOWN")   {in_value=100;};
    if (valueString=="ON")     {in_value=1;};
    if (valueString=="OFF")    {in_value=0;};

    
    //reset parser state
    inputString="";
    messageComplete= false;
    

    //message handler
    if (in_messageType=="post_all" && addressString==unit_name){
      post_all(); 
    }
    if (in_messageType=="restart" && addressString==unit_name){
      send_state(String(unit_name) + " restarting", 0);
      ESP.restart();
    }
    if (in_messageType=="r") {
      if (is_my_state(addressString)){
        send_state(addressString, address_to_value(addressString));
      }
    }
    if (in_messageType=="c") {   
      write_state(addressString, in_value);
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
  send_state(String(unit_name) + " posting all: start", 0);
  int i;
  for (i=0; i<num_l_states; i++){
    send_state(l_address[i], value_l[i]);
  }
  for (i=0; i<num_r_states; i++){
    send_state(r_address[i], value_r[i]);
  }
  for (i=0; i<num_s_states; i++){
    send_state(s_address[i], value_s[i]);
  }
  for (i=0; i<num_u_states; i++){
    send_state(u_address[i], value_u[i]);
  }
  send_state(String(unit_name) + " posting all: end", 0);
}
