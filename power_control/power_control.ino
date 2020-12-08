/* step 1: sensors / base comm / time 
 * 
 * step 4: actor
 * 
 * step 5: mqtt
 * 
 * step 6: xtern logging
 * 
 * 
 */




//unit's stuff
const char* unit_name  = "power";
const char* password   = "pass";
const char* vers       = "v0.88                 ";




void setup() {

  //comm
  setup_debug();
  setup_ip();
  //setup_ntp();

  //sensors
  //setup_temp();
  //setup_modbus();
  //setup_s0();

  //house keeping
  setup_watchdog();
}



void loop() {

    //comm
    handle_debug();
    handle_ip();
    //handle_ntp();

    

    //sensors
    //handle_temp();
    //handle_modbus();
   // handle_s0();


    //house keeping
    handle_watchdog();
}
