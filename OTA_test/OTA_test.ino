const char* unit_name  = "ota_test";
const char* vers       = "v0.91";
 
#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoOTA.h>



void setup() {
  
  setup_debug();
  setup_ether();
  
  setup_server();
  setup_modbus();
  setup_s0();
  setup_heating();

}

void loop() {
  // check for updates
  update_ether();
  handle_server();
  update_heating();
  update_s0();
  handle_debug();
  handle_modbus();

   
  // add your normal loop code below ...
}
