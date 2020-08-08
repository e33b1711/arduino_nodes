//for udp communication
//watch out for the pins needed for the ethernet schield (always 10, 11 12 13 on uno, 50 51 52 53 on mega!)
#include <Ethernet.h>
#include <ArduinoOTA.h>

const int ethernet_sc_pin     = 53;
const int ethernet_reset_pin  = 50;
const byte mac[] = {0x4E, 0xAB, 0x7E, 0xEF, 0xFE, 0x04 };
const IPAddress ip(192,168,178,213);
const IPAddress gateway(192,168,178,1);
const IPAddress myDns(192,168,178,1);
const IPAddress subnet(255,255,255,0);
const int port = 8889;
EthernetServer server(port);
bool alreadyConnected = false; // whether or not the client was connected previously
long lastServerUpdate;
long serverUpdatePeriod = 10000;




void setup_server(){
  Serial.println("===============================");
  Serial.println("Setting up server...");

  lastServerUpdate = millis();

  //reset via pin
  pinMode(ethernet_reset_pin, OUTPUT);
  digitalWrite(ethernet_reset_pin, HIGH);
  delay(10);
  digitalWrite(ethernet_reset_pin, LOW);

  //setup sever
  Ethernet.init(ethernet_sc_pin);
  Ethernet.begin(mac, ip, myDns, gateway, subnet);
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("ERROR: Ethernet shield was not found.");
  }else{
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("ERROR: Ethernet cable is not connected.");
    }else{
      server.begin();
       ArduinoOTA.begin(Ethernet.localIP(), unit_name, password, InternalStorage);
      Serial.print("Server address:");
      Serial.println(Ethernet.localIP());
      Serial.print("Chat server port:");
      Serial.println(port);
      Serial.println("SUCESS!");
  }
  Serial.println("===============================");
  }
  
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
    server.print("bal_power:        ");
    server.println(bal_power);
    server.print("bal_power_valid:  ");
    server.println(bal_power_valid);
    server.print("pwm_setpoint: ");
    server.println(pwm_setpoint);
    server.print("watchdog_counter: ");
    server.println(watchdog_counter);
    //modbus
    server.print("sdm_data:         ");
    for(int i=0; i<SDM_SIZE; i++){
      //server.print(get_sdm_data(i));
      server.print(sdm_data[i]);
      server.print(", ");
    }
    server.println("");
    server.print("sdm_data_valid:   ");
    for(int i=0; i<SDM_SIZE; i++){
      server.print(sdm_data_valid[i]);
      server.print(", ");
    };
    server.println("");
    //s0 sensors
    server.print("powerUtility:     ");
    server.println(powerUtility);
    server.print("errorUtility:     ");
    server.println(errorUtility);
    server.print("powerHeating:     ");
    server.println(powerHeating);
    server.print("errorHeating:     ");
    server.println(errorHeating);
    server.print("powerPV:          ");
    server.println(powerPV);
    server.print("errorPV:          ");
    server.println(errorPV);
    server.println("===========================");
    lastServerUpdate = millis(); 
  }
  
}
