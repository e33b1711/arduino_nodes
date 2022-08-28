//this nodes name
const char* unit_name   = "gr";
const char* password    = "pass";
extern const char *ssid;
extern const char *pass;

//for tcp communication (node mcu)
#include <WiFi.h>

const IPAddress                 server(192,168,178,23);
const int port                  = 8888;





//constants and variables for b states (einer der 2 R-codierte Schalter an einem analogen Eingang)
int counter_a           = 0;
const int num_b_states  = 4;
const int b_pin[]       = {14, 27, 26, 25};       //a state auf der selben unit                         
int value_b[]           = {0,  0,  0,  0};       //an/aus
int prev_value_b[]      = {0,  0,  0,  0};       //an/aus (alter Wert zur Flankenerkennung)


//constants and variables for c states (flanke eines b states)
//0     taster innen
//1     garagentor down
//2     garagentor up
const int num_c_states  = 4;
const int which_b[]     = {0,  1,  2,  0};        //a state auf der selben unit                         
int value_c[]           = {0,  0,  0,  0};        //positive flanke
int aux_value_c[]       = {0,  0,  0,  0};        //negative flanke
long time_c_neg[]       = {0,  0,  0,  0};        //zeit der letzen fallenden flanke
long time_c_pos[]       = {0,  0,  0,  0};        //zeit der letzen steigenden flanke


//constants and variables for t states (temperatur über dht22 an digitalem pin)
#define DHTTYPE DHT22
#define DHTPIN1 22
#define DHTPIN2 23
const int num_t_states      = 1;
const long period_t         = 600000;                                                                                  //update periode in ms
const String t_address[]    = {"TI_GR"};                                                                                                           //addresse
long s_time_t               = 0;                                                                                          //update timer
int i_t                     = 0;                                                                                              //cycle_counter
  

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
const int num_l_states      = 11;
const String l_address[]    = {"LI_GR", "LI_GR", "LI_GR_L1", "LI_GR_L2", "LI_GR_L3", "LI_GR_L4", "DO_GR_UP", "DO_GR_DO", "ZE_GR_0", "ZE_GR_1", "ZE_GR_2"};       //addresse, zum gleichschalten selbe addresse vergeben
const int l_pin[]           = {0,  4, 21,  16, 17, 5,  18, 19, -1, -1, -1};                //digitaler pin
const bool l_inv[]          = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};                //digitaler pin
int value_l[]               = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};
long set_time_l[]           = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};



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
boolean locked_r[]                = {false};       
const long lock_delay_r           = 20000;      
  

////constants and variables for s states (dachfenster)
// üer eingänge auf und ab gesteuert
const int num_s_states          = 0;
const String s_address[]        = {};       //addresse
const String s_up[]             = {};         //l state
const String s_down[]           = {};         //l state
const int up_time_s[]           = {};       // zeit zum öffnen in ms
const int down_time_s[]         = {};       // zeit zum schließen in ms
int value_s[]                   = {};          // -1 zu und verriegelt, 0 entriegelt, 1 auf und verriegelt
int aux_value_s[]               = {};          // -1 zu und verriegelt, 0 entriegelt, 1 auf und verriegelt
bool stop_pending_s[]           = {};
long stop_time_s[]              = {};          // zeit zu stoppen

  
  
//constants and variables for u states (temperatur steller)
const int num_u_states          = 0;
const String u_address[]        = {};        //addresse
const int u_pin[]               = {};               //pwm-pin  
const unsigned long u_interval  = 240000;          //pwm periode /16 in milisekunden
unsigned long previousMillis    = 0;
int u_phase                     = 0;
int value_u[]                   = {};               //stell wert 0-15 (0=aus bis 15=voll)

  



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
  i=8;
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
   i=9;
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
  i=10;
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
  
  
  
