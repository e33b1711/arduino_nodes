void setup_u()
{
  Serial.println("setup_u");
  int i;
  for (i = 0; i < num_u_states; i++) {
    post_state(u_address[i], String(value_u[i]));
    digitalWrite(u_pin[i], LOW);
    pinMode(u_pin[i], OUTPUT);
  }
  for (i = 0; i < num_u_states; i++) {
    delay(400);
    digitalWrite(u_pin[i], HIGH);
  }
  previousMillis = millis();
}


void update_u()
{
  unsigned long currentMillis = millis() + 2 * u_interval;
  int i;
  if (currentMillis - previousMillis >= u_interval) {
    previousMillis = currentMillis;
    u_phase = u_phase + 1;
    if (u_phase > 14) {
      u_phase = 0;
    }
    for (i = 0; i < num_u_states; i++) {
      //Serial.println(value_u[i]>=((u_phase + 2*i) % 16));
      if (value_u[i] > ((u_phase + 2 * i) % 15)) {
        digitalWrite(u_pin[i], HIGH);
      } else {
        digitalWrite(u_pin[i], LOW);
      }
    }
  }
}


void write_u(String address, String value_str) {
  int i;
  int value = value_str.toInt();
  for (i = 0; i < num_u_states; i++) {
    if (u_address[i] == address) {
         Serial.println("write_u: " + address);
      if (value < 0) {
        value_u[i] = 0;
      }
      else if (value > 15) {
        value_u[i] = 7;
      }
      else {
        value_u[i] = value;
      }
      post_state(u_address[i], String(value_u[i]));
    }
  }
}
