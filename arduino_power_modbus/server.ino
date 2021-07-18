//for ntp time & OTA
#include <EthernetUdp.h>
#include <ArduinoOTA.h>
EthernetUDP Udp;
unsigned int localPort = 8888;                // local port to listen for UDP packets
const int NTP_PACKET_SIZE = 48;               // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[NTP_PACKET_SIZE];           //buffer to hold incoming and outgoing packets
const char timeServer[] = "time.nist.gov";    // time.nist.gov NTP server

const unsigned long seconds_per_day = 86400;  //exactly what it says 

unsigned long  epoch_at_millis0;          //timebase referneced to millis=0
unsigned long  epoch_at_timebase;         //time we got the timebase
unsigned long startup_unix_day;           //unix day at statrtup
unsigned long startup_seconds_today;      //unix secondes at statrtup
bool timebase_valid = false;              //do we have a timebase?
unsigned long epoch;                      //epoch updated every cycle
unsigned long seconds_today;              //recent uninx seconds of this day
unsigned long unix_day;                   // this unix day 



//for udp communication
//watch out for the pins needed for the ethernet schield (always 10, 11 12 13 on uno, 50 51 52 53 on mega!)
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>


//ethernet
const int ethernet_sc_pin     = 53;
const int ethernet_reset_pin  = 50;
const byte mac[] = {0x4E, 0xAB, 0x7E, 0xEF, 0xFE, 0x04 };
IPAddress ip(192,168,178,213);
EthernetClient client;



//server throttle timer
unsigned long lastServerUpd;
const unsigned long serverUpdPeriod = 10000;


void setup_server(){
    //ethernet
    Serial.println("===============================");
    Serial.println("Setting up ethernet / mqtt / ntp / ota");

    //reset via pin
    pinMode(ethernet_reset_pin, OUTPUT);
    digitalWrite(ethernet_reset_pin, HIGH);
    delay(10);
    digitalWrite(ethernet_reset_pin, LOW);

  

    //time & OTA
    Serial.println("===============================");
    Serial.println("Setting up  OTA....");
    ArduinoOTA.begin(Ethernet.localIP(), unit_name, password, InternalStorage);
    Serial.println("done.");
    Serial.println("===============================");
}

//called form main loop
void handle_server(){
   
  //OTA
  ArduinoOTA.poll();
}



void print_server_info(){
  Serial.println("===========server==============");
  Serial.print("lastServerUpd+serverUpdPeriod: "); Serial.println(lastServerUpd+serverUpdPeriod);
  Serial.print("millis: "); Serial.println(millis());
   Serial.println("=============================");
}
