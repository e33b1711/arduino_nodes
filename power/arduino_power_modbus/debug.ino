long last_update_debug;
const long debug_update_interval = 5000;

void setup_debug(){
  Serial.begin(9600);
  Serial.println("===============================");
  Serial.println("Power controller unit statrted.");
  Serial.println("===============================");
  last_update_debug = millis();
}

//called form main loop
void handle_debug(){

  //print sensor values & setpoint periodically
  if (last_update_debug+debug_update_interval < millis()){
    last_update_debug = millis();
    
   
    print_heating_info();
    
    
 
    print_s0_info();
   
  }
  
  
}
