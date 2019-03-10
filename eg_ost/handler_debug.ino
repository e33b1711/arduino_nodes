


void handle_debug(){

  static String inputString = "";                // a string to hold incoming data
  static boolean messageComplete = false;        // whether the string is complete
  String in_messageType, addressString, valueString;
  int in_value;

  //get one whole message from com
  while (Serial.available() && !(messageComplete)) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '$') {
messageComplete = true;
    } 
  }
  
  //message parser & handler
  while (messageComplete) {
    //message parser
    Serial.println("handle_debug: parsing message...:");
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

    //handler
    if (in_messageType=="w") {   
          write_state_silent(addressString, in_value);
    }
    else{
      post_all_debug();
    }
 
  }
}

void init_debug(){
  Serial.begin(9600);
  post_all_debug();
}


void post_all_debug(){
  Serial.println("unit adress: " + unit_name + ", posting all: start");
  int i;  
  for (i=0; i<num_l_states; i++){
    Serial.println(l_address[i] + " " +  value_l[i]);
  }
  for (i=0; i<num_r_states; i++){
    Serial.println(r_address[i] + " " +  value_r[i]);
  }
  for (i=0; i<num_s_states; i++){
    Serial.println(s_address[i] + " " +  value_s[i]);
  }
  for (i=0; i<num_t_states; i++){
    Serial.println(t_address[i] + " " +  value_t[i]);
    Serial.println(h_address[i] + " " +  value_h[i]);
  }
  for (i=0; i<num_u_states; i++){
    Serial.println(u_address[i] + " " +  value_u[i]);
  }
  Serial.println("posting all: end");
}
