//this nodes name
const String unit_name = "eg_west";

  
//for tcp communication
//watch out for the pins needed for the ethernet schield (always 10, 11 12 13 on uno, 50 51 52 53 on mega!)
#include <Ethernet.h>
const int ethernet_sc_pin = 10;
const byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x01 };
const IPAddress ip(192,168,178,200);
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
//0 ez gang mitte
//1 ez gang rollo hoch
//2  ez gang rolllo runter
//3 gang ez
//4 ez gang unten
//5 gang keller oben
//6 ez su runter
//7 gang keller unten
//8 xxx
//9 ez we hoch
//10 ez su unten
//11 ez we runter
//12 ku
//13 ez su hoch
//14 ez we unten
//15 


const int num_c_states=16;
const int which_b[]={           
0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15};       //a state auf der selben unit                         
int value_c[]={                 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};       //positive flanke
int aux_value_c[]={             
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};       //negative flanke
long time_c_neg[]={             
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};       //zeit der letzen fallenden flanke
long time_c_pos[]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};       //zeit der letzen steigenden flanke
  

//constants and variables for t states (temperatur über dht22 an digitalem pin)
//achtung 2mal 48, ändern!!!!!!!!
const int num_t_states=4;
const long period_t=1800000;                                                                                //update periode in ms
const String t_address[]={       
"TI_EG_WZ", "TI_EG_KU", "TI_EG_EZ", "TI_EG_GA"};                                                                                                           //addresse
const int t_pin[]={             
46, 47,  48,  49};
int value_t[]={                 
0,   0,   0,   0};                                            //temperatur
int aux_value_t[]={             
0,   0,   0,   0};                                            //feuchtigkeit
long time_t=0;                                                                                          //update timer
int i_t=0;                                                                                              //cycle_counter
  

//constants and variables for h states (feuchtigkeit über zustand t über dht22 an digitalem pin)
const int num_h_states=4;
const String h_address[]={"HI_EG_WZ", "HI_EG_KU", "HI_EG_EZ", "HI_EG_GA"};       //addresse
int value_h[]={0, 0, 0, 0, 0};

  
//constants and variables for l states (einfaches licht / verbraucher)
const int num_l_states=16;
const String l_address[]={      
"LI_EG_AS", "RO_EG_WE_DO", "RO_EG_WE_ON", "RO_EG_SU_ON", "LI_EG_KU_L1", "LI_EG_EZ_KU", "LI_EG_SP", "RO_EG_SU_DO", "LI_EG_AO", "LI_EG_EZ_L1", "LI_EG_EZ_L3", "LI_EG_GA", "LI_EG_AW", "LI_GA_L1", "LI_EG_EZ_KU", "LI_EG_EZ", "L38", "L39","L40", "L41", "L42", "L43", "L44", "L45", "L46", "L47"};       //addresse, zum gleichschalten selbe addresse vergeben
const int l_pin[]={22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45};                //digitaler pin
int value_l[]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
long set_time_l[]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  

////constants and variables for r states (rollo)
// über an/aus und richtungsrelais gesteuert
const int num_r_states=2;
const String r_address[]={      
"RO_EG_SU", "RO_EG_WE"};       //addresse
const String r_on_off[]={       
"RO_EG_SU_ON", "RO_EG_WE_ON"};         //l state
const String r_up_down[]={      
"RO_EG_SU_DO", "RO_EG_WE_DO"};         //l state
const int up_time_r[]={            
31, 31};       // zeit zum öffnen in s
const int down_time_r[]={          
31, 31};       // zeit zum schließen in s
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
//                              hobby1           hobby2             hobby3            lager          waschküche       gang
const int num_u_states=0;
const String u_address[]={      
"U_UG_H1",       "U_UG_H2",         "U_UG_H3",        "U_UG_GA",     "U_UG_LA",       "U_UG_WK"};        //addresse
const int u_pin[]={ 30,              31,                32,               33,             34,             35};               //pwm-pin  
const unsigned long u_interval= 240000;          //pwm periode /16 in milisekunden
unsigned long previousMillis=0;
int u_phase=0;
int value_u[]={5,               5,                 5,                 3,            14,                5};               //stell wert 0-15 (0=aus bis 15=voll)


void user_logic(){
  int i;
    
  //taster verküpfungen
  //3  gang, ez
  i=3;
  if (value_c[3]==1){
     toggle_state("LI_EG_GA");
     //write_state("LI_GA_L1",3);
  }
  //4, ez, gang, unten
  //0  ", mitte
  //1  ez, gang, rollo
  //2  "
  i=4;
  if (value_c[i]==1){
     toggle_state("LI_EG_EZ_KU");
  }
  i=0;
  if (value_c[i]==-1){
    //verriegeln auf auf
    if (time_c_pos[i]+700>time_c_neg[i]){
      toggle_state("LI_EG_EZ");
    }
    else{
      toggle_state("LI_EG_EZ_L1");
    }
  }
  i=1;
  if (value_c[i]==1){
    write_state("RO_EG_SU",1);
    write_state("RO_EG_WE",1);
  }
  i=2;
  if (value_c[i]==1){
    write_state("RO_EG_SU",-1);
    write_state("RO_EG_WE",-1);
  }



  
  //10  ez, süd, unten
  //13  ez, süd, rollo
  //6  "
  i=10;
  if (value_c[i]==-1){
    if (time_c_pos[i]+700>time_c_neg[i]){
      toggle_state("LI_EG_AS");
    }
    else{
      write_state("PUMP",3);
    }
  }
  i=13;
  if (value_c[i]==1){
    //entriegeln
    write_state("RO_EG_SU",0);
    //aktivieren
    write_state("RO_EG_SU_ON",1);
    write_state("RO_EG_SU_DO",0);
  }
  //negative flanke, wenn weniger als 1 sekunde nach positiver dann wird verriegelt
  if (value_c[i]==-1){
    //verriegeln auf auf
    if (time_c_pos[i]+1000>time_c_neg[i]){
      write_state("RO_EG_SU",1);
    }
    else{
      write_state("RO_EG_SU_ON",0);
      write_state("RO_EG_SU_DO",0);
    }
  }
  i=6;
  if (value_c[i]==1){
    //entriegeln
    write_state("RO_EG_SU",0);
    //aktivieren
    write_state("RO_EG_SU_ON",1);
    write_state("RO_EG_SU_DO",1);
  }
  if (value_c[i]==-1){
    //verriegeln
    if (time_c_pos[i]+1000>time_c_neg[i]){
      write_state("RO_EG_SU",-1);
    }
    else{
      write_state("RO_EG_SU_ON",0);
      write_state("RO_EG_SU_DO",0);
    }
  }
  //14  ez, west, unten
  //9 ", rollo
  //11  "
  i=14;
  if (value_c[i]==1){
    toggle_state("LI_EG_AW");
  }
  i=9;
  if (value_c[i]==1){
    //entriegeln
    write_state("RO_EG_WE",0);
    //aktivieren
    write_state("RO_EG_WE_ON",1);
    write_state("RO_EG_WE_DO",0);
  }
  if (value_c[i]==-1){
    //verriegeln auf auf
    if (time_c_pos[i]+1000>time_c_neg[i]){
      write_state("RO_EG_WE",1);
    }
    else{
      write_state("RO_EG_WE_ON",0);
      write_state("RO_EG_WE_DO",0);
    }
  }
  i=11;
  if (value_c[i]==1){
    //entriegeln
    write_state("RO_EG_WE",0);
    //aktivieren
    write_state("RO_EG_WE_DO",1);
    write_state("RO_EG_WE_ON",1);
  }
  if (value_c[i]==-1){
    //verriegeln
    if (time_c_pos[i]+1000>time_c_neg[i]){
      write_state("RO_EG_WE",-1);
    }
    else{
      write_state("RO_EG_WE_ON",0);
      write_state("RO_EG_WE_DO",0);
    }
  }
   
  //10  küche
  i=12;
  if (value_c[i]==1){
    toggle_state("LI_EG_KU_L1");
    write_state("LI_EG_SP", address_to_value("LI_EG_KU_L1"));
  }
      
  //7 kellertreppe unten
  //5 kellertreppe oben
  i=7;
  if (value_c[i]==1){
    write_state("LI_UG_GA",3);
    //write_state("LI_GA_L1",3);
  }
  i=5;
  if (value_c[i]==1){
    toggle_state("LI_EG_GA");
    //write_state("LI_GA_L1",3);
  }
}
  
  
  
