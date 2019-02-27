void setup(){
   //init debug interface
  init_debug();
  
  //init comm
  init_comm();

  // init states
  setup_a_b_c();
  setup_t();
  setup_h();
  //states mit ausgängen zum Schluss
  setup_l();
  setup_d0();
  
}

void loop()
{
    
  //which switch which lamp
  //user logic is called from update_a_b_c
  
  //react to messages
  handle_comm();
  
   //update states
  update_a_b_c();
  update_t();
  update_h();
  //states mit ausgängen zum Schluss
  update_l();
  update_d0();
  
  //debug Interface
  handle_debug();

}

