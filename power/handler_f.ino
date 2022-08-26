void setup_f()
{
  Serial.println("setup_f");
  for (int i = 0; i < num_f_states; i++) {
    pinMode(f_pin[i], INPUT);
  }
}


void update_f()
{
  for (int i = 0; i < num_f_states; i++) {
    int new_val = digitalRead(f_pin[i]);
    if (f_inv[i]) new_val = not(new_val);
    if (new_val != f_value[i]) {
      send_state(f_address[i], new_val);
      Serial.println(f_address[i] + " : " + String(new_val, DEC));
      f_value[i] = new_val;
    }
  }
}
