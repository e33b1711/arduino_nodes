const int pwmPin                      = 5;
unsigned long heatingLastUpdate;
const float c_down                    = 1.0/10;
const float c_up                      = 1.0/15;
const float c_hist                    = 25;   //hystereses
const float c_pwm_max                 = 200;
const unsigned long time_out          = 10000; 
const unsigned long h_update_period   = 1000;
float bal_power                       = 0;
float target_power                    = -50;
int control_mode                      =  1; //0=off, 1=control balanced power, 2=control heat power
bool bal_power_valid                  = false;
int pwm_setpoint                      = 0;
unsigned long watchdog_counter        = 0;
bool h_control_off                    = false;

void setup_heating(){
  Serial.println("===============================");
  pinMode(pwmPin, OUTPUT);
  analogWrite(pwmPin, 0);
  
  Serial.println("Setting up heating.");
  Serial.print("Switching on...");
  analogWrite(pwmPin, 150);
  delay(4000);
  Serial.println(" and off.");
  analogWrite(pwmPin, 0);
  heatingLastUpdate = millis();
  Serial.println("===============================");
}


//called form main loop
void update_heating(){

  int pwm_setpoint_new;

  //try to get a modbus update and regulate the power setting
  if (heatingLastUpdate+h_update_period<millis()){
    //reset watchtdog timer
    heatingLastUpdate = millis();
      
    bool control_on;
    float control_power;
    switch control_mode{
      case 1:
        control_on = modbus_get_bal_power();
        control_power = bal_power;
        break;
      case 2:
        control_on = true;  //TODO: check s0 error condition here!
        control_power = powerHeat;
        break;
      default:
        control_on = false;
        break;
    }
    
    if (control_on){
      
      // P-Controller with up/down weigth
      if (control_power>target_power+c_hist){
        pwm_setpoint_new = pwm_setpoint - (control_power*c_down);
      }
      if (control_power<target_power-c_hist){
        pwm_setpoint_new = pwm_setpoint - (control_power*c_up);
      }
      
      //saturation
      if (pwm_setpoint_new >c_pwm_max)  pwm_setpoint_new = c_pwm_max;
      if (pwm_setpoint_new <0)    pwm_setpoint_new = 0;
  
      //update
      pwm_setpoint = pwm_setpoint_new;
    }else{
      pwm_setpoint = 0;
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
  Serial.print("Balanced power: ");
  Serial.println(bal_power,3);
  Serial.println("target_power: ");
  Serial.println(target_power);
  Serial.println("control_mode: ");
  Serial.println(control_mode);
  Serial.print("PWM setpoint:  ");
  Serial.println(pwm_setpoint);
  Serial.print("Watch dog counter: ");
  Serial.println(watchdog_counter);
  Serial.println("===================================");
  }
