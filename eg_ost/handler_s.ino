void setup_s()
{
}

void update_s(){
  for (int i = 0; i < num_s_states; i++) {
    if (value_s[i] != aux_value_s[i]){
        aux_value_s[i] = value_s[i];
        switch(value_s[i]){
            case 0:
                write_l(s_up[i], 1);
                write_l(s_down[i], 0);
                stop_time_s[i]      = millis()+up_time_s[i];
                stop_pending_s[i]   = true;
                break;
            case 100:
                write_l(s_up[i], 0);
                write_l(s_down[i], 1);
                stop_time_s[i]      = millis()+down_time_s[i];
                stop_pending_s[i]   = true;
                break;
            default:
                aux_value_s[i] = value_s[i];
                write_l(s_up[i], 0);
                write_l(s_down[i], 0);
                stop_time_s[i]      = millis();
                stop_pending_s[i]   = false;
        }
    }
    if (stop_pending_s[i] and stop_time_s[i] < millis()){
        stop_pending_s[i]   = false;
        write_l(s_up[i], 0);
        write_l(s_down[i], 0);
    }
  }
}


void write_s(String address, int value) {
  for (int i = 0; i < num_s_states; i++) {
    if (s_address[i] == address) {
      if (value == 0) {
        value_s[i] = 0;
      }
      if (value == 100) {
        value_s[i] = 100;
      }
      send_state(address, value_s[i]);
    }
  }
}
