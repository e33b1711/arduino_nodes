/***************************************************
  Adafruit MQTT Library Ethernet Example

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Alec Moore
  Derived from the code written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
#include <SPI.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#include <Ethernet.h>
#include <EthernetClient.h>
#include <Dns.h>
#include <Dhcp.h>

/************************* Ethernet Client Setup *****************************/
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
const int ethernet_sc_pin     = 53;

//Uncomment the following, and set to a valid ip if you don't have dhcp available.
IPAddress iotIP (192, 168, 178, 134);
//Uncomment the following, and set to your preference if you don't have automatic dns.
//IPAddress dnsIP (8, 8, 8, 8);
//If you uncommented either of the above lines, make sure to change "Ethernet.begin(mac)" to "Ethernet.begin(mac, iotIP)" or "Ethernet.begin(mac, iotIP, dnsIP)"


/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "192.168.178.222"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "power_control"
#define AIO_KEY         ""

/************ Global State (you don't need to change this!) ******************/

//Set up the ethernet client
EthernetClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// You don't need to change anything below this line!
#define halt(s) { Serial.println(F( s )); while(1);  }


/****************************** Feeds ***************************************/

// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish powerUtilityPub   = Adafruit_MQTT_Publish(&mqtt,  AIO_USERNAME "/powerUtility");
Adafruit_MQTT_Publish powerPVPub        = Adafruit_MQTT_Publish(&mqtt,  AIO_USERNAME "/powerPV");
Adafruit_MQTT_Publish powerHeating      = Adafruit_MQTT_Publish(&mqtt,  AIO_USERNAME "/powerHeating");

/*************************** Sketch Code ************************************/

void setup() {
  Serial.begin(9600);

  Serial.println(F("Adafruit MQTT demo"));

  // Initialise the Client
  Serial.print(F("\nInit the Client..."));
  Ethernet.init(ethernet_sc_pin);
  Ethernet.begin(mac, iotIP);
  delay(1000); //give the ethernet a second to initialize
}

uint32_t x=0;

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  /*
  // this is our 'wait for incoming subscription packets' busy subloop
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(1000))) {
    if (subscription == &onoffbutton) {
      Serial.print(F("Got: "));
      Serial.println((char *)onoffbutton.lastread);
    }
  }
  */

  // Now we can publish stuff!
  Serial.print(F("\nSending photocell val "));
  Serial.print(x);
  Serial.print("...");
  if (! powerUtilityPub.publish(x++)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }
  if (! powerHeating.publish(x++)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }
  if (! powerPVPub.publish(x++)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }

  // ping the server to keep the mqtt connection alive
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }

}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
  }
  Serial.println("MQTT Connected!");
}
