
const int sensorPin         = 4;                        // digital input of the power meter pulses (Utility power, only consumption)
const int sensorPin1        = 5;                        // power to electric heating
const int sensorPin2        = 6;                        // power from PV
const int ledPin            = 13;                       // LED output pin

const int pwmPin            = 9;
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
  
}

void loop() {

  while (Serial.available() > 0) {
    char inChar = Serial.read();

      inString += (char)inChar;
    
    // if you get a newline, print the string, then the string's value:
    if (inChar == '\n') {
      if (inString.substring(0,4)=="pwm:"){
        int val = inString.substring(4).toInt();
        Serial.print("setPWM:");
        Serial.println(val);
        analogWrite(pwmPin, val);
        watchDog = millis();
        // clear the string for new input:
      }
       inString = "";
    }
  }

  if (watchDog + 20000 < millis()){
     Serial.print("setPWM:");
        Serial.println(0);
        analogWrite(pwmPin, 0);
        watchDog = millis();
  }

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

  //detect flanks
  bool negFlank     = oldValue & !sensorValue;
  bool posFlank     = !oldValue & sensorValue;
  bool negFlank1    = oldValue1 & !sensorValue1;
  bool posFlank1    = !oldValue1 & sensorValue1;
  bool negFlank2    = oldValue2 & !sensorValue2;
  bool posFlank2    = !oldValue2 & sensorValue2;

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

}
