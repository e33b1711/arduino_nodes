//this nodes name
const String unit_name = "eg_ost";


//for tcp communication
//watch out for the pins needed for the ethernet schield (always 10, 11 12 13 on uno, 50 51 52 53 on mega!)
#include <Ethernet.h>
const int ethernet_sc_pin = 10;
const byte mac[] = {0xDE, 0xAA, 0x7E, 0xEF, 0xFE, 0x04 };
const IPAddress ip(192,168,178,211);
const IPAddress server(192,168,178,222);
const int port = 8888;




//constants and variables for b states (einer der 2 R-codierte Schalter an einem analogen Eingang)
int counter_a=0;
const int num_b_states=16;
const int b_pin[]={           
  2, 3, 4, 5, 6, 7, 8, 9, 9, 11, 12, 13, 14, 15, 16, 17};       //a state auf der selben unit                         
int value_b[]={               
  0, 0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,   0,  0,  0,  0};       //an/aus
int prev_value_b[]={          
  0, 0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,   0,  0,  0,  0};       //an/aus (alter Wert zur Flankenerkennung)


//constants and variables for c states (flanke eines b states)
//0 klingel
//1 treppe hoch
//2 treppe runter
//3 kachelofen
//4 wc
//5 ht oben
//6  treppe mitte / ht unten
//7 
//8 treppe mitte
//9 wz ez
//10
//11 wz gang
//12 
//13 gang wz
//14 gardarobe
//15 
const int num_c_states=16;
const int which_b[]={           
0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15};       //a state auf der selben unit                         
int value_c[]={                 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};       //positive flanke
int aux_value_c[]={             
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};       //negative flanke
long time_c_neg[]={ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};       //zeit der letzen fallenden flanke
long time_c_pos[]={ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};       //zeit der letzen steigenden flanke


//constants and variables for t states (temperatur über dht22 an digitalem pin)
//                              hobby süd, hobby nord, heizkeller, lager, waschküche, gang
const int num_t_states=0;
const int period_t=309;                                                                                  //update periode in s
const String t_address[]={      
"TI_EG_WC", "TI_48", "TI_49", "TI_50", "TI_51", "TI_52"};       
//addresse
const int t_pin[]={             
49,  48,  49,  50,  51, 52};
int value_t[]={                 
0,   0,   0,   0,   0,   0};                                            //temperatur
int aux_value_t[]={             
0,   0,   0,   0,   0,   0};                                            //feuchtigkeit
long time_t=0;                                                                                          //update 
int i_t=0;                                                                                              //cycle_counter


//constants and variables for h states (feuchtigkeit über zustand t über dht22 an digitalem pin)
const int num_h_states=0;
const String h_address[]={      
"HI_EG_WC", "HI_48", "HI_49", "HI_50", "HI_51", "HI_52"};       //addresse
int value_h[]={                 
0, 0, 0, 0, 0, 0};


//constants and variables for l states (einfaches licht / verbraucher)
const int num_l_states=16;
const String l_address[]={      
"LI_EG_GR", "LI_EG_GA", "LI_GA_L1", "LI_EG_WZ_L1", "LI_EG_WZ_L2", "LI_EG_WZ", "LI_EG_WC", "LI_EG_VH", "ZE_EG_VH", "L31", "L40", "L41", "L42", "L43", "L44", "L45", "L46", "L47"};       //addresse, zum gleichschalten selbe addresse vergeben
const int l_pin[]={             
22,23,24,25,26,27,28,29,30,31,40,41,42,43,44,45};                //digitaler pin
int value_l[]={                 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
long set_time_l[]={
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


////constants and variables for r states (rollo)
// über an/aus und richtungsrelais gesteuert
const int num_r_states=0;
const String r_address[]={      
"RO_EG_SU", "RO_EG_WE"};       //addresse
const String r_on_off[]={       
"RO_EG_SU_ON", "RO_EG_WE_ON"};         //l state
const String r_up_down[]={      
"RO_EG_SU_DO", "RO_EG_WE_DO"};         //l state
const int up_time_r[]={            
25, 25};       // zeit zum öffnen in s
const int down_time_r[]={          
22, 22};       // zeit zum schließen in s
int value_r[]={                  
0, 0};          // -1 zu und verriegelt, 0 entriegelt, 1 auf und verriegelt
long stop_time_r[]={              
0, 0};          // zeit zu stoppen


////constants and variables for s states (dachfenster)
// üer eingänge auf und ab gesteuert
const int num_s_states=0;
const String s_address[]={      
"DF_OG_KN"};       //addresse
const String s_up[]={           
"DF_OG_KN_UP"};         //l state
const String s_down[]={         
"DF_OG_KN_UP_DO"};         //l state
const int up_time_s[]={         
10};       // zeit zum öffnen in ms
const int down_time_s[]={       
200};       // zeit zum schließen in ms
int value_s[]={                  
-1};          // -1 zu und verriegelt, 0 entriegelt, 1 auf und verriegelt
long stop_time_s[]={              
0};          // zeit zu stoppen


//constants and variables for u states (temperatur steller)
//                              0                1                  2                 3              4                5            6
//                              KU               EZ1                EZ2(2 kr, ost)    Gang           WZ              Gardarobe     WC         
const int num_u_states=7;
const String u_address[]={      
"U_EG_KU",       "U_EG_E1",         "U_EG_E2",        "U_EG_GA",     "U_EG_WZ",       "U_EG_GR",    "U_EG_WC"};        //addresse
const int u_pin[]={             
32,               33,               34,               35,             36,              37,            38};               //pwm-pin  
const unsigned long u_interval= 230000;          //pwm periode /16 in milisekunden
unsigned long previousMillis=0;
int u_phase=8;
int value_u[]={                   
6,              6,                  6,                6,             6,              6,            6};               //stell wert 0-15 (0=aus bis 15=voll)


void user_logic()
{
  int i;

  // 14  gararobe
  i=14;
  if (value_c[i]==-1){
    //verriegeln auf auf
    if (time_c_pos[i]+700>time_c_neg[i]){
      toggle_state("LI_EG_GR");
    }
    else{
      write_state("LI_EG_AO",3);
    }
  }

  //6  haustür, innen, unten // treppe unten
  //5  " oben
  i=6;
  if (value_c[i]==1){
    toggle_state("LI_EG_GA");
    //write_state("LI_GA_L1",3);
  }
  i=5;
  if (value_c[i]==1){
    write_state("ZE_EG_VH",1);
  }

  // 13  gang bei wz
  i=13;
  if (value_c[i]==1){
    toggle_state("LI_EG_GA");
    //write_state("LI_GA_L1",3);
  }

  // 11  wz bei gang
  // 9  wz bei ez
  i=11;
  if (value_c[i]==-1){
    if (time_c_pos[i]+700>time_c_neg[i]){
      int value= address_to_value("LI_EG_WZ");
      if (value==1){ 
        value=0;
      } 
      else {
        value=1;
      }
      write_state("LI_EG_WZ",value);
      write_state("LI_EG_WZ_L1",value);
      write_state("LI_EG_WZ_L2",value);
    }
    else{
      write_state("LI_EG_WZ",0);
      write_state("LI_EG_WZ_L1",1);
      write_state("LI_EG_WZ_L2",0);
    }
  }
  i=9;
  if (value_c[i]==-1){
    if (time_c_pos[i]+700>time_c_neg[i]){
      int value= address_to_value("LI_EG_WZ");
      if (value==1){ 
        value=0;
      } 
      else {
        value=1;
      }
      write_state("LI_EG_WZ",value);
      write_state("LI_EG_WZ_L1",value);
      write_state("LI_EG_WZ_L2",value);
    }
    else{
      write_state("LI_EG_WZ",0);
      write_state("LI_EG_WZ_L1",1);
      write_state("LI_EG_WZ_L2",0);
    }
  }

  //3  kachelofen
  i=3;
  if (value_c[i]==-1){
    //verriegeln auf auf
    if (time_c_pos[i]+700>time_c_neg[i]){
      write_state("LI_EG_EZ_KU",3);
    }
    else{
      write_state("LI_EG_EZ_L3",3);
    }
  }

  //4  wc
  i=4;
  if (value_c[i]==1){
    toggle_state("LI_EG_WC");
  }

  //8  treppe oben
  //1  treppe dachfenster
  //2  treppe dachfenster
  i=8;
  if (value_c[i]==-1){
    if (time_c_pos[i]+700>time_c_neg[i]){
      write_state("LI_OG_GA",3);
    }
    else{

      write_state("LI_OG_GA_L1",3);
    }
  }
  i=1;
  if (value_c[i]==1){
    write_state("DF_OG_GA",-1);
  }
  i=2;
  if (value_c[i]==1){
    write_state("DF_OG_GA",1);
  }

  //0 klingel
  i=0;
  if (value_c[i]==1){
    write_state("BELL",1);
  }
  if (value_c[i]==-1){
    write_state("BELL",0);
  }

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
