void setup_h()
{
  Serial.println("setup_h");
}


void update_h(){}



void write_h(String address, int value){
  int i;
  for (i=0; i<num_h_states; i++){
    if (h_address[i]==address){
      value_h[i]=value;
    }
  }
}
