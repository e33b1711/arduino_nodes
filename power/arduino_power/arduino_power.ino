
const int sensorPin         = 4;                        // digital input of the power meter pulses (Utility power, only consumption)
const int sensorPin1        = 5;                        // power to electric heating
const int sensorPin2        = 6;                        // power from PV
const int sensorPin3        = 8;                        // power to utility (eastron)
const int sensorPin4        = 9;                        // power from utility (estron)
const int ledPin            = 13;                       // LED output pin

const double meterConstant  = 3600000;                  // 1 mWs
const double meterConstant1 = 1800000;                  // 0.5 mWs
const double meterConstant2 = 1800000;                  // "
const double meterConstant3 = 9000000;                  // 2.5 mWs
const double meterConstant4 = 9000000;                  // "

const int heatControlPin    = 11;                       // pwm output for controlling the power of the electric heating

const int maxControlPower   = 3000;                     // 100% Power of Heating
const double powerIncrement = maxControlPower / 256;    // one PWM step of heating Power
const int acceptablePower   = 6;                        // acceptebale power drawn from Utility during electric heating

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

bool sensorValue3           = false; 
bool oldValue3              = false;
long lastNegFlank3;
long lastPosFlank3;

bool sensorValue4           = false; 
bool oldValue4              = false;
long lastNegFlank4;
long lastPosFlank4;

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

int powerEx;
bool errorEx                = false;
bool powerUpdateEx          = false;
long nextUpdateEx           = 0;
bool killUpdate3            = false;

int powerIm;
bool errorIm                = false;
bool powerUpdateIm          = false;
long nextUpdateIm           = 0;
bool killUpdate4            = false;

int controlPower            = 0;                         //variables for the control algorithm
bool powerUpdateControl     = false;                     //control power is in watt / powerIncrement
int errorControl            = false;
int incrUp                  = 0;

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

  //get timestamp
  now               = millis();
  
  //reset update flags
  powerUpdateUtility    = false;
  powerUpdatePV         = false;
  powerUpdateHeating    = false;
  powerUpdateControl    = false;
  powerUpdateEx         = false;
  powerUpdateIm         = false;
  
  // read the value from the sensor, LED for debbuging
  oldValue          = sensorValue;
  sensorValue       = !digitalRead(sensorPin);
  oldValue1          = sensorValue1;
  sensorValue1       = digitalRead(sensorPin1);
  oldValue2          = sensorValue2;
  sensorValue2       = digitalRead(sensorPin2);
  oldValue3          = sensorValue3;
  sensorValue3       = digitalRead(sensorPin3);
  oldValue4          = sensorValue4;
  sensorValue4       = digitalRead(sensorPin4);
  digitalWrite(ledPin, sensorValue);

  //detect flanks
  bool negFlank     = oldValue & !sensorValue;
  bool posFlank     = !oldValue & sensorValue;
  bool negFlank1    = oldValue1 & !sensorValue1;
  bool posFlank1    = !oldValue1 & sensorValue1;
  bool negFlank2    = oldValue2 & !sensorValue2;
  bool posFlank2    = !oldValue2 & sensorValue2;
  bool negFlank3    = oldValue3 & !sensorValue3;
  bool posFlank3    = !oldValue3 & sensorValue3;
  bool negFlank4    = oldValue4 & !sensorValue4;
  bool posFlank4    = !oldValue4 & sensorValue4;

  //a positive flank just gets stored
  if (posFlank){
    lastPosFlank        = now;
    nextUpdateUtility      = now;
  }
  if (posFlank1){
    lastPosFlank1       = now;
    nextUpdateHeating   = now;
  }
  if (posFlank2){
    lastPosFlank2       = now;
    nextUpdatePV        = now;
  }
  if (posFlank3){
    lastPosFlank3       = now;
    nextUpdateEx        = now;
  }
  if (posFlank4){
    lastPosFlank4       = now;
    nextUpdateIm        = now;
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
  if (negFlank3 & (now - lastPosFlank3 < 500)){
    powerEx               = meterConstant3/(now - lastNegFlank3);
    lastNegFlank3         = now;
    powerUpdateEx         = killUpdate3;    //supress first update
    killUpdate3           = true;
  }
  if (negFlank4 & (now - lastPosFlank4 < 500)){
    powerIm               = meterConstant4/(now - lastNegFlank4);
    lastNegFlank4         = now;
    powerUpdateIm         = killUpdate4;    //supress first update
    killUpdate4           = true;
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
    if (!sensorValue3 & (now - lastNegFlank3 > 200000)){
    powerEx           = 0;
    powerUpdateEx     = nextUpdateEx < now;
    if (powerUpdateEx) nextUpdateEx = now + 200000;   
  }
  if (!sensorValue4 & (now - lastNegFlank4 > 200000)){
    powerIm = 0;
    powerUpdateIm     = nextUpdateIm < now;
    if (powerUpdateIm) nextUpdateIm = now + 90000;   
  }

  /*
  //a permanent off (> 600s) could be an error condition (Utility counter only)
  if (!sensorValue & (now - lastNegFlank > 600000)){
    errorUtility   = true;
    powerUpdateUtility   = nextUpdateUtility < now;
    if (powerUpdateUtility) nextUpdateUtility    = now + 5000;   
  }
  */
  
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
  if (sensorValue3 & (now - lastPosFlank3 > 2000)){
    errorEx          = true;
    powerUpdateEx    = nextUpdateEx < now;
    if (powerUpdateEx) nextUpdateEx = now + 90000;   
  }
  if (sensorValue4 & (now - lastPosFlank4 > 2000)){
    errorIm               = true;
    powerUpdateIm         = nextUpdateIm < now;
    if (powerUpdateIm) nextUpdateIm = now + 90000;   
  }

  //control algorithm, calculate update
  if (powerUpdateUtility){
    //calculate controlPower update
    int nxtControlPower = controlPower;
    if (powerUtility == 0){
      incrUp++;
      nxtControlPower+=incrUp;
    }
    if (powerUtility > 0){
      incrUp=0;
      int powerDecrement  =  (powerUtility - acceptablePower) / powerIncrement;
      nxtControlPower = controlPower - powerDecrement; 
    }
    if (nxtControlPower<0) nxtControlPower = 0;
    //if there is a error on the Utility sensor, we got also an error on the control
    if (errorUtility || errorControl){
      nxtControlPower = 0;
      errorControl    = true;
    }
    //we got a update
    powerUpdateControl      = true;
    controlPower            = nxtControlPower;    
  }

  //control algorithm, check with measured power
  if (powerUpdateHeating){
    int powerDiff           = powerHeating - int(controlPower*powerIncrement);
    if(powerDiff > 200 ||  powerDiff < -200){
      //if the difference is too big we got an error
      controlPower          = 0;
      errorControl          = true;
      powerUpdateControl    = true;
    }
  }

  //control algorithm, check with measured power
  if (powerUpdateHeating){
    int powerDiff           = powerHeating - int(controlPower*powerIncrement);
    if(powerDiff > 200 ||  powerDiff < -200){
      //if the difference is too big we got an error
      controlPower          = 0;
      errorControl          = true;
      powerUpdateControl    = true;
    }
  }

  //control algorithm, check with pv power
  if (powerUpdateHeating){
    int powerDiff           = int(controlPower*powerIncrement) - powerPV;
    if(powerDiff > 100){
      //if the difference is bigger than zero (plus tolerance) we got an errror
      controlPower            = 0;
      errorControl            = true;
      powerUpdateControl      = true;
    }
  }


  //update the pmw output
  if (powerUpdateControl){
    //write control and enable
    analogWrite(heatControlPin, controlPower);
  }

  //serial outputs
  if (powerUpdateControl){
    Serial.print("powerControl:");
    Serial.println(int(controlPower*powerIncrement));
    Serial.print("errorControl:");
    Serial.println(errorControl); 
  }
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
  if (powerUpdateHeating){
    Serial.print("powerHeating:");
    Serial.println(powerHeating);   
    Serial.print("errorHeating:");
    Serial.println(errorHeating);  
  }
  if (powerUpdateEx){
    Serial.print("powerEx:");
    Serial.println(powerEx); 
    Serial.print("errorEx:");
    Serial.println(errorEx);    
  }
  if (powerUpdateIm){
    Serial.print("powerIm:");
    Serial.println(powerIm);   
    Serial.print("errorIm:");
    Serial.println(errorIm);  
  }

}
