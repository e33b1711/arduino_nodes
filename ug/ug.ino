//this nodes name
const String unit_name = "ug";

  
//for tcp communication
//watch out for the pins needed for the ethernet schield (always 10, 11 12 13 on uno, 50 51 52 53 on mega!)
#include <Ethernet.h>
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x05 };
const int ethernet_sc_pin = 53;
IPAddress ip(192,168,178,204);
IPAddress server(192,168,178,222);
int port = 8888;

  
//constants and variables for b states (einer der 2 R-codierte Schalter an einem analogen Eingang)
int counter_a=0;
const int num_b_states=8;
const int b_pin[]={           
2, 3, 4, 5, 6, 7, 8, 9, 7, 11, 12, 13, 14, 15, 16, 17};       //a state auf der selben unit                         
int value_b[]={               
0, 0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,   0,  0,  0,  0};       //an/aus
int prev_value_b[]={          
0, 0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,   0,  0,  0,  0};       //an/aus (alter Wert zur Flankenerkennung)


//constants and variables for c states (flanke eines b states)
//0 
//1 
//2 GA_HK
//3 HK
//4 GA_notHK
//5 LA
//6 WK
//7 HO
//8 
//9 
//10 
//11
//12 
//13
//14
//15 
const int num_c_states=15;
const int which_b[]={           
0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15};       //a state auf der selben unit                         
int value_c[]={                 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};       //positive flanke
int aux_value_c[]={             
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};       //negative flanke
long time_c_neg[]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};       //zeit der letzen fallenden flanke
long time_c_pos[]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};       //zeit der letzen steigenden flanke
       

//constants and variables for t states (temperatur über dht22 an digitalem pin)
const int num_t_states=5;
const long period_t=1800000;                                                                                  //update periode in ms
const String t_address[]={    
"TI_UG_WK", "TI_UG_HN", "TI_UG_LA", "TI_UG_HS", "TI_UG_HK", "TI_UG_GA"};                                       //addresse
const int t_pin[]={             
40,  41,  42,  43,  44, 45};
int value_t[]={                 
0,   0,   0,   0,   0,   0};                                            //temperatur
int aux_value_t[]={             
0,   0,   0,   0,   0,   0};                                            //feuchtigkeit
long time_t=0;                                                                                          //update timer
int i_t=0;                                                                                              //cycle_counter


//constants and variables for h states (feuchtigkeit über zustand t über dht22 an digitalem pin)
const int num_h_states=5;
const String h_address[]={      
"HI_UG_WK", "HI_UG_HN", "HI_UG_LA", "HI_UG_HS", "HI_UG_HK", "HI_UG_GA"};       //addresse
int value_h[]={0, 0, 0, 0, 0, 0};


//constants and variables for l states (einfaches licht / verbraucher)
const int num_l_states=8;
const String l_address[]={      "BELL", "LI_UG_HO", "PUMP", "LI_UG_LA", "LI_UG_GA", "LI_UG_WK", "LI_UG_GA", "LI_UG_HK"};       //addresse, zum gleichschalten selbe addresse vergeben
const int l_pin[]={             22,24,26,28,30,32,34,36};                //digitaler pin
int value_l[]={                 0, 0, 0, 0, 0, 0, 0, 0};
long set_time_l[]={               0, 0, 0, 0, 0, 0, 0, 0};


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
//                              0                1                  2                 3              4                5
//                              hobby1           hobby2             hobby3            gang          lager             waschküche
const int num_u_states=8;
const String u_address[]={"U_UG_WK", "U_UG_LA",       "U_UG_02",         "U_UG_GA",        "U_UG_H1",     "U_UG_H2",       "U_UG_H3",    "U_UG_07"};        //addresse
const int u_pin[]={23,              25,                27,               29,             31,             33, 35, 37};               //pwm-pin  
const unsigned long u_interval= 240000;          //pwm periode /16 in milisekunden
unsigned long previousMillis=0;
int u_phase=0;
int value_u[]={3, 3, 3, 3, 3, 3, 3, 3};               //stell wert 0-15 (0=aus bis 15=voll)


void user_logic()
{
    int i;

  //2 GA_HK
//3 HK
//4 GA_notHK
//5 LA
//6 WK
//7 HO
  

  //taster verküpfungen
  //3 HK
  i=3;
    if (value_c[i]==1){
     toggle_state("LI_UG_HK");
    }
  //4, 2 Gang
  i=4;
  if (value_c[i]==1){
     toggle_state("LI_UG_GA");
    
  }
  i=2;
  if (value_c[i]==1){
     toggle_state("LI_UG_GA");
    
  }
  //7 Hobby
  i=7;
  if (value_c[i]==1){
     toggle_state("LI_UG_HO");
  }
  //5 Lager
  i=5;
  if (value_c[i]==1){
     toggle_state("LI_UG_LA");
  }
  //6 Waschküche
  i=6;
  if (value_c[i]==1){
     toggle_state("LI_UG_WK");
  }
}
