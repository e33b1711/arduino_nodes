//watchdog
extern void setup_watchdog(void);
extern void watchdog_off(void);
extern void handle_watchdog();

void setup(){

   //init debug interface
  init_debug();

  // init states
  setup_watchdog();
  setup_p();
  setup_t();
 

  //init comm
  init_comm();

}

void loop()
{

  //watchdog
  handle_watchdog();

  //react to messages
  handle_comm();

  //update states
  //user logic is called from update_a_b_c
 
  update_p();
  update_t();
  //update_a_b_c();

  //debug Interface
  handle_debug();

}
