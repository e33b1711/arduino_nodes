
const int sensorPin         = 12;          // digital input of the power meter pulses
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
  //pinMode(sensorPin, INPUT_PULLUP);
  
  pinMode(ledPin, OUTPUT);
  //digitalWrite(heatEnablePin, LOW);
  //pinMode(heatEnablePin, OUTPUT);
  //pinMode(heatControlPin, OUTPUT);

  //setup uart
  Serial.begin(9600);

  //needed to supress wrong messages at startup
  //lastNegFlank  = millis();
  //offTime       = millis() - 60000;
  
}

void loop() {

  Serial.println(digitalRead(sensorPin));

  //delay a little
  delay(50);

}
