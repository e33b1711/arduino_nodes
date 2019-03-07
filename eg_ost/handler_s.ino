void setup_s()
{
  Serial.println("setup_s");
  int i;
  for (i=0; i<num_s_states; i++){
    send_message("w", s_address[i], value_r[i]);
    stop_time_s[i]=millis()+down_time_s[i];;
  }
}


void update_s()
{
  int i;
  for (i=0; i<num_s_states; i++){
    if(stop_time_s[i]>millis()){
      if(value_s[i]==1){
        write_l(s_up[i], 1);
        write_l(s_down[i], 0);
      }
      if(value_s[i]==-1){
        write_l(s_up[i], 0);
        write_l(s_down[i], 1);
      }
    }else{
      if(!(value_s[i]==0)){
        write_l(s_up[i], 0);
        write_l(s_down[i], 0);
      } 
    }
  }
}

    
void write_s(String address, int value){
  int i;
  for (i=0; i<num_s_states; i++){
    if (s_address[i]==address){
      switch (value) {
      case 1:
        if (!(value_s[i]==1)){
          stop_time_s[i]=millis()+up_time_s[i];
          value_s[i]=1;
        }
        break;
      case -1:
        if (!(value_s[i]==-1)){
          stop_time_s[i]=millis()+down_time_s[i];
          value_s[i]=-1;
        }
        break;
      default:
        value_s[i]=0;
        break;
      }
    }
  }
}
