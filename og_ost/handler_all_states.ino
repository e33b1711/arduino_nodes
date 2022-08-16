// state schreiben, egal ob auf dieser unit oder anderer
void write_state(String address, String value) {
  write_l(address, value);
  write_r(address, value);
  write_s(address, value);
  write_u(address, value);
}


/*
//check ob es diesen state auf dieser unit gibt
boolean is_my_state(String address) {
  int i;
  for (i = 0; i < num_t_states; i++) {
    if (t_address[i] == address) {
      return true;
    }
  }
  for (i = 0; i < num_h_states; i++) {
    if (h_address[i] == address) {
      return true;
    }
  }
  for (i = 0; i < num_u_states; i++) {
    if (u_address[i] == address) {
      return true;
    }
  }
  for (i = 0; i < num_l_states; i++) {
    if (l_address[i] == address) {
      return true;
    }
  }
  for (i = 0; i < num_r_states; i++) {
    if (r_address[i] == address) {
      return true;
    }
  }
  for (i = 0; i < num_s_states; i++) {
    if (s_address[i] == address) {
      return true;
    }
  }
  return false;
}
*/
