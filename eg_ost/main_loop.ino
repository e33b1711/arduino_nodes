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
    
 
  //react to messages
  handle_comm();
  //update states
  //user logic is called from update_a_b_c
  update_a_b_c();
  update_t();
  //Serial.println("loop12");
  update_h();
  //Serial.println("loop13");
  update_u();
  //Serial.println("loop14");
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

