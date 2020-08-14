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


const int ethernet_sc_pin     = 53;
const int ethernet_reset_pin  = 50;

const byte mac[] = {0x4E, 0xAB, 0x7E, 0xEF, 0xFE, 0x04 };
IPAddress ip(192,168,178,213);
//const IPAddress gateway(192,168,178,1);
//const IPAddress myDns(192,168,178,1);
//const IPAddress subnet(255,255,255,0);



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
Adafruit_MQTT_Publish powerPVPub        = Adafruit_MQTT_Publish(&mqtt,  AIO_USERNAME "/powerPV");
Adafruit_MQTT_Publish powerHeatPub   = Adafruit_MQTT_Publish(&mqtt,  AIO_USERNAME "/powerHeat");
Adafruit_MQTT_Publish powerBalPub       = Adafruit_MQTT_Publish(&mqtt,  AIO_USERNAME "/powerBal");

Adafruit_MQTT_Publish energyImportPub   = Adafruit_MQTT_Publish(&mqtt,  AIO_USERNAME "/energyImport");
Adafruit_MQTT_Publish energyExportPub   = Adafruit_MQTT_Publish(&mqtt,  AIO_USERNAME "/energyExport");
Adafruit_MQTT_Publish energyPVPub       = Adafruit_MQTT_Publish(&mqtt,  AIO_USERNAME "/energyPV");
Adafruit_MQTT_Publish energyHeatPub     = Adafruit_MQTT_Publish(&mqtt,  AIO_USERNAME "/energyHeat");

Adafruit_MQTT_Publish lastEnergyImportPub   = Adafruit_MQTT_Publish(&mqtt,  AIO_USERNAME "/lastEnergyImport");
Adafruit_MQTT_Publish lastEnergyExportPub   = Adafruit_MQTT_Publish(&mqtt,  AIO_USERNAME "/lastEnergyExport");
Adafruit_MQTT_Publish lastEnergyPVPub       = Adafruit_MQTT_Publish(&mqtt,  AIO_USERNAME "/lastEnergyPV");
Adafruit_MQTT_Publish lastEnergyHeatPub     = Adafruit_MQTT_Publish(&mqtt,  AIO_USERNAME "/lastEnergyHeat");

Adafruit_MQTT_Publish versionPub     = Adafruit_MQTT_Publish(&mqtt,  AIO_USERNAME "/version");

Adafruit_MQTT_Publish statusPub     = Adafruit_MQTT_Publish(&mqtt,  AIO_USERNAME "/status");


unsigned long lastServerUpdate;
const unsigned long serverUpdatePeriod = 10000;


//ntp
#include <EthernetUdp.h>
EthernetUDP Udp;
unsigned int localPort = 8888;       // local port to listen for UDP packets
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
const char timeServer[] = "time.nist.gov"; // time.nist.gov NTP server
unsigned long  epoch_at_millis0;
const unsigned long seconds_per_day = 86400;
unsigned long last_unix_day;
unsigned long startup_unix_day;
unsigned long startup_seconds_today;
bool day_is_whole = false;
bool last_day_is_whole = false;


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
  Serial.println(F("\nInit the mqtt client..."));
  Ethernet.init(ethernet_sc_pin);
  Ethernet.begin(mac, ip);
  delay(1000); //give the ethernet a second to initialize

   if(MQTT_connect()){
        versionPub.publish(vers,20);
        statusPub.publish("Started.",8);
    }else{
        Serial.print("ERROR: MQTT Broker not rechable. ");
    }

  //setup ntp
  Udp.begin(localPort);
  update_time();
  unsigned long epoch           = epoch_at_millis0 + millis()/1000;
  unsigned long secondes_today  = epoch % seconds_per_day;
  unsigned long unix_day        = epoch / seconds_per_day;
  startup_unix_day              = unix_day;
  startup_seconds_today         = secondes_today;

  lastServerUpdate=millis();

  ArduinoOTA.begin(Ethernet.localIP(), unit_name, password, InternalStorage);
  
  Serial.println("===============================");
  
  
}

//called form main loop
void handle_server(){
  ArduinoOTA.poll();

 
  //update the time before millis() overflows
  if(millis() > 0x0FFFFFFF){
    update_time();
  }
  


    if (lastServerUpdate+serverUpdatePeriod<millis()) {

      
      //check new day / get time
      unsigned long epoch           = epoch_at_millis0 + millis()/1000;
      unsigned long secondes_today  = epoch % seconds_per_day;
      unsigned long unix_day        = epoch / seconds_per_day;
      if (last_unix_day < unix_day){
        Serial.println("New UTC/unix day.");
        last_unix_day = unix_day;
        new_day_S0();
        new_day_sdm();
        last_day_is_whole = day_is_whole;
        day_is_whole  = true;
        if(last_day_is_whole){
          lastEnergyImportPub.publish(lastEnergyImport,4);
          lastEnergyExportPub.publish(lastEnergyExport,4);
          lastEnergyPVPub.publish(lastEnergyPV,4);
          lastEnergyHeatPub.publish(lastEnergyHeat,4);
        }
      }
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
        Serial.print("ERROR: MQTT Broker not rechable. ");
      }

      lastServerUpdate = millis(); 
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


// send an NTP request to the time server at the given address
void sendNTPpacket(const char * address) {
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); // NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

void update_time(){
  Serial.println("=================="); 
  Serial.println("Getting NTP time...");
  sendNTPpacket(timeServer); // send an NTP packet to a time server
  delay(1000);
  if (Udp.parsePacket()) {
    // We've received a packet, read the data from it
    Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

    // the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, extract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    //Serial.print("Seconds since Jan 1 1900 = ");
    //Serial.println(secsSince1900);

    // now convert NTP time into everyday time:
    Serial.print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;
    epoch_at_millis0 = epoch - millis()/1000;
    // print Unix time:
    Serial.println(epoch);

    /*
    // print the hour, minute and second:
    Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
    Serial.print((epoch  % 86400L) / 3600); // print the hour (86400 equals secs per day)
    Serial.print(':');
    if (((epoch % 3600) / 60) < 10) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.print((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
    Serial.print(':');
    if ((epoch % 60) < 10) {
      // In the first 10 seconds of each minute, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.println(epoch % 60); // print the second
    */
  }else{
    Serial.println("ERROR: Got no ntp response");
  }
  Serial.println("==================");
}
