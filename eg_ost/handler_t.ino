#include <dht.h>
dht DHT;


void setup_t()
{
  Serial.println("setup_t");
  int i;
  for (i=0; i<num_t_states; i++){
    int chk = DHT.read22(t_pin[i]);
    Serial.println(chk);
    switch(chk){
    case DHTLIB_OK:
      value_t[i]=DHT.temperature*10;
      value_h[i]=DHT.humidity;
      send_message("w", t_address[i], value_t[i]);
      send_message("w", h_address[i], value_h[i]);
      Serial.print("setup_t: value_t: "); Serial.print(value_t[i]); Serial.print("  i: "); Serial.println(i);
      break;
    default:
      value_t[i]=-400;
      value_h[i]=-10;
      Serial.println("setup_t: error reading from dth22.");
      break;
    }
  }
}


void update_t()
{
  if (time_t+(period_t*1000)<millis()){
    time_t=millis();
    i_t++;
    if (i_t==num_t_states){i_t=0;}
    int chk = DHT.read22(t_pin[i_t]);
    Serial.println(chk);
    switch(chk){
    case DHTLIB_OK:
      value_t[i_t]=DHT.temperature*10;
      value_h[i_t]=DHT.humidity;
      send_message("w", t_address[i_t], value_t[i_t]);
      send_message("w", h_address[i_t], value_h[i_t]);
      Serial.print("setup_t: value_t: "); Serial.print(value_t[i_t]); Serial.print("  i: "); Serial.println(i_t);
      break;
    default:
      value_t[i_t]=-400;
      value_h[i_t]=-10;
      Serial.println("setup_t: error reading from dth22.");
      break;
    } 
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
