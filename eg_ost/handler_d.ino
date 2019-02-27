////constants and variables for d states (pwm-dimmer ausgang)
////                              0                1
////                              Wohnzimmer       Esszimmer
//const int num_d_states=2;
//const String d_address[]={      'DI_EG_WZ',      'DI_EG_EZ'};       //addresse, zum gleichschalten selbe addresse vergeben
//const int d_pin[]={             10,              11}                //pwm-pin  
//int value_d[]={                 0,               0};       


void setup_d()
{
  int i;
  for (i=0; i<num_d_states; i++){
  pinMode(d_pin[i], OUTPUT);
  value_d[i]=0;
  send_message("w", d_address[i], value_d[i]);
  }
}


void update_d()
{
  int i;
  for (i=0; i<num_d_states; i++){
    analogWrite(d_pin[i], value_d[i]);
  }
}

void write_d(String address, int value){
  int i;
  for (i=0; i<num_d_states; i++){
    if (d_address[i]==address){
      value_d[i]=value;
    }
  }
}

