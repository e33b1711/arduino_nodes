
const int port = 8889;
EthernetServer server(port);
long lastServerUpdate;
const long serverUpdatePeriod = 10000;





void setup_server(){
  Serial.println("===============================");
  Serial.println("Setting up server...");

  lastServerUpdate = millis();

    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("ERROR: Ethernet cable is not connected.");
    }else{
      server.begin();
      Serial.println("done.");
  }
  Serial.println("===============================");
}
  


//called form main loop
void handle_server(){
    
  if (lastServerUpdate+serverUpdatePeriod<millis()) {
    //Serial.println("DEBUG: Sending server update.");
    server.println("===========================");
    server.print("Unit name:");
    server.println(unit_name);
    server.print("Version:");
    server.println(vers);
    //heating
    server.print("bal_power:");
    server.println(bal_power);
    server.print("bal_power_valid:");
    server.println(bal_power_valid);
    server.print("pwm_setpoint:");
    server.println(pwm_setpoint);
    server.print("watchdog_counter:");
    server.println(watchdog_counter);
    //modbus
    server.print("sdm_data:");
    for(int i=0; i<SDM_SIZE; i++){
      //server.print(get_sdm_data(i));
      server.print(sdm_data[i]);
      server.print(", ");
    }
    server.println("");
    server.print("sdm_data_valid:");
    for(int i=0; i<SDM_SIZE; i++){
      server.print(sdm_data_valid[i]);
      server.print(", ");
    };
    server.println("");
    //s0 sensors
    server.print("powerUtility:");
    server.println(powerUtility);
    server.print("errorUtility:");
    server.println(errorUtility);
    server.print("powerHeating:");
    server.println(powerHeating);
    server.print("errorHeating:");
    server.println(errorHeating);
    server.print("powerPV:");
    server.println(powerPV);
    server.print("errorPV:");
    server.println(errorPV);
    server.println("===========================");
    lastServerUpdate = millis(); 
  }
  
}
