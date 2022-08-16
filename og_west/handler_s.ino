void setup_s()
{
}

void update_s(){
  for (int i = 0; i < num_s_states; i++) {
    if (value_s[i] != aux_value_s[i]){
        aux_value_s[i] = value_s[i];
        post_state(s_address[i], String(value_s[i]));
        switch(value_s[i]){
            case 0:
                write_l(s_up[i], "ON");
                write_l(s_down[i], "OFF");
                stop_time_s[i]      = millis()+up_time_s[i];
                stop_pending_s[i]   = true;
                break;
            case 100:
                write_l(s_up[i], "OFF");
                write_l(s_down[i], "ON");
                stop_time_s[i]      = millis()+down_time_s[i];
                stop_pending_s[i]   = true;
                break;
            default:
                aux_value_s[i] = value_s[i];
                write_l(s_up[i], "OFF");
                write_l(s_down[i], "OFF");
                stop_time_s[i]      = millis();
                stop_pending_s[i]   = false;
        }
    }
    if (stop_pending_s[i] and stop_time_s[i] < millis()){
        stop_pending_s[i]   = false;
        write_l(s_up[i], "OFF");
        write_l(s_down[i], "OFF");
    }
  }
}


void write_s(String address, String value) {
  for (int i = 0; i < num_s_states; i++) {
    if (s_address[i] == address) {
      if (value == "100") {
        value_s[i] = 100;
      }
      if (value == "0") {
        value_s[i] = 0;
      }
    }
  }
}
