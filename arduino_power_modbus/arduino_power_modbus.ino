//for tcp communication
//watch out for the pins needed for the ethernet schield (always 10, 11 12 13 on uno, 50 51 52 53 on mega!)
#include <Ethernet.h>
const byte mac[]                = {0xDE, 0xAA, 0x7E, 0xE1, 0x1E, 0x15 };
const IPAddress                 ip(192,168,178,213);
const IPAddress                 gateway(192,168,178,1);
const IPAddress                 subnet(255,255,255,0);
const int ethernet_sc_pin       = 53;
const int ethernet_reset_pin    = 12;


//unit's stuff
const char* unit_name  = "power";
const char* password   = "pass";
const char* vers       = "v0.11";


//heat
extern int pwm_setpoint;
extern unsigned long watchdog_counter;
extern void heat_off();

//temp
extern float tempHigh, tempLow;


void setup() {
  setup_debug();
  init_comm();
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
  handle_comm();
  asm("WDR");
  update_heating();
  handle_debug();
  handle_temp();
}
