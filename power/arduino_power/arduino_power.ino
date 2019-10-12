
const int sensorPin         = 4;          // digital input of the power meter pulses
const int sensorPin1        = 5;
const int sensorPin2        = 6;
const int ledPin            = 13;         // LED output pin

const int heatControlPin    = 11;         // analog output for the triac control

const int maxControlPower   = 3000;                    // 100% Power of Heating
const double powerIncrement = maxControlPower / 256;   // one PWM step of heating Power
const int acceptablePower   = 6;                       // acceptale Heating Draw from Grid 

long now;

bool sensorValue          = false; 
bool oldValue             = false;
long lastNegFlank;
long lastPosFlank;

bool sensorValue1          = false; 
bool oldValue1             = false;
long lastNegFlank1;
long lastPosFlank1;

bool sensorValue2          = false; 
bool oldValue2             = false;
long lastNegFlank2;
long lastPosFlank2;

double  powerGrid;
bool powerUpdateGrid          = false;
long nextUpdateGrid;
bool killUpdate     = false;


double  powerHeating;
bool powerUpdateHeating          = false;
long nextUpdateHeating;
bool killUpdate1     = false;

double  powerPV;
bool powerUpdatePV          = false;
long nextUpdatePV;
bool killUpdate2     = false;

int controlPower = 0;
long offTime;
bool systemError = false; 
int incrUp       = 0;

void setup() {
  
  //setup pins
  pinMode(sensorPin, INPUT_PULLUP);
  
  pinMode(ledPin, OUTPUT);
  pinMode(heatControlPin, OUTPUT);

  //setup uart
  Serial.begin(9600);

  //needed to supress wrong messages at startup
  lastNegFlank  = millis();
  lastNegFlank1  = millis();
  lastNegFlank2  = millis();
  
}

void loop() {
  // read the value from the sensor, false = LED off / true = LED on !!!!!!
  now               = millis();
  
  oldValue          = sensorValue;
  sensorValue       = !digitalRead(sensorPin);

  oldValue1          = sensorValue1;
  sensorValue1       = digitalRead(sensorPin1);
  digitalWrite(ledPin, sensorValue);

  //detect flanks and store times
  bool negFlank     = oldValue & !sensorValue;
  bool posFlank     = !oldValue & sensorValue;
   bool negFlank1     = oldValue1 & !sensorValue1;
  bool posFlank1     = !oldValue1 & sensorValue1;
    bool negFlank2     = oldValue2 & !sensorValue2;
  bool posFlank2     = !oldValue2 & sensorValue2;

  //a positive Flank just gets stored
  if (posFlank){
    lastPosFlank    = now;
    nextUpdateGrid      = now;
  }
  if (posFlank1){
    lastPosFlank1    = now;
    nextUpdateHeating      = now;
  }
  if (posFlank2){
    lastPosFlank2    = now;
    nextUpdatePV      = now;
  }

  //reset powerUpdate flag
   powerUpdateGrid  = false;
   powerUpdatePV  = false;
   powerUpdateHeating  = false;

  //a short pulse (<500ms) signals a 1/1000th of a kWh / 1/2000th of a kWh (grid / single phase counter)
  if (negFlank & (now - lastPosFlank < 500)){
    powerGrid             = 3600000.0/(now - lastNegFlank);
    lastNegFlank      = now;
    powerUpdateGrid       = killUpdate;
    killUpdate  = true;
  }
  if (negFlank1 & (now - lastPosFlank1 < 500)){
    powerHeating             = 1800000.0/(now - lastNegFlank1);
    lastNegFlank1      = now;
    powerUpdateHeating       = killUpdate1;
    killUpdate1       =true;
  }
  if (negFlank2 & (now - lastPosFlank2 < 500)){
    powerPV             = 1800000.0/(now - lastNegFlank2);
    lastNegFlank2      = now;
    powerUpdateHeating       = killUpdate2;
    killUpdate2 = true;
  }

  //a permanent on (>500ms) signals 0 kWh (grid counter)
  if (sensorValue & (now - lastPosFlank > 500)){
    powerGrid = 0;
    powerUpdateGrid     = nextUpdateGrid < now;
    if (powerUpdateGrid) nextUpdateGrid = now + 5000;   
  }

  //force update after 6 minutes (single phase counter)
  if (!sensorValue1 & (now - lastNegFlank1 > 360000)){
    powerHeating = 0;
    powerUpdateHeating     = nextUpdateHeating < now;
    if (powerUpdateHeating) nextUpdateHeating = now + 5000;   
  }
  if (!sensorValue2 & (now - lastNegFlank2 > 360000)){
    powerPV = 0;
    powerUpdatePV     = nextUpdatePV < now;
    if (powerUpdatePV) nextUpdatePV = now + 5000;   
  }


  //a permanent off (> 600s) could be an error condition (grid counter)
  if (!sensorValue & (now - lastNegFlank > 600000)){
    systemError   = true;
    powerUpdateGrid   = nextUpdateGrid < now;
    if (powerUpdateGrid) nextUpdateGrid    = now + 5000;   
  }

  //control algorithm
  if (powerUpdateGrid){
    
    //calculate controlPower update
    int nxtControlPower = controlPower;
    if (powerGrid == 0){
      incrUp++;
      nxtControlPower+=incrUp;
    }
    if (powerGrid > 0){
      incrUp=0;
      int powerDecrement  =  (powerGrid - acceptablePower) / powerIncrement;
      nxtControlPower = controlPower - powerDecrement; 
    }
    if (nxtControlPower<0) nxtControlPower = 0;
    if (nxtControlPower>=255) systemError = true;
    if (systemError)  nxtControlPower = 0;

    //if controlPower becomes 0, get the time
    if (controlPower>0 & nxtControlPower==0) offTime = now;
    controlPower = nxtControlPower;

    //write control and enable
    analogWrite(heatControlPin, controlPower); 

    
  }

  if (powerUpdateGrid){
    Serial.print("P_grid:");
    Serial.println(int(powerGrid));
    Serial.print("P_control:");
    Serial.println(int(controlPower*powerIncrement));
    Serial.print("system_error:");
    Serial.println(systemError); 
  }

  if (powerUpdatePV){
    Serial.print("P_pv:");
    Serial.println(int(powerPV));    
  }

  if (powerUpdateHeating){
    Serial.print("P_heat:");
    Serial.println(int(powerHeating));    
  }

  //use heating and pv power for error checking and control!!!!!!!!!!!

  //delay a little
  delay(10);

}
