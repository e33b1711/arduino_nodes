//watchdog
extern void setup_watchdog(void);
extern void watchdog_off(void);
extern void handle_watchdog();

void setup(){

   //init debug interface
  init_debug();

  // init states
  setup_watchdog();
  init_comm();
  setup_t();
  setup_p();  
  setup_f();

  //init comm
  

}

void loop()
{

  //watchdog
  handle_watchdog();

  //react to messages
  handle_comm();
  
  update_t();
  

  //debug Interface
  handle_debug();

  user_logic();

  update_p();
  update_f();

}
