void setup_r()
{
}

void update_r(){
  for (int i = 0; i < num_r_states; i++) {
    if (value_r[i] != aux_value_r[i]){
        aux_value_r[i] = value_r[i];
        post_state(r_address[i], String(value_r[i]));
        switch(value_r[i]){
            case 0:
                write_l(r_on_off[i],    "ON");
                write_l(r_up_down[i],   "OFF");
                stop_time_r[i]      = millis()+up_time_r[i];
                stop_pending_r[i]   = true;
                break;
            case 100:
                write_l(r_on_off[i],    "ON");
                write_l(r_up_down[i],   "ON");
                stop_time_r[i]      = millis()+down_time_r[i];
                stop_pending_r[i]   = true;
                break;
            default:
                aux_value_s[i] = value_s[i];
                write_l(r_on_off[i],    "OFF");
                write_l(r_up_down[i],   "OFF");
                stop_time_r[i]      = millis();
                stop_pending_r[i]   = false;
        }
    }
    if (stop_pending_r[i] and stop_time_r[i] < millis()){
        stop_pending_r[i]   = false;
        write_l(r_on_off[i],    "OFF");
        write_l(r_up_down[i],   "OFF");
    }
  }
}


void write_r(String address, String value) {
  for (int i = 0; i < num_r_states; i++) {
    if (r_address[i] == address) {
      if (value == "100") {
        value_r[i] = 100;
      }
      if (value == "0") {
        value_r[i] = 0;
      }
    }
  }
}
