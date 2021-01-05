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
#include <PubSubClient.h>


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


// Function prototypes
void subscribeReceive(char* topic, byte* payload, unsigned int length);
 
// Set your MAC address and IP address here
 
// Make sure to leave out the http and slashes!
const char* server = "test.mosquitto.org";
 
// Ethernet and MQTT related objects
EthernetClient ethClient;
PubSubClient mqttClient(client);

void publish_mqtt(const char topic[], int precision, float value){
    char payload[41];
    dtostrf(value, 0, precision, payload);
    if(!mqttClient.publish(topic, payload)){
        Serial.print("Could not send message.");
    }
}

void publish_mqtt(const char topic[], char value[]){
    if(!mqttClient.publish(topic, value)){
        Serial.print("Could not send message.");
    }
}
 

//server throttle timer
unsigned long lastServerUpd;
const unsigned long serverUpdPeriod = 10000;


void setup_server(){
    //ethernet & mqtt
    Serial.println("===============================");
    Serial.println("Setting up ethernet / mqtt / ntp / ota");
    lastServerUpd = millis();

    //reset via pin
    pinMode(ethernet_reset_pin, OUTPUT);
    digitalWrite(ethernet_reset_pin, HIGH);
    delay(10);
    digitalWrite(ethernet_reset_pin, LOW);

    //setup mttq
    Serial.println(F("Init the mqtt client..."));
    Ethernet.init(ethernet_sc_pin);
    Ethernet.begin(mac, ip);
    delay(2000); //give the ethernet a second to initialize

    // Set the MQTT server to the server stated above ^
    mqttClient.setServer(AIO_SERVER, AIO_SERVERPORT);   

    // Attempt to connect to the server with the ID "myClientID"
    if (mqttClient.connect(AIO_USERNAME)){
        Serial.println("Connection has been established, well done");
        // Establish the subscribe event
        mqttClient.setCallback(subscribeReceive);
    }else{
        Serial.println("Looks like the server connection failed...");
    }

    //time & OTA
    Serial.println("===============================");
    Serial.println("Setting up OTA....");
    Udp.begin(localPort);
    ArduinoOTA.begin(Ethernet.localIP(), unit_name, password, InternalStorage);
    Serial.println("done.");
    Serial.println("===============================");
}

//called form main loop
void handle_server(){
   
    if((lastServerUpd+serverUpdPeriod)<millis()){
        lastServerUpd = millis();

        Serial.println("=====================================");
        Serial.println("Publishing via mqtt...");

        // This is needed at the top of the loop!
        mqttClient.loop();

        // Ensure that we are subscribed to the topic "MakerIOTopic"
        //mqttClient.subscribe(AIO_USERNAME "/controlMode");

        if(!mqttClient.connected()){
            Serial.print("Trying to reconnected to MQTT broker...");
            mqttClient.connect(AIO_USERNAME);
        }

        if(mqttClient.connected()){
            publish_mqtt((char *) AIO_USERNAME "/powerPV",       0,  powerPV);
            publish_mqtt((char *) AIO_USERNAME "/powerHeat",     0,  powerHeat);
            publish_mqtt((char *) AIO_USERNAME "/powerBal",      0,  bal_power);
            publish_mqtt((char *) AIO_USERNAME "/energyImport",  3,  energyImport());
            publish_mqtt((char *) AIO_USERNAME "/energyExport",  3,  energyExport);
            publish_mqtt((char *) AIO_USERNAME "/energyPV",      3,  energyPV());
            publish_mqtt((char *) AIO_USERNAME "/energyHeat",    3,  energyHeat());
            publish_mqtt((char *) AIO_USERNAME "/tempLow",       1,  tempLow);
            publish_mqtt((char *) AIO_USERNAME "/tempHigh",      1,  tempHigh);
            publish_mqtt((char *) AIO_USERNAME "/version",           vers);
            publish_mqtt((char *) AIO_USERNAME "/status",            "started");
            Serial.println("SUCCESS.");
        }else{
            Serial.println("FAIL.");
        }


        Serial.println("=====================================");
    }
}





void subscribeReceive(char* topic, byte* payload, unsigned int length)
{
  // Print the topic
  Serial.print("Topic: ");
  Serial.println(topic);
 
  // Print the message
  Serial.print("Message: ");
  for(int i = 0; i < length; i ++)
  {
    Serial.print(char(payload[i]));
  }
 
  // Print a newline
  Serial.println("");
}

void print_server_info(){
  Serial.println("===========server==============");
  Serial.print("lastServerUpd+serverUpdPeriod: "); Serial.println(lastServerUpd+serverUpdPeriod);
  Serial.print("millis: "); Serial.println(millis());
   Serial.println("=============================");
}
