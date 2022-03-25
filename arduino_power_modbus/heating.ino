void setup_p()
{
  Serial.println("setup_l");
  int i;
  for (i=0; i<num_p_states; i++){
    pinMode(p_pin[i],OUTPUT);
    analogWrite(p_pin[i], 0);
    value_p[i]=0;
    send_message("w", p_address[i], value_p[i]);
    set_time_p[i]=millis();
  }
}


void update_p()
{
  int i;
  for (i=0; i<num_p_states; i++){
  if (value_p[i] > max_p[i]) value_p[i] = max_p[i];
    analogWrite(p_pin[i], value_p[i]);
  }
  
}


void write_p(String address, int value){
  int i;
  for (i=0; i<num_p_states; i++){
    if (p_address[i]==address){
      set_time_p[i]=millis();
      value_p[i] = value;
      if (value_p[i] > max_p[i]) value_p[i] = max_p[i];
      send_message("w", p_address[i], value_p[i]); 
    }
  }
}
