//this nodes name
const char* unit_name   = "gr";
const char* password    = "pass";


//for tcp communication (node mcu)
//watch out for the pins needed for the ethernet schield (always 10, 11 12 13 on uno, 50 51 52 53 on mega!)
#include <ESP8266WiFi.h>
const char *ssid                = "my_ssid"; // your network SSID (name)
const char *pass                = "my_pass";  // your network password
const IPAddress                 server(192,168,178,222);
const int port                  = 8888;





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
const long period_t=1800000;                                                                                  //update periode in ms
const String t_address[]={"TI_GR", "TI_GR_A"};                                                                                                           //addresse
const int t_pin[]={ 22, 24};
int value_t[]={ 0,   0};                                            //temperatur
int aux_value_t[]={  0,   0};                                            //feuchtigkeit
long s_time_t=0;                                                                                          //update timer
int i_t=0;                                                                                              //cycle_counter
  

//constants and variables for h states (feuchtigkeit über zustand t über dht22 an digitalem pin)
const int num_h_states=2;
const String h_address[]={"HI_GR", "HI_GR_A"};       //addresse
int value_h[]={0, 0};

  
//constants and variables for l states (einfaches licht / verbraucher)
/**
 * 0 innen
 * 1 aussen
 * ...
 */
const int num_l_states      = 8;
const String l_address[]    = {"LI_GR", "LI_GR_L1", "LI_GR", "ZE_GR_0", "ZE_GR_1", "ZE_GR_2", "DO_GR_DO", "DO_GR_UP" };       //addresse, zum gleichschalten selbe addresse vergeben
const int l_pin[]           = {23, 25, 27, 29, 31, 33, 35, 37};                //digitaler pin
const bool l_inv[]          = {0,  0,  0,  0,  0,  0,  0,  0};                //digitaler pin
int value_l[]               = {0,  0,  0,  0,  0,  0,  0,  0};
long set_time_l[]           = {0,  0,  0,  0,  0,  0,  0,  0};



////constants and variables for r states (garage door)
// über an/aus und richtungsrelais gesteuert
const int num_r_states            = 1;
const String r_address[]          = {"DO_GR"};       //addresse
const String r_trigger_up[]       = {"DO_GR_UP"};         //l state
const String r_trigger_do[]       = {"DO_GR_DO"};         //l state
const int r_down[]                = {1};         // c state up sensor
const int r_up[]                  = {2};       // c state down sensor
int value_r[]                     = {0};          // -1 z, 0 unsicher, 1 auf, 2 fehler
int aux_value_r[]                 = {0};          // -1 z, 0 unsicher, 1 auf, 2 fehler
long lock_time_r[]                = {0};          
const long lock_delay_r           = 20000;      
  

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
  //0 taster innen =< timer 9
  i=0;
  if(value_c[i]==1) toggle_state("ZE_GR_0");

  //garagen tor -1 => 0 timer 1
  static int prev_value = address_to_value("DO_GR");
  if ( (prev_value==-1) & (prev_value != address_to_value("DO_GR")) ){
    write_state("ZE_GR_1",1);
    //write_state("ZE_GR_2",1);
    //write_state("ZE_EG_VH",1);
    Serial.println("User logic: trigger einfahrt licht.");
  }
  prev_value = address_to_value("DO_GR");

  //timer innenbeleuchtung lang (taster)
  i=3;
  static boolean timer_0_on=false;
  //start
  if ((address_to_value("ZE_GR_0")==1) & (timer_0_on==false)){
    timer_0_on=true;
    write_state("LI_GR",1);
    Serial.println("user_logic: timer 0  an");
  }
  //timer: running
  if ((address_to_value("ZE_GR_0")==1) & (timer_0_on==true)){
    if ((set_time_l[i]+600000)<millis()){
     if (address_to_value("ZE_GR_1")==0) write_state("LI_GR",0);
      write_state("ZE_GR_0",0);
      timer_0_on=false;
      Serial.println("user_logic: timer 0 abgelaufen");
    }
  }
  //timer: external off
   if ((address_to_value("ZE_GR_0")==0) & (timer_0_on==true)){
     timer_0_on=false;
     if (address_to_value("ZE_GR_1")==0) write_state("LI_GR",0);
     Serial.println("user_logic: timer 0 extern abgebrochen.");
  }


  //timer innenbeleuchtung kurz (einfahrt)
   i=4;
  static boolean timer_1_on=false;
  //start
  if ((address_to_value("ZE_GR_1")==1) & (timer_1_on==false)){
    timer_1_on=true;
    write_state("LI_GR",1);
    Serial.println("user_logic: timer 1  an");
  }
  //timer: running
  if ((address_to_value("ZE_GR_1")==1) & (timer_1_on==true)){
    if ((set_time_l[i]+180000)<millis()){
      if (address_to_value("ZE_GR_0")==0) write_state("LI_GR",0);
      write_state("ZE_GR_1",0);
      timer_1_on=false;
      Serial.println("user_logic: timer 1 abgelaufen.");
    }
  }
  //timer: external off
   if ((address_to_value("ZE_GR_1")==0) & (timer_1_on==true)){
     timer_1_on=false;
     if (address_to_value("ZE_GR_0")==0) write_state("LI_GR",0);
     Serial.println("user_logic: timer 1 extern abgebrochen.");
  }


  //timer außen, mit vordach timer auslösen
  i=5;
  static boolean timer_2_on=false;
  //start
  if ((address_to_value("ZE_GR_2")==1) & (timer_2_on==false)){
    timer_2_on=true;
    write_state("LI_GR_L1",1);
    Serial.println("user_logic: timer 2  an");
  }
  //timer: running
  if ((address_to_value("ZE_GR_2")==1) & (timer_2_on==true)){
    if ((set_time_l[i]+180000)<millis()){
      write_state("LI_GR_L1",0);
      write_state("ZE_GR_2",0);
      timer_2_on=false;
      Serial.println("user_logic: timer 2 abgelaufen.");
    }
  }
  //timer: external off
   if ((address_to_value("ZE_GR_2")==0) & (timer_2_on==true)){
     timer_2_on=false;
     write_state("LI_GR_L1",0);
     Serial.println("user_logic: timer 2 extern abgebrochen.");
  }
  
    
}
  
  
  
