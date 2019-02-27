void setup(){
   //init debug interface
  init_debug();
  
  //init comm
  init_comm();

  // init states
  setup_a_b_c();
  setup_t();
  setup_h();
  setup_u();
  setup_r();
  setup_s();
  //states mit ausgängen zum Schluss
  setup_d();
  setup_l();
  
 
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
  update_u();
  update_a_b_c();
  //states mit ausgängen zum Schluss
  update_r();
  update_s();
  update_l();
  update_d();
  update_a_b_c();
  
  //debug Interface
  handle_debug();

}

