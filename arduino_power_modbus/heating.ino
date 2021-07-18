const int pwmPin                      = 5;
unsigned long heatingLastUpdate;
const float c_down                    = 1.0/10;
const float c_up                      = 1.0/15;
const float c_hist                    = 25;   //hystereses
const float c_pwm_max                 = 240;
const float max_temp                  = 85;
const unsigned long time_out          = 10000; 
const unsigned long h_update_period   = 1000;
float target_power                    = -25;
int control_mode                      =  1; //0=off, 1=control balanced power, 2=control heat power
int pwm_setpoint                      = 0;
unsigned long watchdog_counter        = 0;
bool h_control_off                    = false;

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


//called form main loop
void update_heating(){

  

  //watchdog


    //overheat protection
    if (tempHigh > max_temp){
        Serial.println("ERROR: Overtemperature. Setting heating to 0!");
        pwm_setpoint = 0;
    }
  
    //write pwm
    analogWrite(pwmPin, pwm_setpoint);
}


void print_heating_info(){
    Serial.println("=============HEATING INFO==========");
    Serial.print("PWM setpoint:  ");
    Serial.println(pwm_setpoint);
    Serial.print("Watch dog counter: ");
    Serial.println(watchdog_counter);
    Serial.println("===================================");
}
