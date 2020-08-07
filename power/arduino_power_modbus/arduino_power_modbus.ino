//this nodes name
const String unit_name = "pow";


//for tcp communication
//watch out for the pins needed for the ethernet schield (always 10, 11 12 13 on uno, 50 51 52 53 on mega!)
#include <Ethernet.h>
const int ethernet_sc_pin = 53;
const byte mac[] = {0x4E, 0xAB, 0x7E, 0xEF, 0xFE, 0x04 };
const IPAddress ip(192,168,178,213);
const IPAddress server(192,168,178,222);
const IPAddress gateway(192,168,178,1);
const IPAddress subnet(255,255,255,0);
const int port = 8888;


#include <ModbusMaster.h> // EXTREN LIB...  have to be included
ModbusMaster node; //MODBUS NODE NUMBER OF SDM630 .. must be the same as configured in SDM630
const int SDM_SIZE = 8; 
float sdm_data[SDM_SIZE];
bool modbus_data_valid[SDM_SIZE];
static int MAX_ITERATION = 1; //maximum read MODBUS Value if checksum fails
long last_modbus_update;
const long modbus_update_i = 1000;
bool modbus_update      = true;
int round_counter=0;
//                                      Voltage L1-3            Power L1-3              Phase A. Freq
const uint16_t sdm_adresses[SDM_SIZE]= {0x0000, 0x0002, 0x0004, 0x000C, 0x000E, 0x0010, 0x0042, 0x0046};


const int sensorPin         = 9;                        // digital input of the power meter pulses (Utility power, only consumption)
const int sensorPin1        = 10;                        // power to electric heating
const int sensorPin2        = 11;                        // power from PV
const int ledPin            = 13;                       // LED output pin

const int pwmPin            = 4;
int pwm_setpoint             = 0;
String inString             = "";
long watchDog;

const double meterConstant  = 3600000;                  // 1 mWs
const double meterConstant1 = 1800000;                  // 0.5 mWs
const double meterConstant2 = 1800000;                  // "

long now;                                               //timestamp in milliseconds (now())

bool sensorValue            = false;                    //flank detection and timestamps of flanks         
bool oldValue               = false;
long lastNegFlank;
long lastPosFlank;

bool sensorValue1           = false; 
bool oldValue1              = false;
long lastNegFlank1;
long lastPosFlank1;

bool sensorValue2           = false; 
bool oldValue2              = false;
long lastNegFlank2;
long lastPosFlank2;

int powerUtility;                                           //measured power values & update flags / counters & suppression of first update
int errorUtility               = false;                     //error condition are not reversible and require a reset
bool powerUpdateUtility        = false;
long nextUpdateUtility         = 0;
bool killUpdate             = false;

int powerHeating;
bool errorHeating           = false;
bool powerUpdateHeating     = false;
long nextUpdateHeating      = 0;
bool killUpdate1            = false;

int powerPV;
bool errorPV                = false;
bool powerUpdatePV          = false;
long nextUpdatePV           = 0;
bool killUpdate2            = false;

void setup() {
  
  //setup pins
  pinMode(sensorPin, INPUT_PULLUP);
  
  pinMode(ledPin, OUTPUT);
  pinMode(pwmPin, OUTPUT);
  analogWrite(pwmPin, 0);

  //setup uart
  Serial.begin(9600);

  //needed to supress wrong messages at startup
  lastNegFlank  = millis();
  lastNegFlank1  = millis();
  lastNegFlank2  = millis();


  //modbus
  last_modbus_update = millis();
  Serial2.begin(9600);
  node.begin(1, Serial2);

  //comm
  init_comm();
}

void loop() {
  
  handle_comm();

  poll_modbus();

  update_s0();

  if (modbus_update){
    update_heating();
  }

  


 
 /*
  if (powerUpdateUtility){
    Serial.print("powerUtility:");
    Serial.println(powerUtility);
    Serial.print("errorUtility:");
    Serial.println(errorUtility); 
  }
  if (powerUpdatePV){
    Serial.print("powerPV:");
    Serial.println(powerPV); 
    Serial.print("errorPV:");
    Serial.println(errorPV);    
  }
  */
  if (powerUpdateHeating){
    Serial.print("powerHeating:");
    Serial.println(powerHeating);   
    Serial.print("errorHeating:");
    Serial.println(errorHeating);  
  }
  /*
  if (modbus_update){
    Serial.println("debug: modbus_to_serial...");
    modbus_to_serial();
    Serial.println("debug: modbus_to_tcp...");
    modbus_to_tcp();
    Serial.println("debug: done.");
  }
  */
  




    
}
