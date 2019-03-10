
String message_buffer = "";

void send_message(String out_messageType, String out_address, int out_value){
  String message='!' + out_messageType + '!' + out_address + '!' + String(out_value, DEC) + "$\n";
  //client.println(message);
  //client.flush();
  message_buffer += message;
}

void message_handler(){

  //Serial.println("message_handler");

  //generic handler for in and out going messages

  static boolean messageComplete = false;        // whether the string is complete
  String in_messageType, addressString, valueString, inputString;
  int in_value;
  
  //reconnecting server / wifi here...

 Serial.print("message_handler: client.connected(): ");
 Serial.println(client.connected());
 Serial.print("message_handler: client.vailable(): ");
 Serial.println(client.available());

  // get a complete message
  while (client.available() && !(messageComplete)) {
    //get the new byte:
    char inChar = client.read();
    Serial.print(inChar);
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
    Serial.println(message_buffer);
    message_buffer="";
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
