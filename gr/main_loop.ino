void setup(){

   //init debug interface
  init_debug();
  
  //init comm
  init_comm();

  // init states
  setup_a_b_c();
  setup_t();
  setup_h();
  setup_do();

  //states mit ausgängen zum Schluss
  setup_l();
}


void loop()
{

  //react to messages
  handle_comm();

  //update states
  //user logic is called from update_a_b_c
  update_a_b_c();
  update_t();
  update_h();
  update_a_b_c();
  update_l();
  update_do();
  update_a_b_c();

  //debug Interface
  handle_debug();

}
