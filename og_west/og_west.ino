//this nodes name
const char* unit_name 	= "og_west";
const char* password   	= "pass";


//for tcp communication
//watch out for the pins needed for the ethernet schield (always 10, 11 12 13 on uno, 50 51 52 53 on mega!)
#include <Ethernet.h>
const byte mac[]                = {0xDE, 0xAA, 0x7E, 0xE1, 0x1E, 0x16 };
const IPAddress                 ip(192,168,178,217);
const IPAddress                 server(192,168,178,222);
const IPAddress                 gateway(192,168,178,1);
const IPAddress                 subnet(255,255,255,0);
const int port                  = 8888;
const int ethernet_sc_pin       = 53;
const int ethernet_reset_pin    = 12;


//constants and variables for b states (einer der 2 R-codierte Schalter an einem analogen Eingang)
int counter_a=0;
const int num_b_states  = 16;
const int b_pin[]       = {4,  5,  6,  7,  8,  9,  10, 11, 25, 24, 23, 22, 17, 16, 3,  2, 33, 32, 31, 30, 29, 28, 27, 26};                           
int value_b[]           = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};    
int prev_value_b[]      = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};    


//constants and variables for c states (flanke eines b states)
/*
0  bini oben
1  gang bini
2  bini hoch
3  bini runter
4  leo a hoch
5  bini a runter
6  leo a runter
7  bini a hoch
8  leo hoch
9  leo unten
10 gang runter
11 leo runter
12 bini unten
13 gang hoch 
14 leo oben
15 gang leo
*/
const int num_c_states  = 16;
const int which_b[]     = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};       //a state auf der selben unit                         
int value_c[]           = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};           //positive flanke
int aux_value_c[]       = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};          //negative flanke
long time_c_neg[]       = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};           //zeit der letzen fallenden flanke
long time_c_pos[]       = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};        //zeit der letzen steigenden flanke
       
	   
//constants and variables for t states (temperatur über dht22 an digitalem pin)
const int num_t_states      = 3;
const long period_t         = 1800000;                                                                                  //update periode in ms
const String t_address[]    = {"TI_OG_KN", "TI_OG_KS", "TI_OG_GA"};                                    
const int t_pin[]           = {28, 27, 26};
int value_t[]               = {0,  0,  0};                                            //temperatur
int aux_value_t[]           = {0,  0,  0};                                           //feuchtigkeit
long time_t                 = 0;                                                                                          //update timer
int i_t                     = 0;                                                                                            //cycle_counter


//constants and variables for h states (feuchtigkeit über zustand t über dht22 an digitalem pin)
const int num_h_states      = 3;
const String h_address[]    = {"HI_OG_KS", "HI_OG_KN", "HI_OG_GA"};          //addresse
int value_h[]               = {0,  0,  0};    


//constants and variables for l states (einfaches licht / verbraucher)
const int num_l_states      = 28;
const String l_address[]    = {"LI_34", "RO_OG_KS_DO", "LI_OG_KS", "RO_OG_KS_ON", "LI_46", "LI_47", "LI_48", "LI_49", "LI_OG_KS_L1", "LI_OG_KN_L1", "LI_OG_KN", "RO_OG_KN_ON", "LI_GA_L1", "RO_OG_KN_DO", "LI_OG_GA_L1", "LI_OG_GA", "DF_OG_KN_DO", "DF_OG_GA_DO","DF_OG_KN_UP", "DF_OG_KS_UP", "DF_OG_GA_UP", "DF_OG_KS_DO", "LI_60", "LI_61", "VD_OG_KN_DO", "VD_OG_KS_UP", "VD_OG_KN_UP", "VD_OG_KS_DO"};       //addresse, zum gleichschalten selbe addresse vergeben
const int l_pin[]           = {34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 54, 55, 56, 57, 58, 59, 60, 61, 30, 31, 32, 33};                //digitaler pin
const bool l_inv[]          = {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};                //digitaler pin
int value_l[]               = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};    
long set_time_l[]           = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};    


////constants and variables for r states (rollo)
// über an/aus und richtungsrelais gesteuert
const int num_r_states=2;
const String r_address[]	= {"RO_OG_KN", "RO_OG_KS"};       //addresse
const String r_on_off[]		= {"RO_OG_KN_ON", "RO_OG_KS_ON"};         //l state
const String r_up_down[] 	= {"RO_OG_KN_DO", "RO_OG_KS_DO"};         //l state
const int up_time_r[] 		= {31, 31};       // zeit zum öffnen in s
const int down_time_r[] 	= {31, 31};       // zeit zum schließen in s
int value_r[] 				= {0,  0};          // -1 zu und verriegelt, 0 entriegelt, 1 auf und verriegelt
long stop_time_r[] 			= {0,  0};          // zeit zu stoppen

////constants and variables for s states (dachfenster)
// üer eingänge auf und ab gesteuert
const int num_s_states		= 5;
const String s_address[]	= {"DF_OG_KN", "DF_OG_KS", "DF_OG_GA", "VD_OG_KS", "VD_OG_KN"};       //addresse
const String s_up[]			= {"DF_OG_KN_UP", "DF_OG_KS_UP", "DF_OG_GA_UP", "VD_OG_KS_UP", "VD_OG_KN_UP"};         //l state
const String s_down[]		= {"DF_OG_KN_DO", "DF_OG_KS_DO", "DF_OG_GA_DO", "VD_OG_KS_DO", "VD_OG_KN_DO"};         //l state
const int up_time_s[]		= {500, 500, 500, 500, 500};       // zeit zum öffnen in ms
const int down_time_s[] 	= {500, 500, 500, 500, 500};       // zeit zum schließen in ms
int value_s[] 				= {-1,  -1,  -1,  0,   0};          // -1 zu und verriegelt, 0 entriegelt, 1 auf und verriegelt
long stop_time_s[] 			= {0,   0,   0,   0,   0};          // zeit zu stoppen


//constants and variables for u states (temperatur steller)
const int num_u_states          = 0;
const String u_address[]        = {"U_34", "U_35", "U_36", "U_37", "U_38", "U_39", "U_40", "U_41"};        //addresse
const int u_pin[]               = {34, 35, 36, 37, 38, 39, 40, 41};               //pwm-pin  
const unsigned long u_interval  = 240000;          //pwm periode /16 in milisekunden
unsigned long previousMillis    = 0;
int u_phase                     = 0;
int value_u[]                   = {3, 3, 3, 3, 3, 3, 3, 3};               //stell wert 0-15 (0=aus bis 15=voll)


void user_logic()
{
    int i;
   
  //taster verküpfungen
  // 15  Gang, KZ nord, einzel
  // 10  DF
  // 13  "   
  i=15;
   if (value_c[i]==-1){
     if (time_c_pos[i]+700>time_c_neg[i]){
      toggle_state("LI_OG_GA");
     }
     else{
     
      toggle_state("LI_OG_GA_L1");
     }
  }
  i=10;
  if (value_c[i]==1){
     write_state("DF_OG_GA",1);
  }
  i=13;
  if (value_c[i]==1){
     write_state("DF_OG_GA",-1);
  }
  //1  Gang, KZ süd
  i=1;
  if (value_c[i]==-1){
     if (time_c_pos[i]+700>time_c_neg[i]){
      toggle_state("LI_OG_GA");
     }
     else{
     
      toggle_state("LI_OG_GA_L1");
     }
  }
  //14  KZ nord unten
  //9  "   oben
  //8  DF KZ nord
  //11
  i=14;
  if (value_c[i]==1){
     toggle_state("LI_OG_KN");
  }
  i=9;
  if (value_c[i]==1){
     toggle_state("LI_OG_KN_L1");
  }
  i=8;
   if (value_c[i]==-1){
    //verriegeln auf auf
     if (time_c_pos[i]+700>time_c_neg[i]){
      write_state("DF_OG_KN",1);
     }
     else{
       write_state("VD_OG_KN",1);
     }
  }
  i=11;
   if (value_c[i]==-1){
    //verriegeln auf auf
     if (time_c_pos[i]+700>time_c_neg[i]){
      write_state("DF_OG_KN",-1);
     }
     else{
       write_state("VD_OG_KN",-1);
     }
  }
  
  
  
  //4  Rollo, KZ nord
  //6 
  i=4;
  if (value_c[i]==1){
    //entriegeln
     write_state("RO_OG_KN",0);
     //aktivieren
     write_state("RO_OG_KN_ON",1);
     write_state("RO_OG_KN_DO",0);
  }
  //negative flanke, wenn weniger als 1 sekunde nach positiver dann wird verriegelt
  if (value_c[i]==-1){
    //verriegeln auf auf
     if (time_c_pos[i]+1000>time_c_neg[i]){
       write_state("RO_OG_KN",1);
     }
     else{
       write_state("RO_OG_KN_ON",0);
       write_state("RO_OG_KN_DO",0);
     }
  }
  i=6;
  if (value_c[i]==1){
    //entriegeln
     write_state("RO_OG_KN",0);
     //aktivieren
     write_state("RO_OG_KN_ON",1);
     write_state("RO_OG_KN_DO",1);
  }
  if (value_c[i]==-1){
    //verriegeln
     if (time_c_pos[i]+1000>time_c_neg[i]){
       write_state("RO_OG_KN",-1);
     }
     else{
       write_state("RO_OG_KN_ON",0);
       write_state("RO_OG_KN_DO",0);
     }
  }  
  
  
  
  
  
  //0   KZ süd oben
  //12  "    unten
  //2  DF KZ süd
  //3  "
  i=0;
  if (value_c[i]==1){
     toggle_state("LI_OG_KS");
  }
  i=12;
  if (value_c[i]==1){
     toggle_state("LI_OG_KS_L1");
  }
  i=2;
   if (value_c[i]==-1){
    //verriegeln auf auf
     if (time_c_pos[i]+700>time_c_neg[i]){
      write_state("DF_OG_KS",1);
     }
     else{
       write_state("VD_OG_KS",1);
     }
  }
  i=3;
   if (value_c[i]==-1){
    //verriegeln auf auf
     if (time_c_pos[i]+700>time_c_neg[i]){
      write_state("DF_OG_KS",-1);
     }
     else{
       write_state("VD_OG_KS",-1);
     }
  }
  
  
  
  
  
  //7  Rollo, KZ süd
  //5  
  i=7;
  if (value_c[i]==1){
    //entriegeln
     write_state("RO_OG_KS",0);
     //aktivieren
     write_state("RO_OG_KS_ON",1);
     write_state("RO_OG_KS_DO",0);
  }
  //negative flanke, wenn weniger als 1 sekunde nach positiver dann wird verriegelt
  if (value_c[i]==-1){
    //verriegeln auf auf
     if (time_c_pos[i]+1000>time_c_neg[i]){
       write_state("RO_OG_KS",1);
     }
     else{
       write_state("RO_OG_KS_ON",0);
       write_state("RO_OG_KS_DO",0);
     }
  }
  i=5;
  if (value_c[i]==1){
    //entriegeln
     write_state("RO_OG_KS",0);
     //aktivieren
     write_state("RO_OG_KS_ON",1);
     write_state("RO_OG_KS_DO",1);
  }
  if (value_c[i]==-1){
    //verriegeln
     if (time_c_pos[i]+1000>time_c_neg[i]){
       write_state("RO_OG_KS",-1);
     }
     else{
       write_state("RO_OG_KS_ON",0);
       write_state("RO_OG_KS_DO",0);
     }
  }  
  
  
  
  
}
