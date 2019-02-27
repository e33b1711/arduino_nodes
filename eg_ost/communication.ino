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
  Serial.println("connecting...");
  // if you get a connection, report back via serial:
  if (client.connect(server, port)) {
    Serial.println("connected");
  } 
  else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
  // reserve 50 bytes for the inputString
  inputString.reserve(50);
}


void send_message(String out_messageType, String out_address, int out_value){
  String message='!' + out_messageType + '!' + out_address + '!' + String(out_value, DEC) + "$";
  client.println(message);
}


void handle_comm(){
  hear();
}


void hear(){
  if (!client.connected()) {
    Serial.println("No connection to server, trying to connect...");
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
  
  // message parser
  while (client.available() && !(messageComplete)) {

    // get the new byte:
    char inChar = (char)Serial1.read(); 
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '$') {
      messageComplete = true;
    } 
  }
  int i;
  while (messageComplete) {

    //message parser
    int index1=inputString.indexOf('!');
    int index2=inputString.indexOf('!', index1+1);
    int index3=inputString.indexOf('!', index2+1);
    int index4=inputString.indexOf('$');
    String in_messageType=inputString.substring(index1+1, index2);
    String addressString=inputString.substring(index2+1,index3);
    String valueString=inputString.substring(index3+1,index4);
    int in_value=valueString.toInt();

    //debbuging
    if (in_messageType!="t"){
      Serial.println("parsing message:");
      Serial.println(in_messageType);
      Serial.println(addressString);
      Serial.println(in_value);
    }
    //debbuging

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

    //reset parser state
    inputString="";
    messageComplete= false;
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




