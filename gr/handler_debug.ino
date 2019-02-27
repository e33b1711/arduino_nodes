String db_inputString = "";                // a string to hold incoming data
boolean db_messageComplete = false;        // whether the string is complete


void handle_debug(){

  //get one whole message from com
  while (Serial.available() && !(db_messageComplete)) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    db_inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '$') {
      db_messageComplete = true;
    } 
  }
  
  //message parser & handler
  while (db_messageComplete) {
    //message parser
    int index1=db_inputString.indexOf('!');
    int index2=db_inputString.indexOf('!', index1+1);
    int index3=db_inputString.indexOf('!', index2+1);
    int index4=db_inputString.indexOf('$');
    String in_messageType=db_inputString.substring(index1+1, index2);
    String addressString=db_inputString.substring(index2+1,index3);
    String valueString=db_inputString.substring(index3+1,index4);
    int in_value=valueString.toInt();

    //reset parser state
    db_inputString="";
    db_messageComplete= false;

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
  for (i=0; i<num_do_states; i++){
  Serial.println(do_address[i] + " " +  value_do[i]);
  }
  for (i=0; i<num_t_states; i++){
      Serial.println(t_address[i] + " " +  value_t[i]);
      Serial.println(h_address[i] + " " +  value_h[i]);
  }
  Serial.println("posting all: end");
}
