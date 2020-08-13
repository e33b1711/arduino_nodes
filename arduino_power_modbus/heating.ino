const int pwmPin                      = 4;
unsigned long heatingLastUpdate;
const float c_down                    = 1.0/10;
const float c_up                      = 1.0/15;
const float c_hist                    = 50;
const float c_pwm_max                 = 200;
const unsigned long time_out          = 10000; 
const unsigned long h_update_period   = 1000;
float bal_power                    = 0;
bool bal_power_valid                  = false;
int pwm_setpoint                      = 0;
unsigned long watchdog_counter        = 0;
bool h_control_off                    = false;

void setup_heating(){
  pinMode(pwmPin, OUTPUT);
  analogWrite(pwmPin, 0);
  Serial.println("===============================");
  Serial.println("Setting up heating.");
  Serial.println("===============================");
  heatingLastUpdate = millis();
}


//called form main loop
void update_heating(){

  int pwm_setpoint_new;

  //try to get a modbus update and regulate the power setting
  if (heatingLastUpdate+h_update_period<millis()){
    if (modbus_get_bal_power()){
      
      // P-Controller with up/down weigth
      if (bal_power>0){
        pwm_setpoint_new = pwm_setpoint - (bal_power*c_down);
      }
      if (bal_power<-c_hist){
        pwm_setpoint_new = pwm_setpoint - (bal_power*c_up);
      }
      
      //saturation
      if (pwm_setpoint_new >c_pwm_max)  pwm_setpoint_new = c_pwm_max;
      if (pwm_setpoint_new <0)    pwm_setpoint_new = 0;
  
      //update
      pwm_setpoint = pwm_setpoint_new;
  
      //reset watchtdog timer
      heatingLastUpdate = millis();
      
    }
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
  
  //write pwm
  analogWrite(pwmPin, pwm_setpoint);
}


void print_heating_info(){
  Serial.println("=============HEATING INFO==========");
  Serial.print("Balanced power:");
  Serial.println(bal_power);
  Serial.print("Balanced power valid:");
  Serial.println(bal_power_valid);
  Serial.print("PWM setpoint:  ");
  Serial.println(pwm_setpoint);
  Serial.print("Watch dog counter: ");
  Serial.println(watchdog_counter);
  Serial.println("===================================");
  }
