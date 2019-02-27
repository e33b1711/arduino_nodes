// state schreiben, egal ob auf dieser unit oder anderer
void write_state(String address, int value){
  send_message("w", address, value);
  write_state_silent(address, value);
}

// state schreiben, wenn auf unit
void write_state_silent(String address, int value){
//  //debbuging
//    Serial.println("in function write silent");
//    Serial.println(address);
//    //debbuging
  write_d(address, value);
  write_t(address, value);
  write_h(address, value);
  write_u(address, value);
  write_l(address, value);
  write_r(address, value);
  write_s(address, value);
}


// Achtung: funktioniert nur bei eigenen States (auf der selben unit)
void toggle_state(String address){
  int value = address_to_value(address);
  if (value==0){
    write_state(address, 1);
  }
  else{
    write_state(address, 0);
  }
}

int address_to_value(String address){
  int i;
  int value;
  for (i=0; i<num_d_states; i++){
    if (d_address[i]==address){
      return value_d[i];
    }
  }
  for (i=0; i<num_t_states; i++){
    if (t_address[i]==address){
      return value_t[i];
    }
  }
  for (i=0; i<num_h_states; i++){
    if (h_address[i]==address){
      return value_h[i];
    }
  }
  for (i=0; i<num_u_states; i++){
    if (u_address[i]==address){
      return value_u[i];
    }
  }
  for (i=0; i<num_l_states; i++){
    if (l_address[i]==address){
      return value_l[i];
    }
  }
  for (i=0; i<num_r_states; i++){
    if (r_address[i]==address){
      return value_r[i];
    }
  }
  for (i=0; i<num_s_states; i++){
    if (s_address[i]==address){
      return value_s[i];
    }
  }
  
}

//checket ob es diesen state auf dieser unit gibt
boolean is_my_state(String address){
   int i;
  for (i=0; i<num_d_states; i++){
    if (d_address[i]==address){
      return true;
    }
  }
  for (i=0; i<num_t_states; i++){
    if (t_address[i]==address){
      return true;
    }
  }
  for (i=0; i<num_h_states; i++){
    if (h_address[i]==address){
      return true;
    }
  }
  for (i=0; i<num_u_states; i++){
    if (u_address[i]==address){
      return true;
    }
  }
  for (i=0; i<num_l_states; i++){
    if (l_address[i]==address){
      return true;
    }
  }
  for (i=0; i<num_r_states; i++){
    if (r_address[i]==address){
      return true;
    }
  }
  for (i=0; i<num_s_states; i++){
    if (s_address[i]==address){
      return true;
    }
  }
  return false;
}
