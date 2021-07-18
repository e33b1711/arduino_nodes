#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoOTA.h>


EthernetClient client;

void init_comm()
{
  // initialize
  Ethernet.init(ethernet_sc_pin);
  Ethernet.begin(mac, ip, gateway, subnet);
  // give the Ethernet shield a second to initialize:
  delay(500);
  Serial.println("init_comm: connecting...");
  //OTA
  ArduinoOTA.begin(Ethernet.localIP(), unit_name, password, InternalStorage);
}

void handle_comm(){
  //OTA
  ArduinoOTA.poll();
}
