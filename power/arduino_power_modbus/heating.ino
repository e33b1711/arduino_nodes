const int pwmPin            = 4;
int pwm_setpoint            = 0;
long heatingLastUpdate;
const float c_down          = 1.0/10;
const float c_up            = 1.0/15;
const float c_hist          = 50;
const float c_pwm_max       = 200;
const long time_out         = 20000; 
const long h_update_period  = 10000;
long watchdog_counter       = 0;

void setup_heating(){
  pinMode(pwmPin, OUTPUT);
  analogWrite(pwmPin, 0);
  Serial.println("===============================");
  Serial.println("Setting up heating.");
  Serial.println("===============================");
}

//TODO: detect if heating is off / overheated

//called form main loop
void update_heating(){

  int pwm_setpoint_new;
  float bal_power;

  //try to get a modbus update and regulate the power setting
  if (heatingLastUpdate+h_update_period>millis()){
    if (modbus_get_bal_power(bal_power)){
      
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

    Serial.println("DEBUG: Heating update");
    Serial.print("Balanced power: ");
    Serial.println(bal_power);
     Serial.print("pwm_setpoint: ");
    Serial.println(pwm_setpoint);
    
  }

  //watchdog
  if (heatingLastUpdate+time_out>millis()){
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
