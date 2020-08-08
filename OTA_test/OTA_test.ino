/*

 This example polls for sketch updates over Ethernet, sketches
 can be updated by selecting a network port from within
 the Arduino IDE: Tools -> Port -> Network Ports ...

 Circuit:
 * W5100, W5200 or W5500 Ethernet shield attached

 created 13 July 2010
 by dlf (Metodo2 srl)
 modified 31 May 2012
 by Tom Igoe
 modified 16 January 2017
 by Sandeep Mistry
 Ethernet version August 2018
 by Juraj Andrassy
 */
 
#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoOTA.h>

const IPAddress ip(192,168,178,214);
const IPAddress gateway(192,168,178,1);
const IPAddress myDns(192,168,178,1);
const IPAddress subnet(255,255,255,0);

const int ethernet_sc_pin     = 53;
const int ethernet_reset_pin  = 50;

//#define Serial SerialUSB

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xE1 };

void setup() {
  //Initialize serial:
  Serial.begin(9600);
  while (!Serial);

   //setup ethernet
  Ethernet.init(ethernet_sc_pin);
  Ethernet.begin(mac, ip, myDns, gateway, subnet);
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("ERROR: Ethernet shield was not found.");
  }else{
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("ERROR: Ethernet cable is not connected.");
    }else{
      Serial.print("IP address:");
      Serial.println(Ethernet.localIP());
      Serial.println("SUCESS!");
      Serial.println("VErsion99!");
  }
  Serial.println("===============================");
  }

  // start the OTEthernet library with internal (flash) based storage
  ArduinoOTA.begin(Ethernet.localIP(), "Peter", "pass", InternalStorage);
}

void loop() {
  // check for updates
  ArduinoOTA.poll();

  // add your normal loop code below ...
}
