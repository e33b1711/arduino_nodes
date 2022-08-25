//this nodes name
const char* unit_name   = "au";
const char* password    = "pass";
extern const char *ssid;
extern const char *pass;

//for tcp communication (node mcu)
#include <WiFi.h>

const IPAddress                 server(192,168,178,222);
const int port                  = 8888;





//constants and variables for b states (einer der 2 R-codierte Schalter an einem analogen Eingang)
int counter_a           = 0;
const int num_b_states  = 0;
const int b_pin[]       = {14, 27, 26, 25};       //a state auf der selben unit                         
int value_b[]           = {0,  0,  0,  0};       //an/aus
int prev_value_b[]      = {0,  0,  0,  0};       //an/aus (alter Wert zur Flankenerkennung)


//constants and variables for c states (flanke eines b states)
//0     taster innen
//1     garagentor down
//2     garagentor up
const int num_c_states  = 0;
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
const String t_address[]    = {"TI_AU"};                                                                                                           //addresse
long s_time_t               = 0;                                                                                          //update timer
int i_t                     = 0;                                                                                              //cycle_counter
  

//constants and variables for h states (feuchtigkeit über zustand t über dht22 an digitalem pin)
const int num_h_states=1;
const String h_address[]={"HI_AU"};       //addresse
int value_h[]={0};
  
//constants and variables for l states (einfaches licht / verbraucher)
/**
 * 0 innen
 * 1 aussen
 * ...
 */
const int num_l_states      = 0;
const String l_address[]    = {"LI_GR", "LI_GR", "LI_GR_L1", "LI_GR_L2", "LI_GR_L3", "LI_GR_L4", "DO_GR_UP", "DO_GR_DO", "ZE_GR_0", "ZE_GR_1", "ZE_GR_2"};       //addresse, zum gleichschalten selbe addresse vergeben
const int l_pin[]           = {0,  4, 21,  16, 17, 5,  18, 19, -1, -1, -1};                //digitaler pin
const bool l_inv[]          = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};                //digitaler pin
int value_l[]               = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};
long set_time_l[]           = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};



////constants and variables for r states (garage door)
// über an/aus und richtungsrelais gesteuert
const int num_r_states            = 0;
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
 //a total waste
  
    
}
  
  
  
