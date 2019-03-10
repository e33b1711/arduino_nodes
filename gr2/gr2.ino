//this nodes name
const String unit_name = "eg_west";

/**
//for tcp communication
//watch out for the pins needed for the ethernet schield (always 10, 11 12 13 on uno, 50 51 52 53 on mega!)
#include <Ethernet.h>
const int ethernet_sc_pin = 53;
const byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x01 };
const IPAddress ip(192,168,178,200);
const IPAddress server(192,168,178,222);
const int port = 8888;
**/



// wifi settings, on mega wifi on hw serial 1, on uno on sw serial on pins 6 7
#include "WiFiEsp.h"
char ssid[] = "DRGREENTUMB";            // your network SSID (name)
char pass[] = "JUMPAROUND99";        // your network password
//char server[] = "ak-ThinkPad-Edge-E540";
char server[] = "openhabianpi2.fritz.box";
WiFiEspClient client;




//constants and variables for b states (einer der 2 R-codierte Schalter an einem analogen Eingang)
int counter_a=0;
const int num_b_states  = 8;
const int b_pin[]       = {26, 28, 30, 32, 34, 36, 38, 40};       //a state auf der selben unit                         
int value_b[]           = {0, 0, 0, 0, 0, 0, 0, 0};       //an/aus
int prev_value_b[]      = {0, 0, 0, 0, 0, 0, 0, 0};       //an/aus (alter Wert zur Flankenerkennung)


//constants and variables for c states (flanke eines b states)
//0     taster innen
//1     garagentor down
//2     garagentor up
//3     
//4 
//5
//6  
//7 
//8 
//9 
//10
//11 
//12 
//13 
//14 
//15 
const int num_c_states=8;
const int which_b[]={0, 1, 2, 3, 4, 5, 6, 7};       //a state auf der selben unit                         
int value_c[]={0, 0, 0, 0, 0, 0, 0, 0};   //positive flanke
int aux_value_c[]={0, 0, 0, 0, 0, 0, 0, 0};        //negative flanke
long time_c_neg[]={0, 0, 0, 0, 0, 0, 0, 0};        //zeit der letzen fallenden flanke
long time_c_pos[]={0, 0, 0, 0, 0, 0, 0, 0};        //zeit der letzen steigenden flanke


//constants and variables for t states (temperatur über dht22 an digitalem pin)
const int num_t_states=2;
const int period_t=10000;                                                                                  //update periode in ms
const String t_address[]={"TI_GR", "TI_GR_A"};                                                                                                           //addresse
const int t_pin[]={ 22, 24};
int value_t[]={ 0,   0};                                            //temperatur
int aux_value_t[]={  0,   0};                                            //feuchtigkeit
long time_t=0;                                                                                          //update timer
int i_t=0;                                                                                              //cycle_counter
  

//constants and variables for h states (feuchtigkeit über zustand t über dht22 an digitalem pin)
const int num_h_states=2;
const String h_address[]={"HI_GR", "HI_GR_A"};       //addresse
int value_h[]={0, 0};

  
//constants and variables for l states (einfaches licht / verbraucher)
const int num_l_states    = 8;
const String l_address[]  = {"LI_GR", "LI_GR_L1", "LI_GR_L2", "GR_DO_TR", "LI_44", "LI_45", "LI_46", "LI_47" };       //addresse, zum gleichschalten selbe addresse vergeben
const int l_pin[]         = {23, 25, 27, 29, 31, 33, 35, 37};                //digitaler pin
int value_l[]             = {0, 0, 0, 0, 0, 0, 0, 0};
long set_time_l[]         = {0, 0, 0, 0, 0, 0, 0, 0};



////constants and variables for r states (garage door)
// über an/aus und richtungsrelais gesteuert
const int num_r_states      = 1;
const String r_address[]    = {"GR_DO"};       //addresse
const String r_trigger[]    = {"GR_DO_TR"};         //l state
const int r_down[]          = {1};         // c state up sensor
const int r_up[]            = {2};       // c state down sensor
int value_r[]               = {0};          // -1 z, 0 unsicher, 1 auf
long lock_time_r[]          = {0};          
const long lock_delay_r     = 20000;      
  

////constants and variables for s states (dachfenster)
// üer eingänge auf und ab gesteuert
const int num_s_states=0;
const String s_address[]={};       //addresse
const String s_up[]={      };         //l state
const String s_down[]={  };         //l state
const int up_time_s[]={      };       // zeit zum öffnen in ms
const int down_time_s[]={    };       // zeit zum schließen in ms
int value_s[]={               };          // -1 zu und verriegelt, 0 entriegelt, 1 auf und verriegelt
long stop_time_s[]={         };          // zeit zu stoppen

  
  
//constants and variables for u states (temperatur steller)
const int num_u_states=0;
const String u_address[]={ };        //addresse
const int u_pin[]={ };               //pwm-pin  
const unsigned long u_interval= 240000;          //pwm periode /16 in milisekunden
unsigned long previousMillis=0;
int u_phase=0;
int value_u[]={};               //stell wert 0-15 (0=aus bis 15=voll)

  



void user_logic(){
  int i;
  //0 taster innen
  i=0;
  if(value_c[i]==1) toggle_state("LI_GR");
    
}
  
  
  
