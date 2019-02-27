const String unit_name = "og_ost";

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
//0 sz rollo runter
//1 bad df ho
//2 bad li
//3 gang bad
//4 bewegung
//5 bad df runter
//6 sz oben
//7sz unten
//8 sz anselm
//9 gang sz
//10 sz rollo hoch
//11 sz melli
//12 sz df
//13  sz df
//14 bad rollo ho
//15 bad rollo ru

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
const int num_t_states=2;
const int period_t=20000;                                                                                  //update periode in s
const String t_address[]={      "TI_OG_SZ", "TI_OG_BA", "TI_3", "TI_4", "TI_5", "TI_6"};       
                                                                                                        //addresse
const int t_pin[]={             47,  49,  48,  49,  50, 51};
int value_t[]={                 0,   0,   0,   0,   0,   0};                                            //temperatur
int aux_value_t[]={             0,   0,   0,   0,   0,   0};                                            //feuchtigkeit
long time_t=0;                                                                                          //update timer
int i_t=0;                                                                                              //cycle_counter


//constants and variables for h states (feuchtigkeit über zustand t über dht22 an digitalem pin)
const int num_h_states=2;
const String h_address[]={      "HI_OG_SZ", "HI_OG_BA", "HI_3", "HI_4", "HI_5", "HI_6"};       //addresse
int value_h[]={                 0, 0, 0, 0, 0, 0};


//constants and variables for l states (einfaches licht / verbraucher)
const int num_l_states=18;
const String l_address[]={      "VD_OG_SZ_UP", "LI_OG_BA", "VD_OG_SZ_DO", "L25", "DF_OG_SZ_UP", "L27", "DF_OG_SZ_DO", "L29", "LI_OG_SZ_L2", "RO_OG_SZ_DO", "RO_OG_SZ_ON", "ST_OG_BA", "RO_OG_BA_DO", "LI_OG_SZ", "LI_OG_SZ_L1", "RO_OG_BA_ON", "L44", "L45", "L46", "L47"};       //addresse, zum gleichschalten selbe addresse vergeben
const int l_pin[]={             22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,44,45};                //digitaler pin
int value_l[]={                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
long set_time_l[]={
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

////constants and variables for r states (rollo)
// über an/aus und richtungsrelais gesteuert
const int num_r_states=2;
const String r_address[]={      "RO_OG_SZ", "RO_OG_BA"};       //addresse
const String r_on_off[]={       "RO_OG_SZ_ON", "RO_OG_BA_ON"};         //l state
const String r_up_down[]={      "RO_OG_SZ_DO", "RO_OG_BA_DO"};         //l state
const int up_time_r[]={            31, 31};       // zeit zum öffnen in s
const int down_time_r[]={          31, 31};       // zeit zum schließen in s
int value_r[]={                  0, 0};          // -1 zu und verriegelt, 0 entriegelt, 1 auf und verriegelt
long stop_time_r[]={              0, 0};          // zeit zu stoppen

////constants and variables for s states (dachfenster)
// üer eingänge auf und ab gesteuert
const int num_s_states=2;
const String s_address[]={      "DF_OG_SZ", "VD_OG_SZ"};       //addresse
const String s_up[]={           "DF_OG_SZ_UP", "VD_OG_SZ_UP"};         //l state
const String s_down[]={         "DF_OG_SZ_DO", "VD_OG_SZ_DO"};         //l state
const int up_time_s[]={         500, 500};       // zeit zum öffnen in ms
const int down_time_s[]={       500, 500};       // zeit zum schließen in ms
int value_s[]={                  -1, 0};          // -1 zu und verriegelt, 0 entriegelt, 1 auf und verriegelt
long stop_time_s[]={              0, 0};          // zeit zu stoppen



//constants and variables for u states (temperatur steller)
//                              0                1                  2                 3              4                
//                              kind nord (2kr)  Kind süd (2kr)     GA                sz (2 kr)          bad (2 kr)      
const int num_u_states=5;
const String u_address[]={      "U_OG_KN",       "U_OG_KS",         "U_OG_GA",        "U_OG_SZ",     "U_OG_BA"};        //addresse
const int u_pin[]={             38,              39,                40,               41,             42};               //pwm-pin  
const unsigned long u_interval= 250000;          //pwm periode /16 in milisekunden
unsigned long previousMillis=0;
int u_phase=4;
int value_u[]={                   5,               5,                 1,                 1,            14};               //stell wert 0-15 (0=aus bis 15=voll)










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
  //6  sz, tür, mitte
  //7  sz, tür, unten
  //12  df sz
  //13  df sz
  i=6;
   if (value_c[i]==-1){
    //verriegeln auf auf
     if (time_c_pos[i]+700>time_c_neg[i]){
      write_state("LI_OG_SZ",3);
     }
     else{
       write_state("LI_OG_SZ",0);
     write_state("LI_OG_SZ_L1",0);
     write_state("LI_OG_SZ_L2",0);
     }
  }
  i=7;
  if (value_c[i]==-1){
    //verriegeln auf auf
     if (time_c_pos[i]+700>time_c_neg[i]){
      write_state("LI_OG_SZ",3);
     }
     else{
       write_state("LI_OG_SZ",0);
     write_state("LI_OG_SZ_L1",0);
     write_state("LI_OG_SZ_L2",0);
     }
  }
  i=12;
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
  i=13;
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
  
  
  
  //10  rollo sz
  //0  rollo sz
    i=10;
 
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
  i=0;
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
  //11  bett, west
  //8  bett ost
  i=11;
  if (value_c[i]==-1){
    //verriegeln auf auf
     if (time_c_pos[i]+700>time_c_neg[i]){
      write_state("LI_OG_SZ_L1",3);
     }
     else{
       write_state("LI_OG_SZ",0);
     write_state("LI_OG_SZ_L1",0);
     write_state("LI_OG_SZ_L2",0);
     }
  }
  i=8;
  if (value_c[i]==-1){
    //verriegeln auf auf
     if (time_c_pos[i]+700>time_c_neg[i]){
      write_state("LI_OG_SZ_L2",3);
     }
     else{
       write_state("LI_OG_SZ",0);
     write_state("LI_OG_SZ_L1",0);
     write_state("LI_OG_SZ_L2",0);
     }
  }
  //9  flur, sz
  i=9;
  if (value_c[i]==-1){
     if (time_c_pos[i]+700>time_c_neg[i]){
      write_state("LI_OG_GA",3);
     }
     else{
     
      write_state("LI_OG_GA_L1",3);
     }
  }
  
  // 3  flur, bad
  i=3;
  if (value_c[i]==-1){
     if (time_c_pos[i]+700>time_c_neg[i]){
      write_state("LI_OG_GA",3);
     }
     else{
     
      write_state("LI_OG_GA_L1",3);
     }
  }
  // 2  bad, einzel
  //1  rollo bad tür
  //5  rollo bad tür
  i=2;
  if (value_c[i]==1){
     write_state("LI_OG_BA",3);
  }
   i=1;
 
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
  i=5;
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
  //14   rollo bad balkon
 //15   rollo bad balkon
   i=14;
  
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
  i=15;
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
  }
}



