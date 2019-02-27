#include <SPI.h>
#include <Ethernet.h>


String inputString = "";                // a string to hold incoming data
boolean messageComplete = false;        // whether the string is complete
EthernetClient client;


//these are defined unit spefic file
//byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
//IPAddress ip(192,168,1,177);
//IPAddress server(1,1,1,1)
//int port = 8888;


void init_comm()
{
  // initialize
  Ethernet.begin(mac, ip);
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("init_comm: connecting...");
  if (client.connect(server, port)) {
    Serial.println("init_comm: connected");
    client.println("Arduino Node "+ unit_name + " started.$");
  } 
  else {
    // if you didn't get a connection to the server:
    Serial.println("init_comm: connection failed");
  }
  // reserve 50 bytes for the inputString
  inputString.reserve(50);
}


void send_message(String out_messageType, String out_address, int out_value){
  String message='!' + out_messageType + '!' + out_address + '!' + String(out_value, DEC) + "$";
  client.println(message);
}


void handle_comm(){
  
  //check if still connected, eventually reconnect 
  if (!client.connected()) {
    //how is the performance, without connection?
    Serial.println("handel_comm: no connection to server, trying to connect...");
    //reset parser state
    inputString = "";
    messageComplete = false;
    //try to reconnect
    if (client.connect(server, port)) {
      Serial.println("...success.");
    }else{
      Serial.println("...fail.");
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
    //parser
    int index1=inputString.indexOf('!');
    int index2=inputString.indexOf('!', index1+1);
    int index3=inputString.indexOf('!', index2+1);
    int index4=inputString.indexOf('$');
    String in_messageType=inputString.substring(index1+1, index2);
    String addressString=inputString.substring(index2+1,index3);
    String valueString=inputString.substring(index3+1,index4);
    int in_value=valueString.toInt();
   
    //reset parser state
    inputString="";
    messageComplete= false;

    /**
    if (in_messageType!="t"){
      Serial.println("handle_comm: parsing message done:");
      Serial.println("  " + in_messageType);
      Serial.println("  " + addressString);
      Serial.println("  " + in_value);
    }
    **/

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
}


void post_all(){
  client.println("!info!" + unit_name + "!posting all: start!0$");
  int i;
  for (i=0; i<num_d_states; i++){
    client.println("!w!" + d_address[i] + "!" + value_d[i] + "!0$");
  }
  for (i=0; i<num_l_states; i++){
    client.println("!w!" + l_address[i] + "!" + value_l[i] + "!0$");
  }
  for (i=0; i<num_r_states; i++){
    client.println("!w!" + r_address[i] + "!" + value_r[i] + "!0$");
  }
  for (i=0; i<num_t_states; i++){
    client.println("!w!" + t_address[i] + "!" + value_t[i] + "!0$");
    client.println("!w!" + h_address[i] + "!" + value_h[i] + "!0$");
  }
  for (i=0; i<num_u_states; i++){
    send_message("w", u_address[i], value_u[i]);
  }
  send_message("info", unit_name + " posting all: end", 0);
}




