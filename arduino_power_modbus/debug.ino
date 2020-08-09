unsigned long last_update_debug;
const unsigned long debug_update_interval = 4300;

void setup_debug(){
  Serial.begin(9600);
  Serial.println("===============================");
  Serial.println("Power controller unit started.");
  Serial.println("===============================");
  last_update_debug = millis();
}

//called form main loop
void handle_debug(){
  //print sensor values & setpoint periodically
  if (last_update_debug+debug_update_interval < millis()){
    last_update_debug = millis();
    Serial.println("===============================");
    Serial.print("Unit name:");
    Serial.println(unit_name);
    Serial.print("Version:");
    Serial.println(vers);
    Serial.println("===============================");
    print_heating_info();
    print_s0_info();
    modbus_to_serial();
  } 
}
