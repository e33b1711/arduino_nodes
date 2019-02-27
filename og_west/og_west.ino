const String unit_name = "og_west";

//for com to gateway  
String inputString = "";                // a string to hold incoming data
boolean messageComplete = false;        // whether the string is complete


//dht22
#include <dht.h>
dht DHT;

//for debug interface
String db_inputString = "";                // a string to hold incoming data
boolean db_messageComplete = false;        // whether the string is complete

//constants and variables for b states (einer der 2 R-codierte Schalter an einem analogen Eingang)
int counter_a=0;
const int num_b_states=16;
const int b_pin[]={           2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};       //a state auf der selben unit                         
int value_b[]={               0, 0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,   0,  0,  0,  0};       //an/aus
int prev_value_b[]={          0, 0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,   0,  0,  0,  0};       //an/aus (alter Wert zur Flankenerkennung)

//constants and variables for c states (flanke eines b states)
//0 kurbi df hoch
//1 kuri oben 
//2 gang kurbi
//3 kurbi unten 
//4 kurbi df runter 
//5 rolllo kurbi runter
//6 rollo leo runter
//7 roll kurbi hoch
//8 rollo leo hoch
//9 hoch df leo 
//10 leo unten
//11  rot df gang
//12 runter df leo
//13  rosa df gang 
//14  gang nord unten
//15 leo oben 

const int num_c_states=16;
const int which_b[]={           0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15};       //a state auf der selben unit                         
int value_c[]={                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};       //positive flanke
int aux_value_c[]={             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};       //negative flanke
long time_c_neg[]={             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};       //zeit der letzen fallenden flanke
long time_c_pos[]={             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};       //zeit der letzen steigenden flanke


//constants and variables for d states (pwm-dimmer ausgang)
const int num_d_states=0;
const String d_address[]={      "PWM_1",      "PWM_2"};       //addresse, zum gleichschalten selbe addresse vergeben
const int d_pin[]={             2,               3};               //pwm-pin  
int value_d[]={                 0,               0};       

//constants and variables for t states (temperatur über dht22 an digitalem pin)
//                              hobby süd, hobby nord, heizkeller, lager, waschküche, gang
const int num_t_states=3;
const int period_t=10000;                                                                                  //update periode in ms
const String t_address[]={      "TI_OG_KS", "TI_OG_KN", "TI_OG_GA", "TI_50", "TI_51", "TI_52"};       
                                                                                                        //addresse
const int t_pin[]={             47,  48,  49,  50,  51, 52};
int value_t[]={                 0,   0,   0,   0,   0,   0};                                            //temperatur
int aux_value_t[]={             0,   0,   0,   0,   0,   0};                                            //feuchtigkeit
long time_t=0;                                                                                          //update timer
int i_t=0;                                                                                              //cycle_counter


//constants and variables for h states (feuchtigkeit über zustand t über dht22 an digitalem pin)
const int num_h_states=3;
const String h_address[]={      "HI_OG_KS", "HI_OG_KN", "HI_OG_GA", "HI_50", "HI_51", "HI_52"};       //addresse
int value_h[]={                 0, 0, 0, 0, 0, 0};


//constants and variables for l states (einfaches licht / verbraucher)
const int num_l_states=24;
const String l_address[]={      "LI_OG_GA_L1", "VD_OG_KS_DO", "VD_OG_KS_UP", "L25", "VD_OG_KN_UP", "LI_GA_L1", "VD_OG_KN_DO", "LI_OG_KN", "RO_OG_KN_ON", "RO_OG_KN_DO", "LI_OG_GA", "RO_OG_KS_ON", "LI_OG_KS", "LI_OG_KN_L1", "RO_OG_KS_DO", "LI_OG_KS_L1", "DF_OG_KN_DO", "DF_OG_GA_DO","DF_OG_KN_UP", "DF_OG_KS_UP", "DF_OG_GA_UP", "DF_OG_KS_DO", "L44", "L45", "L46", "L47"};       //addresse, zum gleichschalten selbe addresse vergeben
const int l_pin[]={             22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45};                //digitaler pin
int value_l[]={                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
long set_time_l[]={
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

////constants and variables for r states (rollo)
// über an/aus und richtungsrelais gesteuert
const int num_r_states=2;
const String r_address[]={      "RO_OG_KN", "RO_OG_KS"};       //addresse
const String r_on_off[]={       "RO_OG_KN_ON", "RO_OG_KS_ON"};         //l state
const String r_up_down[]={      "RO_OG_KN_DO", "RO_OG_KS_DO"};         //l state
const int up_time_r[]={            31, 31};       // zeit zum öffnen in s
const int down_time_r[]={          31, 31};       // zeit zum schließen in s
int value_r[]={                  0, 0};          // -1 zu und verriegelt, 0 entriegelt, 1 auf und verriegelt
long stop_time_r[]={              0, 0};          // zeit zu stoppen

////constants and variables for s states (dachfenster)
// üer eingänge auf und ab gesteuert
const int num_s_states=5;
const String s_address[]={      "DF_OG_KN", "DF_OG_KS", "DF_OG_GA", "VD_OG_KS", "VD_OG_KN"};       //addresse
const String s_up[]={           "DF_OG_KN_UP", "DF_OG_KS_UP", "DF_OG_GA_UP", "VD_OG_KS_UP", "VD_OG_KN_UP"};         //l state
const String s_down[]={         "DF_OG_KN_DO", "DF_OG_KS_DO", "DF_OG_GA_DO", "VD_OG_KS_DO", "VD_OG_KN_DO"};         //l state
const int up_time_s[]={         500, 500, 500, 500, 500};       // zeit zum öffnen in ms
const int down_time_s[]={       500, 500, 500, 500, 500};       // zeit zum schließen in ms
int value_s[]={                  -1, -1, -1, 0, 0};          // -1 zu und verriegelt, 0 entriegelt, 1 auf und verriegelt
long stop_time_s[]={              0, 0, 0};          // zeit zu stoppen



//constants and variables for u states (temperatur steller)
//                              0                1                  2                 3              4                5
//                              hobby1           hobby2             hobby3            lager          waschküche       gang
const int num_u_states=0;
const String u_address[]={      "U_UG_H1",       "U_UG_H2",         "U_UG_H3",        "U_UG_GA",     "U_UG_LA",       "U_UG_WK"};        //addresse
const int u_pin[]={             30,              31,                32,               33,             34,             35};               //pwm-pin  
const unsigned long u_interval= 240000;          //pwm periode /16 in milisekunden
unsigned long previousMillis=0;
int u_phase=0;
int value_u[]={                   5,               5,                 5,                 3,            14,                5};               //stell wert 0-15 (0=aus bis 15=voll)











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
  
  
  
  //taster verküpfungen
  // 14  Gang, KZ nord, einzel
  // 13  DF
  // 11  "   
  i=14;
   if (value_c[i]==-1){
     if (time_c_pos[i]+700>time_c_neg[i]){
      write_state("LI_OG_GA",3);
     }
     else{
     
      write_state("LI_OG_GA_L1",3);
     }
  }
  i=13;
  if (value_c[i]==1){
     write_state("DF_OG_GA",1);
  }
  i=11;
  if (value_c[i]==1){
     write_state("DF_OG_GA",-1);
  }
  //2  Gang, KZ süd
  i=2;
  if (value_c[i]==-1){
     if (time_c_pos[i]+700>time_c_neg[i]){
      write_state("LI_OG_GA",3);
     }
     else{
     
      write_state("LI_OG_GA_L1",3);
     }
  }
  // 10  KZ nord unten
  // 15  "   oben
  //9  DF KZ nord
  //12
  i=10;
  if (value_c[i]==1){
     write_state("LI_OG_KN",3);
  }
  i=15;
  if (value_c[i]==1){
     write_state("LI_OG_KN_L1",3);
  }
  i=9;
   if (value_c[i]==-1){
    //verriegeln auf auf
     if (time_c_pos[i]+700>time_c_neg[i]){
      write_state("DF_OG_KN",1);
     }
     else{
       write_state("VD_OG_KN",1);
     }
  }
  i=12;
   if (value_c[i]==-1){
    //verriegeln auf auf
     if (time_c_pos[i]+700>time_c_neg[i]){
      write_state("DF_OG_KN",-1);
     }
     else{
       write_state("VD_OG_KN",-1);
     }
  }
  
  
  
  //8  Rollo, KZ nord
  //6 
   i=8;
  
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
  
  
  
  
  
  //3  KZ süd oben
  //1  "    unten
  //0  DF KZ süd
  //4  "
  i=3;
  if (value_c[i]==1){
     write_state("LI_OG_KS",3);
  }
  i=1;
  if (value_c[i]==1){
     write_state("LI_OG_KS_L1",3);
  }
  i=0;
   if (value_c[i]==-1){
    //verriegeln auf auf
     if (time_c_pos[i]+700>time_c_neg[i]){
      write_state("DF_OG_KS",1);
     }
     else{
       write_state("VD_OG_KS",1);
     }
  }
  i=4;
   if (value_c[i]==-1){
    //verriegeln auf auf
     if (time_c_pos[i]+700>time_c_neg[i]){
      write_state("DF_OG_KS",-1);
     }
     else{
       write_state("VD_OG_KS",-1);
     }
  }
  
  
  
  
  
  //5??  Rollo, KZ süd
  //7??  
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




