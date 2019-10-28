#include <SPI.h>
#include <Ethernet.h>


EthernetClient client;

int conn_error_count = 0;
String message_buffer = "";

//these are defined unit spefic file
//byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
//IPAddress ip(192,168,1,177);
//IPAddress server(1,1,1,1)
//int port = 8888;


void init_comm()
{
  // initialize
  Ethernet.init(ethernet_sc_pin);
  Ethernet.begin(mac, ip, gateway, subnet);
  // give the Ethernet shield a second to initialize:
  delay(500);
  Serial.println("init_comm: connecting...");
  if (client.connect(server, port)) {
    Serial.println("init_comm: connected");
    client.println("Arduino Node "+ unit_name + " started.$");
  } 
  else {
    // if you didn't get a connection to the server:
    Serial.println("init_comm: connection failed");
  }
  // reserve 100 bytes for the message buffer
  message_buffer.reserve(100);
}


void send_message(String out_messageType, String out_address, int out_value){
  String message='!' + out_messageType + '!' + out_address + '!' + String(out_value, DEC) + "$\n";
  //client.println(message);
  //client.flush();
  message_buffer += message;
}


void handle_comm(){
  
  static long last_try=0;               // a string to hold incoming data
  static boolean messageComplete = false;        // whether the string is complete
  String in_messageType, addressString, valueString, inputString;
  int in_value;

  //trigger not conneted conndition
  /**static long last_keep_alive=millis();
  if (last_keep_alive+1000<millis()){
  client.print(" \b");
  last_keep_alive = millis();
  }**/

  //check if still connected, eventually reconnect
  static bool reconn_try = false;
  if ((!client.connected()) & !reconn_try) {
    reconn_try=true;
     Serial.println("handel_comm: trying to reconnect...");
      //reset parser state
      inputString = "";
      messageComplete = false;
      //try to reconnect
      if (client.connect(server, port)) {
        Serial.println("     ...success.");
        reconn_try=false;
      }else{
        Serial.println("     ...fail.");
      }  
  }

  //if still unconnected, try to recover each 20 secs
  if (!client.connected() & reconn_try) {
    if (last_try+20000<millis()){
      last_try = millis();
      Serial.println("handel_comm: trying to recover...");
      //reset parser state
      inputString = "";
      messageComplete = false;
      //try to reconnect
      Ethernet.init(ethernet_sc_pin);
      Ethernet.begin(mac, ip);
      delay(500);
      if (client.connect(server, port)) {
        Serial.println("     ...success.");
        //clear in / out buffer
        while (client.available()){ char inChar = client.read(); };
        message_buffer="";
        reconn_try=false;
      }else{
        Serial.println("     ...fail.");
      }  
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
   send_message("info", unit_name + " posting all: start", 0);
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
  send_message("info", unit_name + " posting all: end", 0);
}
