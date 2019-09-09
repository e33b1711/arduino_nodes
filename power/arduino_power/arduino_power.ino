
const int sensorPin         = 4;          // digital input of the power meter pulses
const int ledPin            = 13;         // LED output pin

const int heatEnablePin     = 77;         // enable output for the triac 
const int heatControlPin    = 88;         // analog output for the triac control

const int maxControlPower   = 6000;       // 100% Power of Heating
const double powerIncrement = 6000 / 256; // one PWM step of heating Power
const int acceptablePower   = 6;          // acceptale Heating Draw from Grid 

bool sensorValue          = false; 
bool oldValue             = false;
long now;
long recentFlank;
long lastNegFlank;
long lastPosFlank;

double  power;
bool powerUpdate          = false;
long nextUpdate;
bool notFirstNegFlank     = false;

int controlPower = 0;
long offTime;

void setup() {
  
  //setup pins
  pinMode(sensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(heatEnablePin, LOW);
  pinMode(heatEnablePin, OUTPUT);
  pinMode(heatControlPin, OUTPUT);

  //setup uart
  Serial.begin(9600);

  //needed to supress wrong messages at startup
  nextUpdate = millis();
  lastNegFlank = millis();
  
}

void loop() {
  // read the value from the sensor, false = LED off / true = LED on !!!!!!
  now               = millis();
  oldValue          = sensorValue;
  sensorValue       = digitalRead(sensorPin);

  //detect flanks and store times
  bool negFlank     = oldValue & !sensorValue;
  bool posFlank     = !oldValue & sensorValue;
  recentFlank       = now;

  //a positive Flank just gets stored
  if (posFlank){
    lastPosFlank    = recentFlank;
  }

  //reset powerUpdate flag
   powerUpdate  = false;

  //a short pulse (<500ms) signals a 1/1000th of a kWh
  if (negFlank & (recentFlank - lastPosFlank < 500)){
    power             = 3600.0/(recentFlank - lastNegFlank);
    lastNegFlank      = recentFlank;
    powerUpdate       = notFirstNegFlank;
    notFirstNegFlank  = true;
  }

  //a permanent on (>500ms) signals 0 kWh

  if (sensorValue & (now - lastPosFlank > 500)){
    power = 0;
    powerUpdate     = nextUpdate < now;
    nextUpdate      = now + 5000;   
  }

  //a permanent off (> 60s) could be an error condition
  if (!sensorValue & (now - lastNegFlank > 60000)){
    power         = 99999;
    powerUpdate   = nextUpdate < now;
    nextUpdate    = now + 5000;   
  }

  //control algorithm
  if (powerUpdate){
    
    //calculate controlPower update
    int nxtControlPower = controlPower;
    if (power == 0){
      nxtControlPower++;
    }
    if (power > 0){
      int powerDecrement  =  (power - acceptablePower) / powerIncrement;
      nxtControlPower = controlPower - powerDecrement; 
    }
    if (nxtControlPower<0) controlPower = 0;
    if (nxtControlPower>255) controlPower = 255;

    //if controlPower becomes 0, get the time
    if (controlPower>0 & nxtControlPower==0) offTime = now;

    //write control and enable
    analogWrite(heatControlPin, controlPower); 
    digitalWrite(heatControlPin, (controlPower>0) || (now-offTime < 60000));

    //write uart
    Serial.print("P_grid:");
    Serial.println(int(power));
    Serial.print("P_heat:");
    Serial.println(int(controlPower*powerIncrement));
    
  }

  //delay a little
  delay(10);

}
