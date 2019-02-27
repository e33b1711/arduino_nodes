////constants and variables for r states (rollo)
////                              0                1                  2                 3
////                              Esszimmer
//const int num_r_states=4;
//const String r_address[]={      'RO_EG_SU'};       //addresse
//const String r_on_off[]={             R11_EG_SU};         //l state
//const String r_up_down[]={             R12_EG_SU};         //l state
//const int up_time[]={            10};       // zeit zum öffnen in s
//const int down_time[]={          10};       // zeit zum schließen in s
//int state_r[]={                  0};          // -1 zu und verriegelt, 0 entriegelt, 1 auf und verriegelt
//int stop_time_r[]={              0};          // zeit zu stoppen


void setup_s()
{
  int i;
  for (i=0; i<num_s_states; i++){
  send_message("w", s_address[i], value_r[i]);
  stop_time_s[i]=millis()+down_time_s[i];;
  }
}


void update_s()
{
  int i;
  for (i=0; i<num_s_states; i++){
    if(stop_time_s[i]>millis()){
    if(value_s[i]==1)
    {
      write_l(s_up[i], 1);
      write_l(s_down[i], 0);
    }
    if(value_s[i]==-1)
    {
      write_l(s_up[i], 0);
      write_l(s_down[i], 1);
    }
    }
    else{
    if(!(value_s[i]==0))
    {
      write_l(s_up[i], 0);
      write_l(s_down[i], 0);
    } 
   }
}
}

    
void write_s(String address, int value){
  int i;
  for (i=0; i<num_s_states; i++){
    if (s_address[i]==address){
      
//Serial.println("address match: r");
      
      switch (value) {
        
        case 1:
        if (!(value_s[i]==1)){
          stop_time_s[i]=millis()+up_time_s[i];
          value_s[i]=1;
        }
        break;
        
        case -1:
        if (!(value_s[i]==-1)){
          stop_time_s[i]=millis()+down_time_s[i];
          value_s[i]=-1;
        }
        break;
        
        default:
        value_s[i]=0;
        break;
      }
    }
  }
}






