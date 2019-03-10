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
     if (lock_time_r[i]+1000 < millis()) write_state(r_trigger[i],1);

    //sensors => value 
    bool down = (value_b[r_down[i]]==1);
    bool up   = (value_b[r_up[i]]==1);
    if (down & up){
      value_r[i]=2;
      Serial.println("update_r: error both sensors are closed.");
    }
    if (down & !up)  value_r[i]=-1;
    if (!down & up)  value_r[i]=1;
    if (!down & !up) value_r[i]=0;
    
  }  
}

    
void write_r(String address, int value){
  int i;
  for (i=0; i<num_r_states; i++){
    if (r_address[i]==address){
      if (lock_time_r[i]+lock_delay_r < millis()){
        lock_time_r[i]=millis();
        write_state(r_trigger[i],1);
      }
    }  
  }
}