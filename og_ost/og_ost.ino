//this nodes name
//const char* unit_name   = "og_ost";
const char* unit_name   = "test";
const char* password    = "pass";

  
//for tcp communication
//watch out for the pins needed for the ethernet schield (always 10, 11 12 13 on uno, 50 51 52 53 on mega!)
#include <Ethernet.h>
const byte mac[]                = {0xDE, 0xAA, 0x7E, 0xE1, 0x11, 0x15 };
//const byte mac[]                = {0xDE, 0xAA, 0x7E, 0xE1, 0x1E, 0x15 };
const IPAddress                 ip(192,168,178,77);
//const IPAddress                 server(192,168,178,222);
const IPAddress                 server(192,168,178,222);
const IPAddress                 gateway(192,168,178,1);
const IPAddress                 subnet(255,255,255,0);
const int port                  = 8888;
const int broker_port           = 1883;
const int ethernet_sc_pin       = 53;
const int ethernet_reset_pin    = 12;
const String command_prefix     = "ard_command/";
const String state_prefix       = "ard_state/";


//constants and variables for b states (einer der 2 R-codierte Schalter an einem analogen Eingang)
int counter_a=0;
const int num_b_states  = 16;
const int b_pin[]       = {4,  5,  6,  7,  8,  9,  10, 11, 25, 24, 23, 22, 17, 16, 3,  2, 33, 32, 31, 30, 29, 28, 27, 26};                           
int value_b[]           = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};    
int prev_value_b[]      = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};  


//constants and variables for c states (flanke eines b states)
//0 sz unten
//1 ans
//2 sz rollo runter
//3 sz oben 
//4 bewegung
//5 mell
//6 sz rollohoch
//7 gang sz
//8 bad runter tür
//9 gang bad
//10 bad 
//11 bad hoch tür
//12 bad fenster runter
//13 bad fenster hoch
//14 df sz hoch
//15 df sz runter
const int num_c_states  = 16;
const int which_b[]     = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};       //a state auf der selben unit                         
int value_c[]           = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};           //positive flanke
int aux_value_c[]       = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};          //negative flanke
long time_c_neg[]       = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};           //zeit der letzen fallenden flanke
long time_c_pos[]       = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};        //zeit der letzen steigenden flanke


//constants and variables for t states (temperatur über dht22 an digitalem pin)
#define DHTTYPE DHT22
const int num_t_states      = 2;
const long period_t         = 1800000;                                                                                  //update periode in ms
const String t_address[]    = {"TI_OG_BA", "TI_OG_SZ"};                                     
const int t_pin[]           = {27, 26};
int value_t[]               = {0,  0};                                            //temperatur
int aux_value_t[]           = {0,  0};                                           //feuchtigkeit
long s_time_t               = 0;                                                                                          //update timer
int i_t                     = 0;                                                                                              //cycle_counter


//constants and variables for h states (feuchtigkeit über zustand t über dht22 an digitalem pin)
const int num_h_states      = 2;
const String h_address[]    = {"HI_OG_BA", "HI_OG_SZ"};       //addresse
int value_h[]               = {0,  0};    


//constants and variables for l states (einfaches licht / verbraucher)
// 41 roolo on
// 42 bad
// 43 rollo bad do
// 44 sz
// 45 anselm
// 46 rollo do
// 47 melli
// 48 steckdose?
// 49 rollo bad on
// 60 vd zu
// 58 vd auf
// 59 df zu
// 61 df auf
const int num_l_states      = 17;
const String l_address[]    = {"RO_OG_SZ_ON", "LI_OG_BA", "RO_OG_BA_DO", "LI_OG_SZ", "LI_OG_SZ_L2", "RO_OG_SZ_DO", "LI_OG_SZ_L1", "LI_48", "RO_OG_BA_ON", "LI_54", "LI_55", "LI_56", "LI_57", "VD_OG_SZ_DO", "DF_OG_SZ_DO", "VD_OG_SZ_UP", "DF_OG_SZ_UP"};       //addresse, zum gleichschalten selbe addresse vergeben
const int l_pin[]           = {41, 42, 43, 44, 45, 46, 47, 48, 49, 54, 55, 56, 57, 58, 59, 60, 61};                //digitaler pin
const bool l_inv[]          = {1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0};                //digitaler pin
int value_l[]               = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};    
long set_time_l[]           = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};    



////constants and variables for r states (rollo)
// über an/aus und richtungsrelais gesteuert
const int num_r_states          = 2;
const String r_address[]        = {"RO_OG_SZ", "RO_OG_BA"};                 //addresse
const String r_on_off[]         = {"RO_OG_SZ_ON", "RO_OG_BA_ON"};           //l state
const String r_up_down[]        = {"RO_OG_SZ_DO", "RO_OG_BA_DO"};           //l state
const int up_time_r[]           = {31, 31};                                 // zeit zum öffnen in s
const int down_time_r[]         = {31, 31};                                 // zeit zum schließen in s
int value_r[]                   = {0, 0};                                   // -1 zu und verriegelt, 0 entriegelt, 1 auf und verriegelt
long stop_time_r[]              = {0, 0};                                   // zeit zu stoppen


////constants and variables for s states (dachfenster)
// üer eingänge auf und ab gesteuert
const int num_s_states      = 2;
const String s_address[]    = {"DF_OG_SZ", "VD_OG_SZ"};       //addresse
const String s_up[]         = {"DF_OG_SZ_UP", "VD_OG_SZ_UP"};         //l state
const String s_down[]       = {"DF_OG_SZ_DO", "VD_OG_SZ_DO"};         //l state
const int up_time_s[]       = {500, 500};       // zeit zum öffnen in ms
const int down_time_s[]     = {500, 500};       // zeit zum schließen in ms
int value_s[]               = {-1, 0};          // -1 zu und verriegelt, 0 entriegelt, 1 auf und verriegelt
long stop_time_s[]          = {0, 0};          // zeit zu stoppen


//constants and variables for u states (temperatur steller)
//0 blau U_OG_GA
//1 weiß U_OG_KS
//2 grün U_OG_KN
//3 rosa U_OG_BA
//4 grau U_OG_SZ
//5 
//6 
//7 
const int num_u_states          = 5;
const String u_address[]        = {"U_OG_GA", "U_OG_KS", "U_OG_KN", "U_OG_BA", "U_OG_SZ"};        //addresse
const int u_pin[]               = {34, 35, 36, 37, 38};               //pwm-pin  
const unsigned long u_interval  = 240000;          //pwm periode /16 in milisekunden
unsigned long previousMillis    = 0;
int u_phase                     = 4;
int value_u[]                   = {3, 3, 3, 3, 3};               //stell wert 0-15 (0=aus bis 15=voll)


void user_logic()
{
    int i;  

  //taFer verküpfungen
  //3  sz, tür, mitte
  //0  sz, tür, unten
  //14  df sz
  //15  df sz
  i=3;
   if (value_c[i]==-1){
    //verriegeln auf auf
     if (time_c_pos[i]+700>time_c_neg[i]){
      toggle_state("LI_OG_SZ");
     }
     else{
       write_state("LI_OG_SZ",0);
     write_state("LI_OG_SZ_L1",0);
     write_state("LI_OG_SZ_L2",0);
     }
  }
  i=0;
  if (value_c[i]==-1){
    //verriegeln auf auf
     if (time_c_pos[i]+700>time_c_neg[i]){
      toggle_state("LI_OG_SZ");
     }
     else{
       write_state("LI_OG_SZ",0);
     write_state("LI_OG_SZ_L1",0);
     write_state("LI_OG_SZ_L2",0);
     }
  }
  i=14;
   if (value_c[i]==-1){
    //verriegeln auf auf
     if (time_c_pos[i]+700>time_c_neg[i]){
      write_state("DF_OG_SZ",1);
     }
     else if (time_c_pos[i]+2000>time_c_neg[i]){
       write_state("VD_OG_SZ",1);
     }
       else {
       write_state("VD_OG",1);
     }
  }
  i=15;
   if (value_c[i]==-1){
    //verriegeln auf auf
     if (time_c_pos[i]+700>time_c_neg[i]){
      write_state("DF_OG_SZ",-1);
     }
     else if (time_c_pos[i]+2000>time_c_neg[i]){
       write_state("VD_OG_SZ",-1);
     }
       else {
       write_state("VD_OG",-1);
     }
  }
  
  
  
  //6  rollo sz
  //2  rollo sz
  i=6;
  if (value_c[i]==1){
    //entriegeln
     write_state("RO_OG_SZ",0);
     //aktivieren
     write_state("RO_OG_SZ_ON",1);
     write_state("RO_OG_SZ_DO",0);
  }
  //negative flanke, wenn weniger als 1 sekunde nach positiver dann wird verriegelt
  if (value_c[i]==-1){
    //verriegeln auf auf
     if (time_c_pos[i]+1000>time_c_neg[i]){
       write_state("RO_OG_SZ",1);
     }
     else{
        write_state("RO_OG_SZ_ON",0);
        write_state("RO_OG_SZ_DO",0);
     }
  }
  i=2;
  if (value_c[i]==1){
    //entriegeln
     write_state("RO_OG_SZ",0);
     //aktivieren
     write_state("RO_OG_SZ_ON",1);
     write_state("RO_OG_SZ_DO",1);
  }
  if (value_c[i]==-1){
    //verriegeln
     if (time_c_pos[i]+1000>time_c_neg[i]){
       write_state("RO_OG_SZ",-1);
     }
     else{
        write_state("RO_OG_SZ_ON",0);
        write_state("RO_OG_SZ_DO",0);
     }
  }
  //5  bett, west
  //1  bett ost
  i=5;
  if (value_c[i]==-1){
    //verriegeln auf auf
     if (time_c_pos[i]+700>time_c_neg[i]){
      toggle_state("LI_OG_SZ_L1");
     }
     else{
       write_state("LI_OG_SZ",0);
     write_state("LI_OG_SZ_L1",0);
     write_state("LI_OG_SZ_L2",0);
     }
  }
  i=1;
  if (value_c[i]==-1){
    //verriegeln auf auf
     if (time_c_pos[i]+700>time_c_neg[i]){
      toggle_state("LI_OG_SZ_L2");
     }
     else{
       write_state("LI_OG_SZ",0);
       write_state("LI_OG_SZ_L1",0);
       write_state("LI_OG_SZ_L2",0);
     }
  }
  //7  flur, sz
  i=7;
  if (value_c[i]==-1){
     if (time_c_pos[i]+700>time_c_neg[i]){
       write_state("LI_OG_GA_L1", 3);
     }
     else{
       write_state("LI_OG_GA", 3);
     }
  }
  
  //9  flur, bad
  i=9;
  if (value_c[i]==-1){
     if (time_c_pos[i]+700>time_c_neg[i]){
       write_state("LI_OG_GA_L1", 3);
     }
     else{
       write_state("LI_OG_GA", 3);
     }
  }
  //10  bad, einzel
  //11  rollo bad tür
  //8  rollo bad tür
  i=10;
  if (value_c[i]==1){
     toggle_state("LI_OG_BA");
  }
   i=11;
  if (value_c[i]==1){
    //entriegeln
     write_state("RO_OG_BA",0);
     //aktivieren
     write_state("RO_OG_BA_ON",1);
     write_state("RO_OG_BA_DO",0);
  }
  //negative flanke, wenn weniger als 1 sekunde nach positiver dann wird verriegelt
  if (value_c[i]==-1){
    //verriegeln auf auf
     if (time_c_pos[i]+1000>time_c_neg[i]){
       write_state("RO_OG_BA",1);
     }
     else{
        write_state("RO_OG_BA_ON",0);
        write_state("RO_OG_BA_DO",0);
     }
  }
  i=8;
  if (value_c[i]==1){
    //entriegeln
     write_state("RO_OG_BA",0);
     //aktivieren
     write_state("RO_OG_BA_ON",1);
     write_state("RO_OG_BA_DO",1);
  }
  if (value_c[i]==-1){
    //verriegeln
     if (time_c_pos[i]+1000>time_c_neg[i]){
       write_state("RO_OG_BA",-1);
     }
     else{
        write_state("RO_OG_BA_ON",0);
        write_state("RO_OG_BA_DO",0);
     }
  }
  //13   rollo bad balkon
 //12   rollo bad balkon
   i=13;
  if (value_c[i]==1){
    //entriegeln
     write_state("RO_OG_BA",0);
     //aktivieren
     write_state("RO_OG_BA_ON",1);
     write_state("RO_OG_BA_DO",0);
  }
  //negative flanke, wenn weniger als 1 sekunde nach positiver dann wird verriegelt
  if (value_c[i]==-1){
    //verriegeln auf auf
     if (time_c_pos[i]+1000>time_c_neg[i]){
       write_state("RO_OG_BA",1);
     }
     else{
       write_state("RO_OG_BA_ON",0);
       write_state("RO_OG_BA_DO",0);
     }
  }
  i=12;
  if (value_c[i]==1){
    //entriegeln
     write_state("RO_OG_BA",0);
     //aktivieren
     write_state("RO_OG_BA_ON",1);
     write_state("RO_OG_BA_DO",1);
  }
  if (value_c[i]==-1){
    //verriegeln
     if (time_c_pos[i]+1000>time_c_neg[i]){
       write_state("RO_OG_BA",-1);
     }
     else{
       write_state("RO_OG_BA_ON",0);
       write_state("RO_OG_BA_DO",0);
     }
  }

//4 bewegungsmelder
i=4;
 if (value_c[i]==1){
    //entriegeln
     write_state("ZE_EG_VH",1);
     write_state("ZE_GR_2",1);
  }
}
