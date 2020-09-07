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


//called form main loop
void update_heating(){

    float pwm_set_point_new = 0;

    //try to get a modbus update and regulate the power setting
    if (heatingLastUpdate+h_update_period<millis()){
        //reset watchtdog timer
        heatingLastUpdate = millis();
      
        //default is off
        int pwm_setpoint_new= 0;

        //mode 1 control
        if (modbus_get_bal_power() and control_mode==1){
      
            // P-Controller with up/down weigth
            float power_diff = target_power-bal_power;
            //Serial.print("DEBUG: power_diff:"); Serial.println(power_diff);

            if (power_diff>c_hist){
            pwm_setpoint_new = (power_diff*c_up) + pwm_setpoint;
            }
            //Serial.print("DEBUG: (power_diff*c_up):"); Serial.println((power_diff*c_up));
            //Serial.print("DEBUG: pwm_setpoint_new:"); Serial.println(pwm_setpoint_new);
            if (power_diff<c_hist){
            pwm_setpoint_new = (power_diff*c_down) + pwm_setpoint;
            }
            //Serial.print("DEBUG: (power_diff*c_down):"); Serial.println((power_diff*c_down));
            //Serial.print("DEBUG: pwm_setpoint_new:"); Serial.println(pwm_setpoint_new);

            //saturation
            if (pwm_setpoint_new >c_pwm_max)  pwm_setpoint_new = c_pwm_max;
            if (pwm_setpoint_new <0)    pwm_setpoint_new = 0;
            //Serial.print("DEBUG: pwm_setpoint_new:"); Serial.println(pwm_setpoint_new);

        }

        //mode 2 set pwm direct
        if (control_mode==2){
            pwm_setpoint_new = int(target_power);
            //saturation
            if (pwm_setpoint_new >c_pwm_max)  pwm_setpoint_new = c_pwm_max;
            if (pwm_setpoint_new <0)    pwm_setpoint_new = 0;
        }
        pwm_setpoint = pwm_setpoint_new;
    }
  

  //watchdog
    if (heatingLastUpdate+time_out<millis()){
        watchdog_counter++;
        Serial.println("ERROR: Heating watchdog barked!");
        Serial.print("Watch dogcounter: ");
        Serial.println(watchdog_counter);
        heatingLastUpdate = millis();
        pwm_setpoint = 0;
    }

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
    Serial.print("target_power: ");
    Serial.println(target_power);
    Serial.print("control_mode: ");
    Serial.println(control_mode);
    Serial.print("PWM setpoint:  ");
    Serial.println(pwm_setpoint);
    Serial.print("Watch dog counter: ");
    Serial.println(watchdog_counter);
    Serial.println("===================================");
}
