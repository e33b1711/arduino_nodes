void setup_l()
{
  Serial.println("setup_l");
  int i;
  for (i = 0; i < num_l_states; i++) {
    digitalWrite(l_pin[i], !l_inv[i]);
    pinMode(l_pin[i], OUTPUT);
    value_l[i] = 0;
    post_state(l_address[i], String(value_l[i]));
  }
}


void update_l()
{
  int i;
  for (i = 0; i < num_l_states; i++) {
    switch (value_l[i]) {
      case 1:
        if (l_inv[i] >= 0) digitalWrite(l_pin[i], l_inv[i]);
        break;
      default:
        if (l_inv[i] >= 0) digitalWrite(l_pin[i], !l_inv[i]);
        value_l[i] = 0;
        break;
    }
  }
}


void write_l(String address, String value) {
  int i;
  for (i = 0; i < num_l_states; i++) {
    if (l_address[i] == address) {
      set_time_l[i] = millis();
      if (value_l[i] == 1) {
        value_l[i] = 0;
      } else {
        value_l[i] = 1;
      }

      if (value == "0") {
        value_l[i] = 0;
      }
      if (value == "1") {
        value_l[i] = 1;
      }
      if (value == "OFF") {
        value_l[i] = 0;
      }
      if (value == "ON") {
        value_l[i] = 1;
      }
      if (value_l[i]==1){
        post_state(l_address[i], "ON");
      }else{
        post_state(l_address[i], "OFF");
      }
    }
  }
}


// Achtung: funktioniert nur bei eigenen States (auf der selben unit)
void toggle_state(String address) {
  int value = 0;
   for (int i = 0; i < num_l_states; i++) {
    if (l_address[i] == address) {
      value = value_l[i];
    }
  }
  if (value == 0) {
    write_state(address, "ON");
  }
  else {
    write_state(address, "OFF");
  }
}
