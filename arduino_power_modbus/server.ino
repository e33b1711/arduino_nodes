//for udp communication
//watch out for the pins needed for the ethernet schield (always 10, 11 12 13 on uno, 50 51 52 53 on mega!)
#include <SPI.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#include <Ethernet.h>
#include <EthernetClient.h>
#include <Dns.h>
#include <Dhcp.h>
#include <ArduinoOTA.h>

//ethernet
const int ethernet_sc_pin     = 53;
const int ethernet_reset_pin  = 50;
const byte mac[] = {0x4E, 0xAB, 0x7E, 0xEF, 0xFE, 0x04 };
IPAddress ip(192,168,178,213);
EthernetClient client;

//mqtt
#define AIO_SERVER      "192.168.178.222"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "power_control"
#define AIO_KEY         ""
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
#define halt(s) { Serial.println(F( s )); while(1);  }


//mqtt feeds
Adafruit_MQTT_Publish powerPVPub        = Adafruit_MQTT_Publish(&mqtt,  AIO_USERNAME "/powerPV");
Adafruit_MQTT_Publish powerHeatPub   = Adafruit_MQTT_Publish(&mqtt,  AIO_USERNAME "/powerHeat");
Adafruit_MQTT_Publish powerBalPub       = Adafruit_MQTT_Publish(&mqtt,  AIO_USERNAME "/powerBal");

Adafruit_MQTT_Publish energyImportPub   = Adafruit_MQTT_Publish(&mqtt,  AIO_USERNAME "/energyImport");
Adafruit_MQTT_Publish energyExportPub   = Adafruit_MQTT_Publish(&mqtt,  AIO_USERNAME "/energyExport");
Adafruit_MQTT_Publish energyPVPub       = Adafruit_MQTT_Publish(&mqtt,  AIO_USERNAME "/energyPV");
Adafruit_MQTT_Publish energyHeatPub     = Adafruit_MQTT_Publish(&mqtt,  AIO_USERNAME "/energyHeat");

Adafruit_MQTT_Publish versionPub     = Adafruit_MQTT_Publish(&mqtt,  AIO_USERNAME "/version");
Adafruit_MQTT_Publish statusPub     = Adafruit_MQTT_Publish(&mqtt,  AIO_USERNAME "/status");


unsigned long lastServerUpdate;
const unsigned long serverUpdatePeriod = 10000;





void setup_server(){
  Serial.println("===============================");
  Serial.println("Setting up ethernet / mqtt / ntp / ota");
  lastServerUpdate = millis();

  //reset via pin
  pinMode(ethernet_reset_pin, OUTPUT);
  digitalWrite(ethernet_reset_pin, HIGH);
  delay(10);
  digitalWrite(ethernet_reset_pin, LOW);

  //setup mttq
  Serial.println(F("Init the mqtt client..."));
  Ethernet.init(ethernet_sc_pin);
  Ethernet.begin(mac, ip);
  delay(1000); //give the ethernet a second to initialize

  if(MQTT_connect()){
    versionPub.publish(vers,20);
    statusPub.publish("Started.",8);
  }else{
    Serial.println("ERROR: MQTT Broker not rechable. ");
  }

  ArduinoOTA.begin(Ethernet.localIP(), unit_name, password, InternalStorage);
  

  Serial.println("===============================");
}

//called form main loop
void handle_server(){
  ArduinoOTA.poll();

    if (lastServerUpdate+serverUpdatePeriod<millis()) {
      lastServerUpdate = millis(); 

      /*
      Serial.println("=====================================");
      Serial.println("Publishing via mqtt...");
      Serial.print("Unix day: ");
      Serial.println(unix_day);
      Serial.print("Unix secondes today: ");
      Serial.println(secondes_today);
      */
      
      if(MQTT_connect()){
        powerPVPub.publish(powerPV, 4);
        powerHeatPub.publish(powerHeat, 4);
        powerBalPub.publish(bal_power, 4);
        //
        energyImportPub.publish(energyImport, 4);
        energyExportPub.publish(energyExport, 4);
        energyPVPub.publish(energyPV, 4);
        energyHeatPub.publish(energyHeat, 4);
      }else{
        Serial.println("ERROR: MQTT Broker not rechable. ");
      }
 
      //Serial.println("=====================================");
    }
 
}


bool MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  if ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       return false;
  }else{
    Serial.println("MQTT Connected!");
    return true;
  }
}
