const char* unit_name  = "power";
const char* password   = "pass";
const char* vers       = "v0.94";

extern float bal_power;
extern bool bal_power_valid;
extern int pwm_setpoint;
extern long watchdog_counter;

extern float sdm_data[];
extern bool sdm_data_valid[];

extern int powerUtility;    
extern int errorUtility;
extern int powerHeating;
extern bool errorHeating;
extern int powerPV;
extern bool errorPV;

//ntp / time related
extern unsigned long  epoch_at_millis0;
extern void update_time();
extern void new_day_S0();

//TODO: ntp
//ntp: 
//=>epoch at startup
//=>resync at 00:00
//energy counter, letzter tag / aktuell:
//netzbezug
//pv erzeugt
//heizung verbraucht
//verschenkt (energie-menge über modbus - bezug)??

void setup() {

  setup_debug();
  setup_modbus();
  setup_server();
  setup_s0();
  setup_heating();
  
}

//TODO: count energy today, ntp protocol?
//TODO: detect if heating is off / overheated
//TODO: direct mode over serial


void loop() {

  update_heating();
  handle_server();
  update_s0();
  handle_debug();
  handle_modbus();

}
