/*
  Analog Input

  Demonstrates analog input by reading an analog sensor on analog pin 0 and
  turning on and off a light emitting diode(LED) connected to digital pin 13.
  The amount of time the LED will be on and off depends on the value obtained
  by analogRead().

  The circuit:
  - potentiometer
    center pin of the potentiometer to the analog input 0
    one side pin (either one) to ground
    the other side pin to +5V
  - LED
    anode (long leg) attached to digital output 13
    cathode (short leg) attached to ground

  - Note: because most Arduinos have a built-in LED attached to pin 13 on the
    board, the LED is optional.

  created by David Cuartielles
  modified 30 Aug 2011
  By Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/AnalogInput
*/

int sensorPin = A2;    // select the input pin for the potentiometer
int ledPin = 13;      // select the pin for the LED
int sensorValue = 400;  // variable to store the value coming from the sensor
int oldValue    = 0;
int count       = 0;
long lastTime   = millis();
long pfTime     = millis();
bool stop_flag  = false;

void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);

   Serial.begin(9600);
   //Serial.println("Started...");
}

void loop() {
  // read the value from the sensor:
  oldValue    = sensorValue;
  sensorValue = analogRead(sensorPin);

  // measure time between negative flanks
  if((sensorValue > 300) && (oldValue < 300) ){
    long actTime = millis();
    long diffTime = actTime - lastTime;
    lastTime = actTime;
    //Serial.println(diffTime);
    Serial.print("P_grid:");
    Serial.println(3600000/diffTime);
    //Serial.println(" kW");
  }

  //check if led is constant on => 0 kW
  //first get time of positive flank
  if((sensorValue < 300) && (oldValue > 300) ){
    pfTime = millis();
    stop_flag = false;
  }
  //if led is on longer than 500 ms => 0 kW
  if( (sensorValue < 300) && (pfTime + 500 < millis()) ){
    if (!stop_flag){
      Serial.println("P_grid:0");
      stop_flag = true;
    }
  }

  //delay a little
  delay(10);

}
