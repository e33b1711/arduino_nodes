const int sensorPin0         = 2;                        // digital input of the power meter pulses (Utility power, only consumption)
const int sensorPin1        = 3;                        // power to electric heating
const int sensorPin2        = 19;                        // power from PV


//Number of pulses, used to measure energy.
long pulseCount0 = 0;
long pulseCount1 = 0;
long pulseCount2 = 0;

const float powerConstant0 = 3600000;                  // 1 mWs
const float powerConstant1 = 1800000;                  // 0.5 mWs
const float powerConstant2 = 1800000;                  // "

const float energyConstant0 = 0.001;                  // 1 mWs
const float energyConstant1 = 0.0005;                  // 0.5 mWs
const float energyConstant2 = 0.0005;                  // "

//Used to measure power.
unsigned long pulseTime0,lastTime0, timeout0;
unsigned long pulseTime1,lastTime1, timeout1;
unsigned long pulseTime2,lastTime2, timeout2;

float powerUtility, powerHeat, powerPV = 0;

void setup_s0()
{
    Serial.println("===============================");
    Serial.println("Setting up S0.");
    // KWH interrupt attached to IRQ 1 = pin3
    attachInterrupt(digitalPinToInterrupt(sensorPin0), onPulse0, RISING);
    attachInterrupt(digitalPinToInterrupt(sensorPin1), onPulse1, RISING);
    attachInterrupt(digitalPinToInterrupt(sensorPin2), onPulse2, RISING);
    timeout0 = millis();
    timeout1 = millis();
    timeout2 = millis();
    Serial.println("done.");
    Serial.println("===============================");
}

void update_s0()
{
    /*
    Serial.print("powerUtility: ");
    Serial.println(powerUtility);
    Serial.print("energyUtility: ");
    Serial.println(energyUtility);
    Serial.print("powerHeat: ");
    Serial.println(powerHeat);
    Serial.print("energyHeat: ");
    Serial.println(energyHeat);
    Serial.print("powerPV: ");
    Serial.println(powerPV);
    Serial.print("energyPV: ");
    Serial.println(energyPV);
    Serial.println("==============================");
    delay(4000);
    */

  
    //a permanent on (>500ms) signals 0 kWh (Utility counter only)
  if (!digitalRead(sensorPin0) & (millis() - lastTime0 > 500)){
    powerUtility = 0; 
  }
  //force update every 10 sec (simulation a pulse)
  if (millis() - timeout0 > 4000){
    timeout0 = millis();
    float  power = (powerConstant0 / (timeout0 - lastTime0));
    if (power < powerUtility){
      powerUtility = power;
    }
    //Serial.println(">>>>>>>Timeout 0");
  }

  //force update every 4 sec (simulation a pulse)
  if (millis() - timeout1 > 4000){
    timeout1 = millis();
    float power = (powerConstant1 / (timeout1 - lastTime1));
    if (power < powerHeat){
      powerHeat = power;
    }
    //Serial.println(">>>>>>>Timeout 1");
  }
   if (millis() - timeout2 > 4000){
    timeout2 = millis();
    float  power = (powerConstant2 / (timeout2 - lastTime2));
    if (power < powerPV){
      powerPV = power;
    }
    //Serial.println(">>>>>Timeout 2");
  }

}

// The interrupt routine
void onPulse0()
{
    //used to measure time between pulses.
    lastTime0 = pulseTime0;
    pulseTime0 = millis();
    timeout0 = pulseTime0;
    //pulseCounter
    pulseCount0++;
    powerUtility = (powerConstant0 / (pulseTime0 - lastTime0));
    //Serial.println(".");
}

// The interrupt routine
void onPulse1()
{
    //used to measure time between pulses.
    lastTime1 = pulseTime1;
    pulseTime1 = millis();
    timeout1 = pulseTime1;
    //pulseCounter
    pulseCount1++;
    powerHeat = (powerConstant1 / (pulseTime1 - lastTime1));
    //Serial.println("..");
}

// The interrupt routine
void onPulse2()
{
    //used to measure time between pulses.
    lastTime2 = pulseTime2;
    pulseTime2 = millis();
    timeout2 = pulseTime2;
    //pulseCounter
    pulseCount2++;
    powerPV = (powerConstant2 / (pulseTime2 - lastTime2));
    //Serial.println("...");
}

float energyImport(){
  return energyConstant0 * pulseCount0;
}
float energyHeat(){
  return energyConstant1 * pulseCount1;
}
float energyPV(){
  return energyConstant2 * pulseCount2;
}



void print_s0_info(){
  Serial.println("=============S0 INFO===============");
   
  Serial.print("powerUtility: ");
  Serial.println(powerUtility,3);

  Serial.print("powerPV: ");
  Serial.println(powerPV,3);

  Serial.print("powerHeat: ");
  Serial.println(powerHeat,3);

  Serial.print("energyImport: ");
  Serial.println(energyImport(),3);

  Serial.print("energyPV: ");
  Serial.println(energyPV(),3);

  Serial.print("energyHeat: ");
  Serial.println(energyHeat(),3);

  
  Serial.print("pulseCount0: ");
  Serial.println(pulseCount0);

  Serial.print("pulseCount1: ");
  Serial.println(pulseCount1);

  Serial.print("pulseCount2: ");
  Serial.println(pulseCount2);
  
  Serial.println("===================================");
}










  
