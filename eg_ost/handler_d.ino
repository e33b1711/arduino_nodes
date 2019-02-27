void setup_d()
{
  int i;
  for (i=0; i<num_d_states; i++){
  pinMode(d_pin[i], OUTPUT);
  value_d[i]=0;
  send_message("w", d_address[i], value_d[i]);
  }
}


void update_d()
{
  int i;
  for (i=0; i<num_d_states; i++){
    analogWrite(d_pin[i], value_d[i]);
  }
}


void write_d(String address, int value){
  int i;
  for (i=0; i<num_d_states; i++){
    if (d_address[i]==address){
      value_d[i]=value;
    }
  }
}

