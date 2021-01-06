//watch out for the pins needed for the ethernet schield (always 10, 11 12 13 on uno, 50 51 52 53 on mega!)
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <PubSubClient.h>
#include <ArduinoOTA.h>

//ethernet
const int ethernet_sc_pin     = 53;
const int ethernet_reset_pin  = 50;
const byte mac[] = {0x4E, 0xAB, 0x7E, 0xEF, 0xFE, 0x04 };
const IPAddress ip(192,168,178,213);
const IPAddress                 gateway(192,168,178,1);
const IPAddress                 subnet(255,255,255,0);
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
    Ethernet.begin(mac, ip, gateway, subnet);
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
    ArduinoOTA.begin(Ethernet.localIP(), unit_name, password, InternalStorage);
    Serial.println("done.");
    Serial.println("===============================");
}

//called form main loop
void handle_server(){
    
    //OTA
    ArduinoOTA.poll();
   
    if((lastServerUpd+serverUpdPeriod)<millis()){
        lastServerUpd = millis();

        Serial.println("=====================================");
        Serial.println("Publishing via mqtt...");

        //reconnect
        /*
        if(!mqttClient.connected()){
            // Attempt to connect to the server with the ID "myClientID"
            if (mqttClient.connect(AIO_USERNAME)){
                Serial.println("Connection has been established, well done");
                // Establish the subscribe event
                mqttClient.setCallback(subscribeReceive);
            }else{
                Serial.println("Looks like the server connection failed...");
            }
        }
        */

        // This is needed at the top of the loop!
        mqttClient.loop();

        // Ensure that we are subscribed to the topic "MakerIOTopic"
        if (mqttClient.subscribe("power_control/#")){
            Serial.println("subcribe SUCCESS.");
        }else{
           Serial.println("subcribe FAIL.");
        }
        //mqttClient.subscribe((char *) AIO_USERNAME "/testTopic1");
        //mqttClient.subscribe("power_control/#");

        

        //if(mqttClient.connected()){
            publish_mqtt((char *) AIO_USERNAME "/powerPV",       0,  powerPV);
            publish_mqtt((char *) AIO_USERNAME "/powerHeat",     0,  powerHeat);
            publish_mqtt((char *) AIO_USERNAME "/powerBal",      0,  bal_power);
            publish_mqtt((char *) AIO_USERNAME "/energyImport",  3,  energyImport*0.001);
            publish_mqtt((char *) AIO_USERNAME "/energyExport",  3,  energyExport*0.001);
            publish_mqtt((char *) AIO_USERNAME "/energyPV",      3,  energyPV());
            publish_mqtt((char *) AIO_USERNAME "/energyHeat",    3,  energyHeat());
            publish_mqtt((char *) AIO_USERNAME "/tempLow",       1,  tempLow);
            publish_mqtt((char *) AIO_USERNAME "/controlMode",   1,  control_mode);
            publish_mqtt((char *) AIO_USERNAME "/targetPower",   1,  target_power);
            publish_mqtt((char *) AIO_USERNAME "/tempHigh",      1,  tempHigh);
            publish_mqtt((char *) AIO_USERNAME "/waterWarning",  1,  waterWarning);
            publish_mqtt((char *) AIO_USERNAME "/version",           vers);
            publish_mqtt((char *) AIO_USERNAME "/status",            "started");
        //    Serial.println("SUCCESS.");
        //}else{
        //    Serial.println("FAIL.");
        //}


        Serial.println("=====================================");
    }
}





void subscribeReceive(char* topic, byte* payload, unsigned int length)
{
  Serial.println("===========mqtt in==============");
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
  Serial.println("===========mqtt in==============");
}

void print_server_info(){
  Serial.println("===========server==============");
  Serial.print("lastServerUpd+serverUpdPeriod: "); Serial.println(lastServerUpd+serverUpdPeriod);
  Serial.print("millis: "); Serial.println(millis());
   Serial.println("=============================");
}
