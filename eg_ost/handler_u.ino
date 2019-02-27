////constants and variables for u states (temperatur steller)
////                              0                1                  2                 3              4                5
////                              hobby1           hobby2             hobby3            lager          waschküche       gang
//const int num_u_states=6;
//const String u_address[]={      'U_UG_H1',       'U_UG_H2',         'U_UG_H3',        'U_UG_LA',     'U_UG_WK',       'U_UG_GA'};        //addresse
//const int u_pin[]={             32,              33,                34,               35,             36,             37};               //pwm-pin  
//const long u_interval= 5000;          //pwm periode /16 in milisekunden
//unsigned long previousMillis=0;
//int u_phase=0;
//int value_u={                   5,               5,                 5,                 3,            14,                5};               //stell wert 0-15 (0=aus bis 15=voll)



void setup_u()
{
  int i;
  Serial.println("init_u");
  
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
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    u_phase=u_phase+1;
    if (u_phase>14){u_phase=0;}
    for (i=0; i<num_u_states; i++){
      Serial.println(value_u[i]>=((u_phase + 2*i) % 16));
      if (value_u[i]>((u_phase + 2*i) % 15)){
        digitalWrite(u_pin[i],LOW);
      }
        else{
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



