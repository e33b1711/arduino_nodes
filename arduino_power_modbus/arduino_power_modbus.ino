/*TODO
Status:
modbus Error (no response)
Heating Error (no powerHeat when pwm)
s0 Error (balPower>0 and powerImport=0, balPower<0 and powerPV=0)
no ntp time 
=> non-persistent status
=> persistent status (later resetable)
=> check all dependencies of status <> functions

Control:
=> set target power / control mode over serial (done)
=> set target power / control mode over mqtt
=> reset status over mqtt
=> reset status over debug
=> direct mode (control off) (done)

write long time statistics, WIP, new day!!

lower cutoff power heating / how to prevent toggling?

*/


//unit's stuff
const char* unit_name  = "power";
const char* password   = "pass";
const char* vers       = "v0.11";


//heat
extern int pwm_setpoint;
extern unsigned long watchdog_counter;
extern void heat_off();




//S0
extern float powerUtility, powerHeat, powerPV;
float energyImport();
float energyHeat();
float energyPV();
extern long pulseCount0;
extern long pulseCount1;
extern long pulseCount2;
extern float energyExport,lastEnergyExport;
extern float unsalEnergyImportZero;
extern float unsalEnergyExportZero;


//temp
extern float tempHigh, tempLow;


void setup() {
  setup_debug();
  setup_server();
  setup_s0();
  setup_heating();
  setup_temp();
  watchdogSetup();
}

void watchdogSetup(void){
  cli(); // disable all interrupts
  asm("WDR");
  WDTCSR |= (1<<WDCE) | (1<<WDE);
  WDTCSR =  (1<<WDIE) | (1<<WDE) | (1<<WDP3)| (1<<WDP0); // 8s / no interrupt, system reset
  sei();
}

void watchdogOff(void){
  cli(); // disable all interrupts
  asm("WDR");
  WDTCSR |= (0<<WDCE) | (0<<WDE);
  WDTCSR =  (0<<WDIE) | (0<<WDE) | (0<<WDP3)| (0<<WDP0); // 8s / no interrupt, system reset
  sei();
}

ISR(WDT_vect){
  heat_off();
  Serial.println(">>>>>>>>>>>>>>>>>>>>>HW Watchdog Interrupt<<<<<<<<<<<<<<<<<<<");
}


void loop() {
  asm("WDR");
  handle_server();
  asm("WDR");
  update_heating();
  update_s0();
  handle_debug();
  handle_temp();
}
