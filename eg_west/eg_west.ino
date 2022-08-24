//this nodes name
const char* unit_name   = "eg_west";
const char* password    = "pass";

  
//for tcp communication
//watch out for the pins needed for the ethernet schield (always 10, 11 12 13 on uno, 50 51 52 53 on mega!)
#include <Ethernet.h>
const byte mac[]                = {0xDE, 0xAA, 0x7E, 0xE1, 0x1E, 0x17 };
const IPAddress                 ip(192,168,178,218);
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
 // 0 sü hoch
 // 1 we hoch
 // 2 sü runter
 // 3 sü unten
 // 4 ez hoch
 // 5 gang keller oben
 // 6 " unten
 // 7 
 // 8 ez runter
 // 9 ez unten
// 10 gang ez
// 11 ez mitte
// 12 
// 13 küche
// 14 we unten
// 15 we runter
const int num_c_states  = 16;
const int which_b[]     = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};       //a state auf der selben unit                         
int value_c[]           = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};           //positive flanke
int aux_value_c[]       = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};          //negative flanke
long time_c_neg[]       = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};           //zeit der letzen fallenden flanke
long time_c_pos[]       = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};        //zeit der letzen steigenden flanke
       
  
//constants and variables for t states (temperatur über dht22 an digitalem pin)
#define DHTTYPE DHT22
const int num_t_states      = 4;
const long period_t         = 600000;                                                                                  //update periode in ms
const String t_address[]    = {"TI_EG_GA", "TI_EG_KU", "TI_EG_WZ", "TI_EG_EZ"};                                     
const int t_pin[]           = {58, 59, 60, 61};
int value_t[]               = {0,  0,  0,  0};                                            //temperatur
int aux_value_t[]           = {0,  0,  0,  0};                                           //feuchtigkeit
long s_time_t               = 0;                                                                                          //update timer
int i_t                     = 0;                                                                                        //cycle_counter
  

//constants and variables for h states (feuchtigkeit über zustand t über dht22 an digitalem pin)
const int num_h_states      = 4;
const String h_address[]    = {"HI_EG_GA", "HI_EG_KU", "HI_EG_WZ", "HI_EG_EZ"};     //addresse
int value_h[]               = {0,  0,  0,  0};    

  
//constants and variables for l states (einfaches licht / verbraucher)
const int num_l_states      = 16;
const String l_address[]    = {"LI_EG_EZ_KU", "LI_EG_SP", "LI_EG_AS", "LI_EG_KU_L1", "LI_EG_EZ_L1", "LI_EG_AO", "LI_EG_EZ", "LI_EG_AW", "RO_EG_WE_ON", "LI_GA_L1", "RO_EG_SU_ON", "RO_EG_SU_DO", "LI_EG_EZ_KU", "LI_EG_EZ_L3", "RO_EG_WE_DO", "LI_EG_GA"};       //addresse, zum gleichschalten selbe addresse vergeben
const int l_pin[]           = {34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49};                //digitaler pin
const bool l_inv[]          = {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1};                //digitaler pin
int value_l[]               = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};    
long set_time_l[]           = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};    

//"", "RO_EG_WE_DO", "RO_EG_WE_ON", "RO_EG_SU_ON", "", "", "", "RO_EG_SU_DO", "", "", "", "", "", "", "", "", "L38", "L39","L40", "L41", "L42", "L43", "L44", "L45", "L46", "L47"};       //addresse, zum gleichschalten selbe addresse vergeben

  

////constants and variables for r states (rollo)
// über an/aus und richtungsrelais gesteuert
const int num_r_states=2;
const String r_address[]	= {"RO_EG_SU", "RO_EG_WE"};       //addresse
const String r_on_off[]		= {"RO_EG_SU_ON", "RO_EG_WE_ON"};         //l state
const String r_up_down[] 	= {"RO_EG_SU_DO", "RO_EG_WE_DO"};         //l state
const long up_time_r[] 	 	= {31000, 31000};       // zeit zum öffnen in ms
const long down_time_r[]	= {31000, 31000};       // zeit zum schließen in ms
int value_r[]				= {50, 50};          // -1 zu und verriegelt, 0 entriegelt, 1 auf und verriegelt
int aux_value_r[]           = {50, 50};          // -1 zu und verriegelt, 0 entriegelt, 1 auf und verriegelt
long stop_time_r[]          = {0, 0};          // zeit zu stoppen
boolean stop_pending_r[]    = {false,   false};


////constants and variables for s states (dachfenster)
// üer eingänge auf und ab gesteuert
const int num_s_states      = 0;
const String s_address[]    = {"DF_00", "DF_01"};       //addresse
const String s_up[]         = {"LI_A0", "LI_A2"};         //l state
const String s_down[]       = {"LI_A1", "LI_A3"};          //l state
const int up_time_s[]       = {10, 10};       // zeit zum öffnen in ms
const int down_time_s[]     = {200, 200};       // zeit zum schließen in ms
int value_s[]               = {-1, -1};          // -1 zu und verriegelt, 0 entriegelt, 1 auf und verriegelt
int aux_value_s[]           = {-1, -1};          // -1 zu und verriegelt, 0 entriegelt, 1 auf und verriegelt
long stop_time_s[]          = {0,  0};          // zeit zu stoppen
boolean stop_pending_s[]    = {false,   false};

  
  
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


void user_logic(){
  int i;
    
  //taster verküpfungen
  //10  gang, ez
  i=10;
  if (value_c[i]==1){
     toggle_state_ext("LI_EG_GA");
  }
  //9, ez, gang, unten
  //11  ", mitte
  //4  ez, gang, rollo
  //8  "
  i=9;
  if (value_c[i]==1){
     toggle_state("LI_EG_EZ_KU");
  }
  i=11;
  if (value_c[i]==-1){
    //verriegeln auf auf
    if (time_c_pos[i]+700>time_c_neg[i]){
      toggle_state("LI_EG_EZ");
    }
    else{
      toggle_state("LI_EG_EZ_L1");
    }
  }
  i=4;
  if (value_c[i]==1){
    write_state("RO_EG_SU",0);
    write_state("RO_EG_WE",0);
  }
  i=8;
  if (value_c[i]==1){
    write_state("RO_EG_SU",100);
    write_state("RO_EG_WE",100);
  }



  
  //3  ez, süd, unten
  //0  ez, süd, rollo
  //2  "
  i=3;
  if (value_c[i]==-1){
    if (time_c_pos[i]+700>time_c_neg[i]){
      toggle_state("LI_EG_AS");
    }
    else{
      toggle_state_ext("PUMP");
    }
  }
  i=0;
  if (value_c[i]==1){
    write_state("RO_EG_SU",0);
  }
  //negative flanke, wenn weniger als 1 sekunde nach positiver dann wird verriegelt
  if (value_c[i]==-1){
    if (time_c_pos[i]+1000<time_c_neg[i]){
      write_state("RO_EG_SU",50);
    }
  }
  i=2;
  if (value_c[i]==1){
    write_state("RO_EG_SU",100);
  }
  if (value_c[i]==-1){
    //verriegeln
    if (time_c_pos[i]+1000<time_c_neg[i]){
      write_state("RO_EG_SU",50);
    }
  }
  //14  ez, west, unten
  //1 ", rollo
  //15  "
  i=14;
  if (value_c[i]==1){
    toggle_state("LI_EG_AW");
  }
  i=1;
  if (value_c[i]==1){
    write_state("RO_EG_WE",0);
  }
  if (value_c[i]==-1){
    if (time_c_pos[i]+1000<time_c_neg[i]){
      write_state("RO_EG_WE",50);
    }
  }
  i=15;
  if (value_c[i]==1){
    //entriegeln
    write_state("RO_EG_WE",100);
  }
  if (value_c[i]==-1){
    //verriegeln
    if (time_c_pos[i]+1000<time_c_neg[i]){
      write_state("RO_EG_WE",50);
    }
  }
   
  //13  küche
  i=13;
  if (value_c[i]==1){
    toggle_state("LI_EG_KU_L1");
  }
      
  //5 kellertreppe unten
  //6 kellertreppe oben
  i=6;
  if (value_c[i]==1){
    send_command("LI_UG_GA",3);
  }
  i=5;
  if (value_c[i]==1){
    toggle_state_ext("LI_EG_GA");
  }
}
  
  
  
