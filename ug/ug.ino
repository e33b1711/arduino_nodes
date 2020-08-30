//this nodes name
const char* unit_name   = "ug";
const char* password    = "pass";

  
//for tcp communication
//watch out for the pins needed for the ethernet schield (always 10, 11 12 13 on uno, 50 51 52 53 on mega!)
#include <Ethernet.h>
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x05 };
const int ethernet_sc_pin = 53;
IPAddress ip(192,168,178,211);
IPAddress server(192,168,178,222);
int port = 8888;
const int ethernet_reset_pin = 12;

  
//constants and variables for b states (einer der 2 R-codierte Schalter an einem analogen Eingang)
int counter_a=0;
const int num_b_states      = 8;
const int b_pin[]           = {2,  3,  16, 17, 22, 23, 24, 25};       //a state auf der selben unit                         
int value_b[]               = {0,  0,  0,  0,  0,  0,  0,  0};       //an/aus
int prev_value_b[]          = {0,  0,  0,  0,  0,  0,  0,  0};       //an/aus (alter Wert zur Flankenerkennung)


//constants and variables for c states (flanke eines b states)
//0 
//1 
//2 HK
//3 GA_HK
//4 GA
//5 HN
//6 WK
//7 HS
//8 
//9 
//10 
//11
//12 
//13
//14
//15 
const int num_c_states  = 8;
const int which_b[]     = {0, 1, 2, 3, 4, 5, 6, 7};       //a state auf der selben unit                         
int value_c[]           = {0, 0, 0, 0, 0, 0, 0, 0};       //positive flanke
int aux_value_c[]       = {0, 0, 0, 0, 0, 0, 0, 0};       //negative flanke
long time_c_neg[]       = {0, 0, 0, 0, 0, 0, 0, 0};       //zeit der letzen fallenden flanke
long time_c_pos[]       = {0, 0, 0, 0, 0, 0, 0, 0};       //zeit der letzen steigenden flanke
       

//constants and variables for t states (temperatur über dht22 an digitalem pin)
const int num_t_states      = 6;
const long period_t         = 1800000;                                                                                  //update periode in ms
const String t_address[]    = {"TI_UG_WK", "TI_UG_HN", "TI_UG_LA", "TI_UG_GA", "TI_UG_HK", "TI_UG_HS",};                                       //addresse
const int t_pin[]           = {26, 27, 28, 29, 30, 31};
int value_t[]               = {0,  0,  0,  0,  0,  0};                                            //temperatur
int aux_value_t[]           = {0,  0,  0,  0,  0,  0};                                            //feuchtigkeit
long time_t                 = 0;                                                                                          //update timer
int i_t                     = 0;                                                                                              //cycle_counter


//constants and variables for h states (feuchtigkeit über zustand t über dht22 an digitalem pin)
const int num_h_states      = 6;
const String h_address[]    = {"HI_UG_WK", "HI_UG_HN", "HI_UG_LA", "HI_UG_GA", "HI_UG_HK", "HI_UG_HS"};          //addresse 
int value_h[]               = {0, 0, 0, 0, 0, 0};


//constants and variables for l states (einfaches licht / verbraucher)
const int num_l_states=8;
const String l_address[]    = {"LI_UG_HO", "LI_UG_HK", "LI_UG_GA", "BELL", "PUMP", "LI_UG_GA", "LI_UG_WK", "LI_UG_HN"};       //addresse, zum gleichschalten selbe addresse vergeben
const int l_pin[]           = {42, 43, 44, 45, 46, 47, 48, 49};                //digitaler pin
const bool l_inv[]          = {1,  1,  1,  1,  1,  1,  1,  1};                //digitaler pin
int value_l[]               = {0,  0,  0,  0,  0,  0,  0,  0};
long set_time_l[]           = {0,  0,  0,  0,  0,  0,  0,  0};


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
const int num_s_states      = 0;
const String s_address[]    = {"DF_00", "DF_01"};       //addresse
const String s_up[]         = {"LI_A0", "LI_A2"};         //l state
const String s_down[]       = {"LI_A1", "LI_A3"};          //l state
const int up_time_s[]       = {10, 10};       // zeit zum öffnen in ms
const int down_time_s[]     = {200, 200};       // zeit zum schließen in ms
int value_s[]               = {-1, -1};          // -1 zu und verriegelt, 0 entriegelt, 1 auf und verriegelt
long stop_time_s[]          = {0,  0};          // zeit zu stoppen



//constants and variables for u states (temperatur steller)
//                              0                1                  2                 3              4                5
//                              hobby1           hobby2             hobby3            gang          lager             waschküche
const int num_u_states          = 6;
const String u_address[]        = {"U_UG_WK", "U_UG_LA", "U_UG_GA", "U_UG_H1", "U_UG_H2", "U_UG_H3"};        //addresse
const int u_pin[]               = {34, 35, 36, 37, 38, 39, 40, 41};               //pwm-pin  
const unsigned long u_interval  = 240000;          //pwm periode /16 in milisekunden
unsigned long previousMillis    = 0;
int u_phase                     = 0;
int value_u[]                   = {3,  3,  3,  3,  3,  3,  3,  3};               //stell wert 0-15 (0=aus bis 15=voll)


void user_logic()
{
    int i;

//2 HK
//3 GA_HK
//4 GA
//5 HN
//6 WK
//7 HS


  //taster verküpfungen
  //2 HK
  i=2;
    if (value_c[i]==1){
     toggle_state("LI_UG_HK");
    }
  //3, 4 Gang
  i=3;
  if (value_c[i]==1){
     toggle_state("LI_UG_GA");
    
  }
  i=4;
  if (value_c[i]==1){
     toggle_state("LI_UG_GA");
    
  }
  //7 Hobby Süd
  i=7;
  if (value_c[i]==-1){
    //verriegeln auf auf
     if (time_c_pos[i]+700>time_c_neg[i]){
       toggle_state("LI_UG_HO");
     }
     else{
       write_state("LI_UG_HN",0);
       write_state("LI_UG_HO",0);
       Serial.println("hello4");
     }
  }
  //5 Hobby Nord
  i=5;
  if (value_c[i]==-1){
    //verriegeln auf auf
     if (time_c_pos[i]+700>time_c_neg[i]){
       toggle_state("LI_UG_HN");
     }
     else{
       write_state("LI_UG_HN",0);
       write_state("LI_UG_HO",0);
       Serial.println("hello5");
     }
  }
  //6 Waschküche
  i=6;
  if (value_c[i]==1){
     toggle_state("LI_UG_WK");
  }

  //turn off BELL trigger after one second
  i=3;
  if ((value_l[i]==1) && set_time_l[i]+5000<millis()){
    write_l("BELL",0); 
    send_message("w", l_address[i], value_l[i]);
  }
  
  
}
