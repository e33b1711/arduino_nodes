void setup_r()
{
  Serial.println("setup_r");
  int i;
  for (i=0; i<num_r_states; i++){
    value_r[i]      = 0;
    aux_value_r[i]  = 0;
    lock_time_r[i]  = millis();
    send_state(r_address[i], value_r[i]);
  }  
}


void update_r()
{
  int i;
  for (i=0; i<num_r_states; i++){
    //reset trigger state to 0
     if (lock_time_r[i]+1000 < millis() and locked_r[i]){
        locked_r[i] = false;
        write_state(r_trigger_up[i],0);
        write_state(r_trigger_do[i],0);
     }

    //sensors => value 
    bool down = (value_b[r_down[i]]==1);
    bool up   = (value_b[r_up[i]]==1);
    value_r[i]=50;
    if (down & !up)  value_r[i]=100;
    if (!down & up)  value_r[i]=0; 
    if (aux_value_r[i] != value_r[i]){
      send_state(r_address[i], value_r[i]);
      aux_value_r[i] = value_r[i];
    }
    
  }  
}

    
void write_r(String address, int value){
  int i;
  for (i=0; i<num_r_states; i++){
    if (r_address[i]==address){
      Serial.println("write_r: match. ");
      if (lock_time_r[i]+lock_delay_r < millis()){
        if (value != value_r[i]){
          if (value == 0)   write_state(r_trigger_up[i],1);
          if (value == 100)  write_state(r_trigger_do[i],1);
          lock_time_r[i]=millis();
          locked_r[i] = true;
        }
        else{
          Serial.println("write_r: already this value. ");
        }
      }
      else{
      Serial.println("write_r: still locked. ");
      }
    }  
  }
}
