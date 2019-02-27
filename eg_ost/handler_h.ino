////constants and variables for h states (feuchtigkeit über zustand t über dht22 an digitalem pin)
////                              0                1                 2                 3
////                              wie t states
//const int num_h_states=4;
//const String address={          'HI_EG_WZ',      'HI_EG_EZ',       'HI_EG_KU',       'HI_EG_GA'};       //addresse
//int value_h[]={                 0,               0,                0,                0};
//


void setup_h()
{
  
}


void update_h()
{
  
}

void write_h(String address, int value){
  int i;
  for (i=0; i<num_h_states; i++){
    if (h_address[i]==address){
      value_h[i]=value;
    }
  }
}

