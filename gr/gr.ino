/**
to do:
pin out
wifi
do state
user logic
timer in user logic
**/

//this nodes name
const String unit_name = "gr";


//for tcp communication
//watch out for the pins needed for the ethernet schield (always 10, 11 12 13 on uno, 50 51 52 53 on mega!)
#include <Ethernet.h>
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x01 };
IPAddress ip(192,168,178,200);
IPAddress server(192,168,178,222);
int port = 8888;


//constants and variables for b states (einer der 2 R-codierte Schalter an einem analogen Eingang)
int counter_a=0;
const int num_b_states=3;
const int b_pin[]={           
  2, 3, 4};       //a state auf der selben unit                         
int value_b[]={               
  0, 0, 0};       //an/aus
int prev_value_b[]={          
  0, 0, 0};       //an/aus (alter Wert zur Flankenerkennung)


//constants and variables for c states (flanke eines b states)
//0 taster
//1 tor zu
//2 tor auf
const int num_c_states=15;
const int which_b[]={           
  0, 1, 2};       //a state auf der selben unit                         
int value_c[]={                 
  0, 0, 0};       //positive flanke
int aux_value_c[]={             
  0, 0, 0};       //negative flanke
long time_c_neg[]={             
  0, 0, 0};       //zeit der letzen fallenden flanke
long time_c_pos[]={             
  0, 0, 0};       //zeit der letzen steigenden flanke       


//constants and variables for t states (temperatur über dht22 an digitalem pin)
//                              hobby süd, hobby nord, heizkeller, lager, waschküche, gang
const int num_t_states=1;
const int period_t=309;                                                                                  //update periode in s
const String t_address[]={      
  "TI_GR", "TI_G2"};       
//addresse
const int t_pin[]={             
  49,  48};
int value_t[]={                 
  0,   0};                                            //temperatur
int aux_value_t[]={             
  0,   0};                                            //feuchtigkeit
long time_t=0;                                                                                          //update 
int i_t=0;                                                                                              //cycle_counter


//constants and variables for h states (feuchtigkeit über zustand t über dht22 an digitalem pin)
const int num_h_states=1;
const String h_address[]={      
  "HI_EG_WC", "HI_48", "HI_49", "HI_50", "HI_51", "HI_52"};       //addresse
int value_h[]={                 
  0, 0, 0, 0, 0, 0};


//constants and variables for l states (einfaches licht / verbraucher)
const int num_l_states=16;
const String l_address[]={      
  "LI_GR", "LI_GR_L2", "LI_GR_L3", "ZE_GR", "ZE_G2"};       //addresse, zum gleichschalten selbe addresse vergeben
const int l_pin[]={             
  22,23,24,25,26};                //digitaler pin
int value_l[]={                 
    0, 0, 0, 0, 0};
long set_time_l[]={
    0, 0, 0, 0, 0};


////constants and variables for do states (garagentor)
// über an/aus und richtungsrelais gesteuert
const int num_do_states=1;
const String do_address[]={      
  "DO_GR"};       //addresse
const int do_pin_up[]={
    99};
const int do_pin_down[]={
    99};
const int do_pin_aktor[]={
    99};
int value_do[]={
    99};
long lock_time[]={
    0};


void user_logic()
{
  int i;
  // c states über debug schnittstelle
  for (i=0; i<num_c_states; i++){
    if (value_c[i]==1){
      Serial.print("pos Flanke: ");
      Serial.println(i);
    }
    if (value_c[i]==-1){
      Serial.print("neg Flanke: ");
      Serial.println(i);
    }
  }

  //anpassen für einfahrtsbeleuchtung (3min) und taster (10min)
  //timer: Vordach Beleuchtung
  static boolean timer_on=false;
  //start
  if ((address_to_value("ZE_EG_VH")==1) & (timer_on==false)){
    timer_on=true;
    write_state("LI_EG_VH",1);
    Serial.print("user_logic: timer an");
  }
  //timer: running
  if ((address_to_value("ZE_EG_VH")==1) & (timer_on==true)){
    if ((set_time_l[8]+180000)<millis()){
      write_state("LI_EG_VH",0);
      write_state("ZE_EG_VH",0);
      timer_on=false;
      Serial.print("user_logic: timer abgelaufen");
    }
  }
  //timer: external off
   if ((address_to_value("ZE_EG_VH")==0) & (timer_on==true)){
     timer_on=false;
     write_state("LI_EG_VH",0);
     Serial.print("user_logic: timer extern abgebrochen. LI_EG_VH ausgeschaltet!");
  }

}








