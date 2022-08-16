void setup_a_b_c()
{
  Serial.println("setup_a_b_c");
  int i;
  for (i = 0; i < num_b_states; i++) {
    pinMode(b_pin[i], INPUT);
    digitalWrite(b_pin[i], HIGH);
  }
}


void update_a_b_c()
{
  int i;
  i = counter_a;

  //b, lesen und wert zuweisen
  prev_value_b[i] = value_b[i];
  int val;
  val = digitalRead(b_pin[i]);
  //Serial.println(val);
  //Serial.println(b_pin[i]);
  if (val == 1) {
    value_b[i] = 0;
  } else {
    value_b[i] = 1;
  }

  //c
  if (value_b[i] == 1 && prev_value_b[i] == 0) {
    value_c[i] = 1;
    time_c_pos[i] = millis();
    Serial.print("update_abc: pos Flanke: ");
    Serial.println(i);
  }
  else if (value_b[i] == 0 && prev_value_b[i] == 1)
  {
    value_c[i] = -1;
    time_c_neg[i] = millis();
    Serial.print("update_abc: neg Flanke: ");
    Serial.println(i);
  }
  else
  {
    value_c[i] = 0;
  }

  //count up
  counter_a = counter_a + 1;
  if (counter_a == num_b_states) {
    counter_a = 0;
    user_logic();
  }
}
