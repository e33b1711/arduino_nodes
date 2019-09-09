
const int sensorPin         = 4;          // digital input of the power meter pulses
const int ledPin            = 13;         // LED output pin

const int heatEnablePin     = 10;         // enable output for the triac 
const int heatControlPin    = 11;         // analog output for the triac control

const int maxControlPower   = 6000;       // 100% Power of Heating
const double powerIncrement = 6000 / 256; // one PWM step of heating Power
const int acceptablePower   = 6;          // acceptale Heating Draw from Grid 

bool sensorValue          = false; 
bool oldValue             = false;
long now;
long lastNegFlank;
long lastPosFlank;

double  power;
bool powerUpdate          = false;
long nextUpdate;
bool notFirstNegFlank     = false;

int controlPower = 0;
long offTime;
bool systemError = false; 
int incrUp       = 0;

void setup() {
  
  //setup pins
  pinMode(sensorPin, INPUT_PULLUP);
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(heatEnablePin, LOW);
  pinMode(heatEnablePin, OUTPUT);
  pinMode(heatControlPin, OUTPUT);

  //setup uart
  Serial.begin(9600);

  //needed to supress wrong messages at startup
  lastNegFlank  = millis();
  offTime       = millis() - 60000;
  
}

void loop() {
  // read the value from the sensor, false = LED off / true = LED on !!!!!!
  now               = millis();
  oldValue          = sensorValue;
  sensorValue       = !digitalRead(sensorPin);
  digitalWrite(ledPin, sensorValue);

  //detect flanks and store times
  bool negFlank     = oldValue & !sensorValue;
  bool posFlank     = !oldValue & sensorValue;

  //a positive Flank just gets stored
  if (posFlank){
    lastPosFlank    = now;
    nextUpdate      = now;
  }

  //reset powerUpdate flag
   powerUpdate  = false;

  //a short pulse (<500ms) signals a 1/1000th of a kWh
  if (negFlank & (now - lastPosFlank < 500)){
    power             = 3600000.0/(now - lastNegFlank);
    lastNegFlank      = now;
    powerUpdate       = notFirstNegFlank;
    notFirstNegFlank  = true;
  }

  //a permanent on (>500ms) signals 0 kWh

  if (sensorValue & (now - lastPosFlank > 500)){
    power = 0;
    powerUpdate     = nextUpdate < now;
    if (powerUpdate) nextUpdate = now + 5000;   
  }

  //a permanent off (> 600s) could be an error condition
  if (!sensorValue & (now - lastNegFlank > 600000)){
    systemError   = true;
    powerUpdate   = nextUpdate < now;
    if (powerUpdate) nextUpdate    = now + 5000;   
  }

  //control algorithm
  if (powerUpdate){
    
    //calculate controlPower update
    int nxtControlPower = controlPower;
    if (power == 0){
      incrUp++;
      nxtControlPower+=incrUp;
    }
    if (power > 0){
      incrUp=0;
      int powerDecrement  =  (power - acceptablePower) / powerIncrement;
      nxtControlPower = controlPower - powerDecrement; 
    }
    if (nxtControlPower<0) nxtControlPower = 0;
    if (nxtControlPower>=255) systemError = true;
    if (systemError)  nxtControlPower = 0;

    //if controlPower becomes 0, get the time
    if (controlPower>0 & nxtControlPower==0) offTime = now;
    controlPower = nxtControlPower;

    //write control and enable
    bool h_enable = ( (controlPower>0) || (now-offTime < 60000) ) && !systemError;
    analogWrite(heatControlPin, controlPower); 
    digitalWrite(heatEnablePin, h_enable);

    //write uart
    Serial.print("P_grid:");
    Serial.println(int(power));
    Serial.print("P_heat:");
    Serial.println(int(controlPower*powerIncrement));
    Serial.print("heating_enable:");
    Serial.println(h_enable);
    Serial.print("system_error:");
    Serial.println(systemError);
    
  }

  //delay a little
  delay(10);

}
