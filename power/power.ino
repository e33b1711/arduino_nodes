
//unit's stuff
const char* unit_name  = "power";
const char* password   = "pass";

//for tcp communication
//watch out for the pins needed for the ethernet schield (always 10, 11 12 13 on uno, 50 51 52 53 on mega!)
#include <Ethernet.h>
const byte mac[]                = {0x4E, 0xAB, 0x7E, 0xEF, 0xFE, 0x04 };
const IPAddress                 ip(192,168,178,213);
const IPAddress                 server(192,168,178,23);
const IPAddress                 gateway(192,168,178,1);
const IPAddress                 subnet(255,255,255,0);
const int port                  = 8888;
const int ethernet_sc_pin       = 53;
const int ethernet_reset_pin    = 12;


//constants and variables for t states (temperatur über dht22 an digitalem pin)
#define DHTTYPE DHT22
const int num_t_states      = 2;
const long period_t         = 100000;                                            //update periode in ms
const String t_address[]    = {"TI_PU_O", "TI_PU_U"};                                    
int value_t[]               = {0,  0};                                            //temperatur
long lastUpdateDS_t;
const long updatePeriodDS_t = 100000;


//constants and variables for f states (digitaler eingang)
const int num_f_states      = 2;                          
const int f_pin[]           = {57, 2};
const bool f_inv[]           = {true, false};
const String f_address[]    = {"F_HE", "F_WW"};                                    
int f_value[]               = {-1,  -1};                  //alter wert



//constants and variables for p states (pwm output)
const int num_p_states      = 1;
const String p_address[]    = {"U_EL"};
const int p_pin[]           = {5};                //digitaler pin
unsigned short value_p[]    = {0};    
long set_time_p[]           = {0};    
unsigned short  max_p[]     = {210};

//TODO: wheres the water warning?

void user_logic()
{

    //repeat water warning if 1
    const long ww_interval_c = 3600000; //1 hour
    static long ww_time =0;
    if ( (ww_time+ww_interval_c) < millis() and (address_to_value("F_WW")!=0)){
        send_state("F_WW", 1);
        ww_time = millis();
    }

    //timeout for elo heating
    const long heat_timeout_c = 30000;
    if ( (set_time_p[0]+heat_timeout_c) < millis() and (address_to_value("U_EL")!=0)){
        write_p("U_EL", 0);
        send_state(String(unit_name) + " heat timeout", 0);
    }
    


    //overtemp for elo heating, 85°
    const int max_temp_c = 900;
    const int thresh_temp_c = 850;
    const int normal_max_p = 210;
    const int temp_coeff = 40;
    int temp = address_to_value("TI_PU_O");
    
    if ( temp < thresh_temp_c ){
        max_p[0] = normal_max_p;
    }
    if (( temp > thresh_temp_c ) and ( temp < max_temp_c )){
        max_p[0] = (max_temp_c-temp) * temp_coeff;
    }
    if ( temp > max_temp_c ){
        max_p[0] = 0;
    }

    

}
