void setup() {

  setup_debug();
  setup_modbus();
  setup_server();
  setup_s0();
  setup_heating();
  
}

//TO DO: count energy
//TO DO: over heat counter measures


void loop() {

  update_heating();
  handle_server();
  update_s0();
  handle_debug();

}
