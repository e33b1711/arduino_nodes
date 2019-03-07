void setup_r()
{
  Serial.println("setup_r");
  int i;
  for (i=0; i<num_r_states; i++){
  send_message("w", r_address[i], value_r[i]);
  }
}


void update_r()
{
  int i;
  for (i=0; i<num_r_states; i++){
    if(stop_time_r[i]>millis()){
      if(value_r[i]==1){
        write_l(r_on_off[i], 1);
        write_l(r_up_down[i], 0);
      }
      if(value_r[i]==-1){
        write_l(r_on_off[i], 1);
        write_l(r_up_down[i], 1);
      }
    }else{
      if(!(value_r[i]==0)){
        write_l(r_on_off[i], 0);
        write_l(r_up_down[i], 0);
      } 
    }
  }
}

    
void write_r(String address, int value){
  int i;
  for (i=0; i<num_r_states; i++){
    if (r_address[i]==address){
      switch (value) {
      case 1:
        if (!(value_r[i]==1)){
          stop_time_r[i]=millis()+up_time_r[i]*1000;
          value_r[i]=1;
        }
        break;
      case -1:
        if (!(value_r[i]==-1)){
          stop_time_r[i]=millis()+down_time_r[i]*1000;
          value_r[i]=-1;
        }
        break;
      default:
        value_r[i]=0;
        break;
      }
    }  
  }
}
