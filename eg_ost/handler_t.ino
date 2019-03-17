#include "SDHT.h"

SDHT dht;

void setup_t()
{
  Serial.println("setup_t");
  int i;
  for (i=0; i<num_t_states; i++){
   handle_one_t(i);
  }
}


void update_t()
{
  if (time_t+(period_t)<millis()){
    time_t=millis();
    i_t++;
    if (i_t==num_t_states){i_t=0;}
    handle_one_t(i_t);
    
  }
}


void write_t(String address, int value){
  int i;
  for (i=0; i<num_t_states; i++){
    if (t_address[i]==address){
      value_t[i]=value;
    }
  }
}

void handle_one_t(int i){
  if (!(i<num_t_states)) return;
 int8_t notice  = dht.broadcast(DHT22, t_pin[i]);
   if (notice == SDHT_OK){
      value_t[i]=dht.celsius*10;
      value_h[i]=dht.humidity;
      send_message("w", t_address[i], value_t[i]);
      send_message("w", h_address[i], value_h[i]);
      send_message("w", "E"+t_address[i], 0);
      Serial.print("handle_one_t: value_t: "); Serial.print(value_t[i]); 
      Serial.print(" value_h: "); Serial.print(value_h[i]);
      Serial.print("  i: "); Serial.println(i);
   }else{
      send_message("w", "E"+t_address[i], 1);
      Serial.print("handle_one_t: error reading from dth22. "); Serial.print("  i: "); Serial.println(i);
  }
}
