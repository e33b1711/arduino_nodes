void setup_u()
{
  Serial.println("setup_u");
  int i;
  for (i=0; i<num_u_states; i++){
    send_message("w", u_address[i], value_u[i]);
    digitalWrite(u_pin[i],HIGH);
    pinMode(u_pin[i],OUTPUT);
  }
  for (i=0; i<num_u_states; i++){
    delay(1000);
    digitalWrite(u_pin[i],LOW);
  }
  previousMillis=millis();
}


void update_u()
{
  unsigned long currentMillis = millis()+2*u_interval;
  int i;  
  if (currentMillis - previousMillis >= u_interval) {
    previousMillis = currentMillis;
    u_phase=u_phase+1;
    if (u_phase>14){u_phase=0;}
    for (i=0; i<num_u_states; i++){
      Serial.println(value_u[i]>=((u_phase + 2*i) % 16));
      if (value_u[i]>((u_phase + 2*i) % 15)){
        digitalWrite(u_pin[i],LOW);
      }else{
        digitalWrite(u_pin[i],HIGH);
      }
    }
  } 
}


void write_u(String address, int value){
  int i;
  for (i=0; i<num_u_states; i++){
    if (u_address[i]==address){
      if (value<0){
        value_u[i]=0;}
        else if (value>15){
           value_u[i]=7;}
           else{
      value_u[i]=value;}
      Serial.println(value_u[i]);
      Serial.println("update u");
    }
  }
}
