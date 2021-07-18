const int sensorPin1        = 3;                        // power to electric heating
const int sensorPin2        = 19;                        // power from PV

//Number of pulses, used to measure energy.

long pulseCount1 = 0;
long pulseCount2 = 0;

const float powerConstant1 = 1800000;                  // 0.5 mWs
const float powerConstant2 = 1800000;                  // "

const float energyConstant1 = 0.0005;                  // 0.5 mWs
const float energyConstant2 = 0.0005;                  // "


//Used to measure power.
unsigned long pulseTime1,lastTime1, timeout1;
unsigned long pulseTime2,lastTime2, timeout2;
float powerUtility, powerHeat, powerPV = 0;


void setup_s0(){
    Serial.println("===============================");
    Serial.println("Setting up S0.");
    // KWH interrupt attached to IRQ 1 = pin3
    attachInterrupt(digitalPinToInterrupt(sensorPin1), onPulse1, RISING);
    attachInterrupt(digitalPinToInterrupt(sensorPin2), onPulse2, RISING);
    timeout1 = millis();
    timeout2 = millis();
    Serial.println("done.");
    Serial.println("===============================");
}

void update_s0(){
    /*
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
void onPulse1(){
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
void onPulse2(){
    //used to measure time between pulses.
    lastTime2 = pulseTime2;
    pulseTime2 = millis();
    timeout2 = pulseTime2;
    //pulseCounter
    pulseCount2++;
    powerPV = (powerConstant2 / (pulseTime2 - lastTime2));
    //Serial.println("...");
}


void print_s0_info(){
    Serial.print("powerPV: ");
    Serial.println(powerPV,3);
    Serial.print("powerHeat: ");
    Serial.println(powerHeat,3);
}










  
