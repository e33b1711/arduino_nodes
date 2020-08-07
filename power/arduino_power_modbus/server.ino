//for udp communication
//watch out for the pins needed for the ethernet schield (always 10, 11 12 13 on uno, 50 51 52 53 on mega!)
#include <Ethernet.h>
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

  EthernetClient client = server.available();

  // when the client sends the first byte, say hello:
  /*
  if (client) {
      // clear out the input buffer:
      client.flush();
      Serial.println("New Server Client");
      alreadyConnected = true;
    }
  */
  if (lastServerUpdate+serverUpdatePeriod<millis()) {
    //TODO: usefull info
    //TODO: also to debug
    Serial.println("DEBUG: Sending server update.");
    server.println("Power Info 0.");
    server.println("Power Info 1.");
    server.println("Power Info 2.");
    lastServerUpdate = millis(); 
  }
  //TODO: incoming: power setpoint / direct mode

  
}
