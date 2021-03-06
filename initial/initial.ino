//this nodes name
const char* unit_name   = "initial";
const char* password    = "pass";

  
//for tcp communication
//watch out for the pins needed for the ethernet schield (always 10, 11 12 13 on uno, 50 51 52 53 on mega!)
#include <Ethernet.h>
const byte mac[]                = {0xDE, 0xAA, 0x7E, 0xE1, 0x1E, 0x18 };
const IPAddress                 ip(192,168,178,219);
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
//0 
//1 
//2 
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
const int num_c_states  = 16;
const int which_b[]     = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};       //a state auf der selben unit                         
int value_c[]           = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};           //positive flanke
int aux_value_c[]       = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};          //negative flanke
long time_c_neg[]       = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};           //zeit der letzen fallenden flanke
long time_c_pos[]       = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};        //zeit der letzen steigenden flanke
       

//constants and variables for t states (temperatur über dht22 an digitalem pin)
#define DHTTYPE DHT22
const int num_t_states      = 8;
const long period_t         = 1800000;                                                                                  //update periode in ms
const String t_address[]    = {"TI_33", "TI_32", "TI_31", "TI_30", "TI_29", "TI_28", "TI_27", "TI_26"};                                     
const int t_pin[]           = {33, 32, 31, 30, 29, 28, 27, 26};
int value_t[]               = {0,  0,  0,  0,  0,  0,  0,  0};                                            //temperatur
int aux_value_t[]           = {0,  0,  0,  0,  0,  0,  0,  0};                                           //feuchtigkeit
long s_time_t               = 0;                                                                                          //update timer
int i_t                     = 0;                                                                                              //cycle_counter


//constants and variables for h states (feuchtigkeit über zustand t über dht22 an digitalem pin)
const int num_h_states      = 0;
const String h_address[]    = {"HI_33", "HI_32", "HI_31", "HI_30", "HI_29", "HI_28", "HI_27", "HI_26"};          //addresse
int value_h[]               = {0,  0,  0,  0,  0,  0,  0,  0};    


//constants and variables for l states (einfaches licht / verbraucher)
const int num_l_states      = 24;
const String l_address[]    = {"LI_34", "LI_35", "LI_36", "LI_37", "LI_38", "LI_39", "LI_40", "LI_41", "LI_42", "LI_43", "LI_44", "LI_45", "LI_46", "LI_47", "LI_48", "LI_49", "LI_54", "LI_55", "LI_56", "LI_57", "LI_58", "LI_59", "LI_60", "LI_61"};       //addresse, zum gleichschalten selbe addresse vergeben
const int l_pin[]           = {34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 54, 55, 56, 57, 58, 59, 60, 61};                //digitaler pin
const bool l_inv[]          = {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0};                //digitaler pin
int value_l[]               = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};    
long set_time_l[]           = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};    


////constants and variables for r states (rollo)
// über an/aus und richtungsrelais gesteuert
const int num_r_states      = 0;
const String r_address[]    = {"RO_EG_SU", "RO_EG_WE"};       //addresse
const String r_on_off[]     = {"RO_EG_SU_ON", "RO_EG_WE_ON"};         //l state
const String r_up_down[]    = {"RO_EG_SU_DO", "RO_EG_WE_DO"};         //l state
const int up_time_r[]       = {25, 25};       // zeit zum öffnen in s
const int down_time_r[]     = {22, 22};       // zeit zum schließen in s
int value_r[]               = {0, 0};          // -1 zu und verriegelt, 0 entriegelt, 1 auf und verriegelt
long stop_time_r[]          = {0, 0};          // zeit zu stoppen


////constants and variables for s states (dachfenster)
// üer eingänge auf und ab gesteuert
const int num_s_states      = 2;
const String s_address[]    = {"DF_00", "DF_01"};       //addresse
const String s_up[]         = {"LI_A0", "LI_A2"};         //l state
const String s_down[]       = {"LI_A1", "LI_A3"};          //l state
const int up_time_s[]       = {10, 10};       // zeit zum öffnen in ms
const int down_time_s[]     = {200, 200};       // zeit zum schließen in ms
int value_s[]               = {-1, -1};          // -1 zu und verriegelt, 0 entriegelt, 1 auf und verriegelt
long stop_time_s[]          = {0,  0};          // zeit zu stoppen


//constants and variables for u states (temperatur steller)
//0 
//1 
//2 
//3 
//4 
//5 
//6 
//7 
const int num_u_states          = 0;
const String u_address[]        = {"U_34", "U_35", "U_36", "U_37", "U_38", "U_39", "U_40", "U_41"};        //addresse
const int u_pin[]               = {34, 35, 36, 37, 38, 39, 40, 41};               //pwm-pin  
const unsigned long u_interval  = 240000;          //pwm periode /16 in milisekunden
unsigned long previousMillis    = 0;
int u_phase                     = 0;
int value_u[]                   = {3, 3, 3, 3, 3, 3, 3, 3};               //stell wert 0-15 (0=aus bis 15=voll)


void user_logic()
{

/*
  int pin = A0;
  Serial.println(pin);
   pin = A1;
  Serial.println(pin);
   pin = A2;
  Serial.println(pin);
   pin = A3;
  Serial.println(pin);
   pin = A4;
  Serial.println(pin);
   pin = A5;
  Serial.println(pin);
   pin = A6;
  Serial.println(pin);
   pin = A7;
  Serial.println(pin);

  while(true){}
  */
 
  
  
}
