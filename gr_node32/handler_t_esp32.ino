#include "DHT.h"

DHT dht[] = {
  {DHTPIN1, DHT22},
  {DHTPIN2, DHT22},
};

void setup_t()
{
  Serial.println("setup_t");
  int i;
  for (i=0; i<num_t_states; i++){
    dht[i].begin();
    handle_one_t(i);
  }
}


void update_t()
{
  if (s_time_t+(period_t)<millis()){
    s_time_t=millis();
    i_t++;
    if (i_t==num_t_states){i_t=0;}
    handle_one_t(i_t);
    
  }
}


void handle_one_t(int i){
  if (!(i<num_t_states)) return;
  float h = dht[i].readHumidity()*0.1;
  float t = dht[i].readTemperature();
  if (isnan(h) || isnan(t)) {
    send_state(t_address[i], "999.9");
    send_state(h_address[i], "999.9");
    Serial.print("handle_one_t: error reading from dth22. "); Serial.print("  i: "); Serial.println(i);
  }else{
    send_state(t_address[i], String(t));
    send_state(h_address[i], String(h*10.0));
  }
}
