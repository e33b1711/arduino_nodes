// state schreiben, egal ob auf dieser unit oder anderer
void write_state(String address, int value){
  write_p(address, value);
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
  for (i=0; i<num_t_states; i++){
    if (t_address[i]==address){
      return value_t[i];
    }
  }
  for (i=0; i<num_p_states; i++){
    if (p_address[i]==address){
      return value_p[i];
    }
  }
  
}

//checket ob es diesen state auf dieser unit gibt
boolean is_my_state(String address){
   int i;
  for (i=0; i<num_t_states; i++){
    if (t_address[i]==address){
      return true;
    }
  }
  for (i=0; i<num_p_states; i++){
    if (p_address[i]==address){
      return true;
    }
  }
  return false;
}
