#include <Ethernet.h>


const String unit_name = "gr";

//for com to tcp relay  
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x01 };
IPAddress ip(192,168,1,177);
IPAddress server(1,1,1,1);
int port = 8888;

//dht22
//#include <dht.h>
//dht DHT;

//for debug interface
String db_inputString = "";                // a string to hold incoming data
boolean db_messageComplete = false;        // whether the string is complete

//constants and variables for b states (einer der 2 R-codierte Schalter an einem analogen Eingang)
int counter_a=0;
const int num_b_states=6;
const int b_pin[]=    {2, 3, 4, 5, 6, 7};       //a state auf der selben unit                         
int value_b[]=        {0, 0, 0, 0, 0, 0};       //an/aus
int prev_value_b[]=   {0, 0, 0, 0, 0, 0};       //an/aus (alter Wert zur Flankenerkennung)

//constants and variables for c states (flanke eines b states)
const int num_c_states=6;
const int which_b[]=  {0, 1, 2, 3, 4, 5};       //a state auf der selben unit                         
int value_c[]=        {0, 0, 0, 0, 0, 0};       //positive flanke
int aux_value_c[]=    {0, 0, 0, 0, 0, 0};       //negative flanke
long time_c_neg[]=    {0, 0, 0, 0, 0, 0};       //zeit der letzen fallenden flanke
long time_c_pos[]=    {0, 0, 0, 0, 0, 0};       //zeit der letzen steigenden flanke



//constants and variables for d states (pwm-dimmer ausgang)
const int num_d_states=0;
const String d_address[]={      
  "PWM_1",      "PWM_2"};       //addresse, zum gleichschalten selbe addresse vergeben
const int d_pin[]={             
  2,               3};               //pwm-pin  
int value_d[]={                 
  0,               0};       

//constants and variables for t states (temperatur über dht22 an digitalem pin)
//                              hobby süd, hobby nord, heizkeller, lager, waschküche, gang
const int num_t_states=2;
const int period_t=309;                                       //update periode in s
const String t_address[]=       {"TI_GR", "TI_GA"};       
//addresse
const int t_pin[]=              {6,       7};                 //addresse
int value_t[]=                  {0,       0};                 //temperatur
int aux_value_t[]=              {0,       0};                 //feuchtigkeit
long time_t=0;                                                //update 
int i_t=0;                                                    //cycle_counter


//constants and variables for h states (feuchtigkeit über zustand t über dht22 an digitalem pin)
const int num_h_states=2;
const String h_address[]=     {"HI_GR", "HI_GA"};             //addresse
int value_h[]=                {0,       0};


//constants and variables for l states (einfaches licht / verbraucher)
const int num_l_states=4;
const String l_address[]=     {"LI_GR_L1", "LI_GR_L2", "LI_GR_L3", "LI_GR_L4"};       //addresse, zum gleichschalten selbe addresse vergeben
const int l_pin[]=            {8,          9,           10,        11};                //digitaler pin
int value_l[]=                {0,          0,           0,         0};
long set_time_l[]=            {0,          0,           0,         0};


////constants and variables for do state (garagen tor)




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

  // 14  gararobe
  i=14;
  if (value_c[i]==-1){
    //verriegeln auf auf
    if (time_c_pos[i]+700>time_c_neg[i]){
      write_state("LI_EG_GR",3);
    }
    else{
      write_state("LI_EG_AO",3);
    }
  }


  //Beleuchtung Timer
  static boolean timer_on=false;
  //start
  if ((address_to_value("ZE_GR")==1) & (timer_on==false)){
    timer_on=true;
    write_state("LI_GR_L1",1);
    Serial.print("timer an");
  }
  //running
  if ((address_to_value("ZE_GR")==1) & (timer_on==true)){
    if ((set_time_l[8]+180000)<millis()){
      write_state("LI_GR_L1",0);
      write_state("ZE_GR",0);
      timer_on=false;
      Serial.print("timer abgelaufen");
    }
  }
  //external off
   if ((address_to_value("ZE_GR")==0) & (timer_on==true)){
    timer_on=false;
     write_state("LI_GR_L1",0);
    Serial.print("timer extern abgebrochen. LI_GR_L1 ausgeschaltet!");
  }


}








