void update_heating(){

  int pwm_setpoint_new;

  float bal_power = (sdm_data[3] + sdm_data[4] + sdm_data[5]);
  if (!(modbus_data_valid[3] and modbus_data_valid[4] and modbus_data_valid[5])) bal_power = 100000; 
  Serial.print("Balanced Power: "); Serial.println(bal_power);
  
  
  const float c_down = 1.0/10;
  const float c_up = 1.0/15;
  if (bal_power>0){
    pwm_setpoint_new = pwm_setpoint - (bal_power*c_down);
  }
  if (bal_power<-50){
    pwm_setpoint_new = pwm_setpoint - (bal_power*c_up);
  }
  if (pwm_setpoint_new >200)  pwm_setpoint_new = 200;
  if (pwm_setpoint_new <0)    pwm_setpoint_new = 0;
  pwm_setpoint = pwm_setpoint_new;
  Serial.print("New pwm setpoint: "); Serial.println(pwm_setpoint);
  analogWrite(pwmPin, pwm_setpoint);
}
