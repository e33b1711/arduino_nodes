

const int sensorPin         = 9;                        // digital input of the power meter pulses (Utility power, only consumption)
const int sensorPin1        = 10;                        // power to electric heating
const int sensorPin2        = 11;                        // power from PV
const int ledPin            = 13;                       // LED output pin


const double meterConstant  = 3600000;                  // 1 mWs
const double meterConstant1 = 1800000;                  // 0.5 mWs
const double meterConstant2 = 1800000;                  // "

unsigned long now;                                               //timestamp in milliseconds (now())

bool sensorValue            = false;                    //flank detection and timestamps of flanks         
bool oldValue               = false;
unsigned long lastNegFlank;
unsigned long lastPosFlank;

bool sensorValue1           = false; 
bool oldValue1              = false;
unsigned long lastNegFlank1;
unsigned long lastPosFlank1;

bool sensorValue2           = false; 
bool oldValue2              = false;
unsigned long lastNegFlank2;
unsigned long lastPosFlank2;

int powerUtility;                                           //measured power values & update flags / counters & suppression of first update
int errorUtility               = false;                     //error condition are not reversible and require a reset
bool powerUpdateUtility        = false;
unsigned long nextUpdateUtility         = 0;
bool killUpdate             = false;

int powerHeating;
bool errorHeating           = false;
bool powerUpdateHeating     = false;
unsigned long nextUpdateHeating      = 0;
bool killUpdate1            = false;

int powerPV;
bool errorPV                = false;
bool powerUpdatePV          = false;
unsigned long nextUpdatePV           = 0;
bool killUpdate2            = false;

unsigned long energyUtility           = 1000;
unsigned long energyHeat              = 0;
unsigned long energyPV                = 0;
unsigned long lastEnergyUtility       = 0;
unsigned long lastEnergyHeat          = 0;
unsigned long lastEnergyPV            = 0;
const unsigned long     energyConstant  = 1000;                  // mWh
const unsigned long     energyConstant1 = 500;                   // mWh
const unsigned long     energyConstant2 = 500;                   // mWh

void setup_s0(){
  //setup pins
  pinMode(sensorPin, INPUT_PULLUP);
  pinMode(sensorPin1, INPUT_PULLUP);
  pinMode(sensorPin2, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

    //needed to supress wrong messages at startup
  lastNegFlank  = millis();
  lastNegFlank1  = millis();
  lastNegFlank2  = millis();
  
  Serial.println("===============================");
  Serial.println("Setting S0 sensors.");
  Serial.println("===============================");
}


//called form main loop
void update_s0(){
//get timestamp
  now               = millis();
  
  //reset update flags
  powerUpdateUtility    = false;
  powerUpdatePV         = false;
  powerUpdateHeating    = false;
  
  // read the value from the sensor, LED for debbuging
  oldValue          = sensorValue;
  sensorValue       = !digitalRead(sensorPin);
  oldValue1          = sensorValue1;
  sensorValue1       = digitalRead(sensorPin1);
  oldValue2          = sensorValue2;
  sensorValue2       = digitalRead(sensorPin2);
  digitalWrite(ledPin, sensorValue);
  
  
  //Serial.print(sensorValue);
  //Serial.print(sensorValue1);
  //Serial.println(sensorValue2);
  
  //detect flanks
  bool negFlank     = oldValue & !sensorValue;
  bool posFlank     = !oldValue & sensorValue;
  bool negFlank1    = oldValue1 & !sensorValue1;
  bool posFlank1    = !oldValue1 & sensorValue1;
  bool negFlank2    = oldValue2 & !sensorValue2;
  bool posFlank2    = !oldValue2 & sensorValue2;

  //a positive flank just gets stored
  if (posFlank){
    lastPosFlank            = now;
    nextUpdateUtility       = now;
    energyUtility           += energyConstant;
  }
  if (posFlank1){
    lastPosFlank1       = now;
    nextUpdateHeating   = now;
    energyHeat          += energyConstant1;
  }
  if (posFlank2){
    lastPosFlank2       = now;
    nextUpdatePV        = now;
    energyPV          += energyConstant2;
  }

   //a short pulse (<500ms) signals kWh increment
  if (negFlank & (now - lastPosFlank < 500)){
    powerUtility          = meterConstant/(now - lastNegFlank);
    lastNegFlank          = now;
    powerUpdateUtility    = killUpdate;     //supress first update
    killUpdate            = true;
  }
  if (negFlank1 & (now - lastPosFlank1 < 500)){
    powerHeating          = meterConstant1/(now - lastNegFlank1);
    lastNegFlank1         = now;
    powerUpdateHeating    = killUpdate1;    //supress first update
    killUpdate1           = true;
  }
  if (negFlank2 & (now - lastPosFlank2 < 500)){
    powerPV               = meterConstant2/(now - lastNegFlank2);
    lastNegFlank2         = now;
    powerUpdatePV         = killUpdate2;    //supress first update
    killUpdate2           = true;
  }

  //a permanent on (>500ms) signals 0 kWh (Utility counter only)
  if (sensorValue & (now - lastPosFlank > 500)){
    powerUtility = 0;
    powerUpdateUtility     = nextUpdateUtility < now;
    if (powerUpdateUtility) nextUpdateUtility = now + 5000;   
  }

  //force update after 90 sec (<20W) // 200 sec (<45W) (single phase counter only)
  if (!sensorValue1 & (now - lastNegFlank1 > 90000)){
    powerHeating            = 0;
    powerUpdateHeating      = nextUpdateHeating < now;
    if (powerUpdateHeating) nextUpdateHeating = now + 90000;   
  }
  if (!sensorValue2 & (now - lastNegFlank2 > 90000)){
    powerPV           = 0;
    powerUpdatePV     = nextUpdatePV < now;
    if (powerUpdatePV) nextUpdatePV = now + 90000;   
  }
  
  //a permanent on (> 2s) could be an error condition (single phase counter/ eastron)
  if (sensorValue1 & (now - lastPosFlank1 > 2000)){
    errorHeating          = true;
    powerUpdateHeating    = nextUpdateHeating < now;
    if (powerUpdateHeating) nextUpdateHeating = now + 90000;   
  }
  if (sensorValue2 & (now - lastPosFlank2 > 2000)){
    errorPV               = true;
    powerUpdatePV         = nextUpdatePV < now;
    if (powerUpdatePV) nextUpdatePV = now + 90000;   
  }
}




void print_s0_info(){
  Serial.println("=============S0 INFO===============");
   
  Serial.print("Utility: ");
  Serial.print(powerUtility);
  Serial.print(" ");
  Serial.println(errorUtility);

  Serial.print("PV: ");
  Serial.print(powerPV);
  Serial.print(" ");
  Serial.println(errorPV);

  Serial.print("Heating: ");
  Serial.print(powerHeating);
  Serial.print(" ");
  Serial.println(errorHeating);
  
  Serial.println("===================================");
}


void new_day_S0(){
  lastEnergyUtility       = energyUtility;
  lastEnergyHeat          = energyHeat;
  lastEnergyPV            = energyPV;
  energyUtility           = 0;
  energyHeat              = 0;
  energyPV                = 0; 
}











  
