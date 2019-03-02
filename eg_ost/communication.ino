#include <SPI.h>
#include <Ethernet.h>


EthernetClient client;


//these are defined unit spefic file
//byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
//IPAddress ip(192,168,1,177);
//IPAddress server(1,1,1,1)
//int port = 8888;


void init_comm()
{
  // initialize
  Ethernet.init(ethernet_sc_pin);
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
  //inputString.reserve(50);
}


void send_message(String out_messageType, String out_address, int out_value){
  String message='!' + out_messageType + '!' + out_address + '!' + String(out_value, DEC) + "$";
  client.println(message);
  client.flush();
}


void handle_comm(){

  static String inputString = "";                // a string to hold incoming data
  static boolean messageComplete = false;        // whether the string is complete
  String in_messageType, addressString, valueString;
  int in_value;
  
  //check if still connected, eventually reconnect 
  if (!client.connected()) {
    //how is the performance, without connection?
    Serial.println("handel_comm: no connection to server, trying to connect...");
    //reset parser state
    inputString = "";
    messageComplete = false;
    //try to reconnect
    Ethernet.init(ethernet_sc_pin);
    Ethernet.begin(mac, ip);
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
    //message parser
    Serial.print("handle_debug: parsing message...:");
    
    int index1=inputString.indexOf('!');
    int index2=inputString.indexOf('!', index1+1);
    int index3=inputString.indexOf('!', index2+1);
    int index4=inputString.indexOf('$');

    Serial.print("  " + inputString + " index1-4: ");
    Serial.print(index1);
    Serial.print("  ");
    Serial.print(index2);
    Serial.print("  ");
    Serial.print(index3);
    Serial.print("  ");
    Serial.println(index4);
    
    in_messageType=inputString.substring(index1+1, index2);
    Serial.println("  " + inputString);
    addressString=inputString.substring(index2+1,index3);
    Serial.println("  " + addressString);
    valueString=inputString.substring(index3+1,index4);
    Serial.println("  " + valueString);
    in_value=valueString.toInt();
    Serial.print("  ");
    Serial.println(in_value);

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
}


void post_all(){
  client.println("!info!" + unit_name + "!posting all: start!0$");
  int i;
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
