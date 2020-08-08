
const IPAddress ip(192,168,178,245);
const IPAddress gateway(192,168,178,1);
const IPAddress myDns(192,168,178,1);
const IPAddress subnet(255,255,255,0);

const int ethernet_sc_pin     = 53;
const int ethernet_reset_pin  = 50;

//#define Serial SerialUSB

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xAE, 0xEF, 0xAB, 0xE1 };


void setup_ether(){

  //setup ethernet
  Serial.println("===============================");
  Serial.println("Setting up ethernet");
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
    }
  Serial.println("===============================");
  }
  // start the OTEthernet library with internal (flash) based storage
  ArduinoOTA.begin(Ethernet.localIP(), unit_name, "pass", InternalStorage);



  
}


void update_ether(){

   ArduinoOTA.poll();

  
}
