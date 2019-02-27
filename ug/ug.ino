const String unit_name = "ug";

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
//0
//1
//2
//3 la
//4
//5 wk
//6 ho
//7 
//8 hk
//9 gang
//10 
//11
//12 
//13
//14
//15 

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
const int num_t_states=6;
const int period_t=10000;                                                                                  //update periode in ms
const String t_address[]={      "TI_UG_WK", "TI_UG_HN", "TI_UG_LA", "TI_UG_HS", "TI_UG_HK", "TI_UG_GA"};       
                                                                                                        //addresse
const int t_pin[]={             40,  41,  42,  43,  44, 45};
int value_t[]={                 0,   0,   0,   0,   0,   0};                                            //temperatur
int aux_value_t[]={             0,   0,   0,   0,   0,   0};                                            //feuchtigkeit
long time_t=0;                                                                                          //update timer
int i_t=0;                                                                                              //cycle_counter

//constants and variables for h states (feuchtigkeit über zustand t über dht22 an digitalem pin)
const int num_h_states=6;
const String h_address[]={      "HI_UG_WK", "HI_UG_HN", "HI_UG_LA", "HI_UG_HS", "HI_UG_HK", "HI_UG_GA"};       //addresse
int value_h[]={                 0, 0, 0, 0, 0, 0};

//constants and variables for l states (einfaches licht / verbraucher)
const int num_l_states=18;
const String l_address[]={      "BELL", "LI_UG_WK", "LI_UG_HO", "LI_UG_GA", "PUMP", "LI_UG_LA", "LI_GA_L1", "LI_UG_HK", "L36", "L37", "L38", "L39","L40", "L41", "L42", "L43", "L44", "L45", "L46", "L47"};       //addresse, zum gleichschalten selbe addresse vergeben
const int l_pin[]={             22,23,24,25,26,27,28,29,36,37,38,39,40,41,42,43,44,45};                //digitaler pin
int value_l[]={                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
long set_time[]={               0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

////constants and variables for r states (rollo)
// über an/aus und richtungsrelais gesteuert
const int num_r_states=0;
const String r_address[]={      "RO_EG_SU", "RO_EG_WE"};       //addresse
const String r_on_off[]={       "RO_EG_SU_ON", "RO_EG_WE_ON"};         //l state
const String r_up_down[]={      "RO_EG_SU_DO", "RO_EG_WE_DO"};         //l state
const int up_time_r[]={            25, 25};       // zeit zum öffnen in s
const int down_time_r[]={          22, 22};       // zeit zum schließen in s
int value_r[]={                  0, 0};          // -1 zu und verriegelt, 0 entriegelt, 1 auf und verriegelt
long stop_time_r[]={              0, 0};          // zeit zu stoppen

////constants and variables for s states (dachfenster)
// üer eingänge auf und ab gesteuert
const int num_s_states=0;
const String s_address[]={      "DF_OG_KN"};       //addresse
const String s_up[]={           "DF_OG_KN_UP"};         //l state
const String s_down[]={         "DF_OG_KN_UP_DO"};         //l state
const int up_time_s[]={         10};       // zeit zum öffnen in ms
const int down_time_s[]={       200};       // zeit zum schließen in ms
int value_s[]={                  -1};          // -1 zu und verriegelt, 0 entriegelt, 1 auf und verriegelt
long stop_time_s[]={              0};          // zeit zu stoppen




//constants and variables for u states (temperatur steller)
//                              0                1                  2                 3              4                5
//                              hobby1           hobby2             hobby3            gang          lager             waschküche
const int num_u_states=6;
const String u_address[]={      "U_UG_H1",       "U_UG_H2",         "U_UG_H3",        "U_UG_GA",     "U_UG_LA",       "U_UG_WK"};        //addresse
const int u_pin[]={             30,              31,                32,               33,             34,             35};               //pwm-pin  
const unsigned long u_interval= 240000;          //pwm periode /16 in milisekunden
unsigned long previousMillis=0;
int u_phase=0;
int value_u[]={                   5,               5,                 5,                 1,            1,                14};               //stell wert 0-15 (0=aus bis 15=voll)







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
  //8 HK
  i=8;
    if (value_c[i]==-1){
      //verriegeln auf auf
       if (time_c_pos[i]+700>time_c_neg[i]){
        write_state("LI_UG_HK",3);
       }
       else{
         write_state("PUMP",3);
       }
    }
  //9 Gang
  i=9;
  if (value_c[i]==1){
     write_state("LI_UG_GA",3);
    
  }
  //6 Hobby
  i=6;
  if (value_c[i]==1){
     write_state("LI_UG_HO",3);
  }
  //3 Lager
  i=3;
  if (value_c[i]==1){
     write_state("LI_UG_LA",3);
  }
  //5 Waschküche
  i=5;
  if (value_c[i]==1){
     write_state("LI_UG_WK",3);
  }
}




