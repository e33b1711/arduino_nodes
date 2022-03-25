const int pwmPin                      = 5;
const float max_temp                  = 85;
const unsigned long time_out          = 10000;
unsigned long heatingLastUpdate; 
int pwm_setpoint                      = 0;


void setup_heating(){
    Serial.println("===============================");
    pinMode(pwmPin, OUTPUT);
    analogWrite(pwmPin, 0);

    Serial.println("Setting up heating.");
    /*
    Serial.print("Switching on...");
    analogWrite(pwmPin, 150);
    delay(4000);
    Serial.println(" and off.");
    analogWrite(pwmPin, 0);
    */
    heatingLastUpdate = millis();
    Serial.println("===============================");
}

void heat_off(){
  pinMode(pwmPin, OUTPUT);
  analogWrite(pwmPin, 0);
}

void set_pwm(int val){
    heatingLastUpdate = millis();
    pwm_setpoint = val;  
}


//called form main loop
void update_heating(){

    //overheat protection
    if ((pwm_setpoint > 0) and (tempHigh > max_temp)){
        Serial.println("ERROR: Overtemperature. Setting heating to 0!");
        pwm_setpoint = 0;
    }

    //watchdog
    //if ((pwm_setpoint > 0) and (heatingLastUpdate+time_out<millis()) ){
    //    Serial.println("ERROR: pwm update too old!");
    //    pwm_setpoint = 0;
    //}
  
    //write pwm
    analogWrite(pwmPin, pwm_setpoint);
}


void print_heating_info(){
    Serial.print("setpoint_pwm: ");
    Serial.println(pwm_setpoint);
}
