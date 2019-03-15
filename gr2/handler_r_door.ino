void setup_r()
{
  Serial.println("setup_r");
  int i;
  for (i=0; i<num_r_states; i++){
    value_r[i]=0;
    lock_time_r[i]=millis();
  }  
}


void update_r()
{
  int i;
  for (i=0; i<num_r_states; i++){
    //reset trigger state to 0
    if (lock_time_r[i]+1000 < millis()) write_state_silent(r_trigger[i],0);

    //sensors => value 
    bool down = (value_b[r_down[i]]==1);
    bool up   = (value_b[r_up[i]]==1);
    if (down & up)   value_r[i]=2;
    if (down & !up)  value_r[i]=-1;
    if (!down & up)  value_r[i]=1;
    if (!down & !up) value_r[i]=0;

    //compare to last value / post changes
    if (value_r[i] != last_value_r[i]){
      send_message("w", r_address[i], value_r[i]);
      if (value_r[i] == 2){
        Serial.print("update_r: error, both contacts are closed. i: ");
        Serial.println(i);
      }
    }
    last_value_r[i]=value_r[i];
    
  }  
}

    
void write_r(String address, int value){
  int i;
  for (i=0; i<num_r_states; i++){
    if (r_address[i]==address){
      if (lock_time_r[i]+lock_delay_r < millis()){
        lock_time_r[i]=millis();
        write_state_silent(r_trigger[i],1);
      }
    }  
  }
}
