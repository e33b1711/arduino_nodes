////constants and variables for l states (einfaches licht / verbraucher)
////                              0                1                  2                 3
////                              Wohnzimmer       Esszimmer
//const int num_l_states=4;
//const String l_address[]={      'LI_EG_WZ',      'LI_EG_WZ',        'LI_EG_EZ',       'LI_EG_GA',};       //addresse, zum gleichschalten selbe addresse vergeben
//const int l_pin[]={             12,              13,                 14,              15};                //digitaler pin
//int value_l[]={                 0,               0                   0,               0};
//int aux_value_l[]


void setup_l()
{
  int i;
  for (i=0; i<num_l_states; i++){
  digitalWrite(l_pin[i],HIGH);
  pinMode(l_pin[i],OUTPUT);
  value_l[i]=0;
  send_message("w", l_address[i], value_l[i]);
  }
}


void update_l()
{
  int i;
  for (i=0; i<num_l_states; i++){
  switch(value_l[i]){
  case 1:
    digitalWrite(l_pin[i],LOW);
    break;
  default:
    digitalWrite(l_pin[i],HIGH);
    value_l[i]=0;
    break;
    }
  }
}


void write_l(String address, int value){
  int i;
  
  
      
  for (i=0; i<num_l_states; i++){
    
   
    
    if (l_address[i]==address){
      
      //db
      //Serial.println("write_l" + address);
      // Serial.println(l_pin[i]);
      set_time_l[i]=millis();
     
      switch (value) {
      case 0:
      value_l[i]=0;
      break;
      case 1:
      value_l[i]=1;
      break;
      default:
      if (value_l[i]==1){
        value_l[i]=0;
      }
      else
        {
       value_l[i]=1;
        }
      send_message("w", l_address[i], value_l[i]);
      break; 
    }
     
      
  }
}

}





