/**
 * => this goes to main file 
// wifi settings, on mega wifi on hw serial 1, on uno on sw serial on pins 6 7
#include "WiFiEsp.h"
char ssid[] = "DRGREENTUMB";            // your network SSID (name)
char pass[] = "JUMPAROUND99";        // your network password
char server[] = "openhabianpi2.fritz.box";
WiFiEspClient client;
**/

/**
#include "WiFiEsp.h"

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

int network_status = WL_IDLE_STATUS;     // the Wifi radio's status


void init_comm(){
  Serial.println("setup_wifi_comm");
  //conecting to wifi network
  connect_network();
  //connect to server
  if ( network_status == WL_CONNECTED) connect_server();
  //reserve message buffer
  message_buffer.reserve(100);
}



void handle_comm(){
  //reconnect eventually

  //call generic message_handler
  message_handler();
}

void connect_network(){
  //a hw reset by io pin could be done here
   Serial1.begin(9600);
  // initialize ESP module
  WiFi.init(&Serial1);
  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("connect_network: WiFi shield not present");
  }
  Serial.println("connect_network: trying to connect to network.");
  network_status = WiFi.begin(ssid, pass);
  if ( network_status == WL_CONNECTED) {
    Serial.println("connect_network: Connected to network.");
    // Print WiFi MAC address
    printMacAddress();
    printWifiStatus();
  }else{
    Serial.println("connect_network: Connecting to network failed.");
  }
}



void connect_server(){
  Serial.println("connect_server: connecting to server...");
  if(client.connect(server, 8888)){
    Serial.println("    success.");
  }else{
    Serial.println("    fail.");
  }
}



void printMacAddress()
{
  // get your MAC address
  byte mac[6];
  WiFi.macAddress(mac);
  
  // print MAC address
  char buf[20];
  sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X", mac[5], mac[4], mac[3], mac[2], mac[1], mac[0]);
  Serial.print("MAC address: ");
  Serial.println(buf);
}



void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}



void listNetworks()
{
  // scan for nearby networks
  int numSsid = WiFi.scanNetworks();
  if (numSsid == -1) {
    Serial.println("Couldn't get a wifi connection");
    while (true);
  }

  // print the list of networks seen
  Serial.print("Number of available networks:");
  Serial.println(numSsid);

  // print the network number and name for each network found
  for (int thisNet = 0; thisNet < numSsid; thisNet++) {
    Serial.print(thisNet);
    Serial.print(") ");
    Serial.print(WiFi.SSID(thisNet));
    Serial.print("\tSignal: ");
    Serial.print(WiFi.RSSI(thisNet));
    Serial.print(" dBm");
    Serial.print("\tEncryption: ");
    printEncryptionType(WiFi.encryptionType(thisNet));
  }
}



void printEncryptionType(int thisType) {
  // read the encryption type and print out the name
  switch (thisType) {
    case ENC_TYPE_WEP:
      Serial.print("WEP");
      break;
    case ENC_TYPE_WPA_PSK:
      Serial.print("WPA_PSK");
      break;
    case ENC_TYPE_WPA2_PSK:
      Serial.print("WPA2_PSK");
      break;
    case ENC_TYPE_WPA_WPA2_PSK:
      Serial.print("WPA_WPA2_PSK");
      break;
    case ENC_TYPE_NONE:
      Serial.print("None");
      break;
  }
  Serial.println();
}
**/
