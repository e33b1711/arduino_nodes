const int sensorPin0         = 2;                        // digital input of the power meter pulses (Utility power, only consumption)
const int sensorPin1        = 3;                        // power to electric heating
const int sensorPin2        = 19;                        // power from PV


//Number of pulses, used to measure energy.
long pulseCount0 = 0;
long pulseCount1 = 0;
long pulseCount2 = 0;

const long powerConstant0 = 3600000;                  // 1 mWs
const long powerConstant1 = 1800000;                  // 0.5 mWs
const long powerConstant2 = 1800000;                  // "

const long energyConstant0 = 1;                  // 1 mWs
const long energyConstant1 = 2;                  // 0.5 mWs
const long energyConstant2 = 2;                  // "

//Used to measure power.
unsigned long pulseTime0,lastTime0, timeout0;
unsigned long pulseTime1,lastTime1, timeout1;
unsigned long pulseTime2,lastTime2, timeout2;

//power and energy
unsigned long powerUtility, energyImport, lastEnergyImport  = 0;
unsigned long powerHeat, energyHeat, lastEnergyHeat           = 0;
unsigned long powerPV, energyPV, lastEnergyPV                 = 0;

//Number of pulses per wh - found or set on the meter.
int ppwh = 1; //1000 pulses/kwh = 1 pulse per wh

void setup_s0()
{

    // KWH interrupt attached to IRQ 1 = pin3
    attachInterrupt(digitalPinToInterrupt(sensorPin0), onPulse0, RISING);
    attachInterrupt(digitalPinToInterrupt(sensorPin1), onPulse1, RISING);
    attachInterrupt(digitalPinToInterrupt(sensorPin2), onPulse2, RISING);
    timeout0 = millis();
    timeout1 = millis();
    timeout2 = millis();
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
    unsigned long  power = (powerConstant0 / (timeout0 - lastTime0));
    if (power < powerUtility){
      powerUtility = power;
    }
    //Serial.println(">>>>>>>Timeout 0");
  }

  //force update every 4 sec (simulation a pulse)
  if (millis() - timeout1 > 4000){
    timeout1 = millis();
    unsigned long  power = (powerConstant1 / (timeout1 - lastTime1));
    if (power < powerHeat){
      powerHeat = power;
    }
    //Serial.println(">>>>>>>Timeout 1");
  }
   if (millis() - timeout2 > 4000){
    timeout2 = millis();
    unsigned long  power = (powerConstant2 / (timeout2 - lastTime2));
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
    //Calculate power
    powerUtility = (powerConstant0 / (pulseTime0 - lastTime0));
    //Find kwh elapsed
    energyExport = pulseCount0 / energyConstant0;
    Serial.println(".");
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
    //Calculate power
    powerHeat = (powerConstant1 / (pulseTime1 - lastTime1));
    //Find kwh elapsed
    energyHeat = pulseCount1 / energyConstant1;
    Serial.println("..");
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
    //Calculate power
    powerPV = (powerConstant2 / (pulseTime2 - lastTime2));
    //Find kwh elapsed
    energyPV = pulseCount2 / energyConstant2;
    Serial.println("...");
}




void print_s0_info(){
  Serial.println("=============S0 INFO===============");
   
  Serial.print("Utility: ");
  Serial.println(powerUtility);

  Serial.print("PV: ");
  Serial.println(powerPV);

  Serial.print("Heating: ");
  Serial.println(powerHeat);

  
  Serial.println("===================================");
}


void new_day_S0(){
  lastEnergyExport        = energyExport;
  lastEnergyHeat          = energyHeat;
  lastEnergyPV            = energyPV;
  energyImport            = 0;
  energyHeat              = 0;
  energyPV                = 0; 
}











  
