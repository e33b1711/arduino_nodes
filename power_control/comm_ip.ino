#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <ArduinoOTA.h>

//ethernet
const int ethernet_sc_pin     = 53;
const int ethernet_reset_pin  = 50;
const byte mac[] = {0x4E, 0xAB, 0x7E, 0xEF, 0xFE, 0x04 };
IPAddress ip(192,168,178,213);
EthernetClient client;


void setup_ip(){
    //ethernet & mqtt
    Serial.println("===============================");
    Serial.println("Setting up ethernet / ip / ota update");

    //reset via pin
    pinMode(ethernet_reset_pin, OUTPUT);
    digitalWrite(ethernet_reset_pin, HIGH);
    delay(10);
    digitalWrite(ethernet_reset_pin, LOW);

    //setup ip
    Ethernet.init(ethernet_sc_pin);
    Ethernet.begin(mac, ip);

    //setup OTA
    ArduinoOTA.begin(Ethernet.localIP(), unit_name, password, InternalStorage);
    Serial.println("done.");
    Serial.println("===============================");
}

void handle_ip(){
    //keep alive?
    ArduinoOTA.poll();
}
