////constants and variables for t states (temperatur über dht22 an digitalem pin)
////                              0                1                 2                 3
////                              strang wz grün   " gelb            " schwarz         " blau
////                              wohnzimmer       esszimmer         küche             gang eg
//const int num_t_states=4;
//const String t_address[]={      'TI_EG_WZ',      'TI_EG_EZ',       'TI_EG_KU',       'TI_EG_GA'};       //addresse
//const int t_pin[]={             4                5                 6                 7
//int value_t[]={                 0,               0,                0,                0};                //temperatur
//int value_h[]={             0,               0,                0,                0};                //feuchtigkeit
//int time_t



void setup_t()
{
  int i;
  for (i=0; i<num_t_states; i++){
    
      
      int chk = DHT.read22(t_pin[i]);
      switch (chk)
      {
      case DHTLIB_OK:  
        break;
      default:
        send_message("dht22_error", t_address[i], 0);
        Serial.println("setup_t: error reading from dth22.");
        break;
      }
      value_t[i]=DHT.temperature*10;
      value_h[i]=DHT.humidity;
      send_message("w", t_address[i], value_t[i]);
      send_message("w", h_address[i], value_h[i]);
  }
}



void update_t()
{
  if (time_t+(period_t*1000)<millis())
  {
    time_t=millis();
    
//    //debbuging
//    Serial.println("updating t");
//    //debbuging
    i_t++;
    if (i_t==num_t_states){i_t=0;}
    int chk;
    chk = DHT.read22(t_pin[i_t]);
    value_t[i_t]=DHT.temperature*10;
    value_h[i_t]=DHT.humidity;
    send_message("w", t_address[i_t], value_t[i_t]);
    send_message("w", h_address[i_t], value_h[i_t]);
    
      
}
}

void write_t(String address, int value){
  int i;
  for (i=0; i<num_t_states; i++){
    if (t_address[i]==address){
      value_t[i]=value;
    }
  }
}








