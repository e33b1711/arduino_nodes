// wifi settings, on mega wifi on hw serial 1, on uno on sw serial on pins 6 7
#include "WiFiEsp.h"
char ssid[] = "DRGREENTUMB";            // your network SSID (name)
char pass[] = "JUMPAROUND99";        // your network password
char server[] = "openhabianpi2.fritz.box";
WiFiEspClient client;

void setup() {
  Serial.begin(9600);


  setup_wifi_comm();
 
}

void loop()
{

  handle_wifi_comm();

 
}
