float reform_uint16_2_float32(uint16_t u1, uint16_t u2)
{  
   uint32_t num = ((uint32_t)u1 & 0xFFFF) << 16 | ((uint32_t)u2 & 0xFFFF);
    float numf;
    memcpy(&numf, &num, 4);
    return numf;
}


boolean getRTUMore(uint16_t m_startAddress,uint8_t m_length,int arrayPosition){

  uint16_t result;
  int counter = 0;
  while(counter < MAX_ITERATION)
  {
    //digitalWrite(3, HIGH); // TX
    if (node.ku8MBSuccess == node.readInputRegisters(m_startAddress, (m_length*2))) {
      for(int i = 0; i < m_length; i++){
        sdm_data[arrayPosition] = reform_uint16_2_float32(node.getResponseBuffer(i*2),node.getResponseBuffer((i*2)+1));
        arrayPosition++;
      }
      node.clearResponseBuffer();
      return true;   
    }
    counter++;
   //digitalWrite(3, LOW);
  }
  return false;  
} 

void poll_modbus() {
  if (last_modbus_update+modbus_update_i>millis()){
    modbus_update = false;
  }else{
    modbus_data_valid[round_counter] = getRTUMore(sdm_adresses[round_counter],1,round_counter);
    round_counter = (round_counter+1);
    if (round_counter == SDM_SIZE){
      round_counter = 0;
      modbus_update = true;
       Serial.println("modbus_update");
      last_modbus_update = millis();
    }else{
      modbus_update=false;
    }
  }
}
 


void print_param(int START,int END)
{
  for(int i = START; i <= END; i++)
  {
    //Serial.println("p");
    //Serial.println(startparam);
    //Serial.println("=");
    Serial.print(sdm_data[i]);
    Serial.print("   ");
    //Serial.println("&");
  }
  Serial.println(" ");
  
}


void modbus_to_tcp(){
    send_message("i", "P_L1", sdm_data[0]);
    send_message("i", "P_L1", sdm_data[1]);
    send_message("i", "P_L3", sdm_data[2]);
}


void modbus_to_serial()
{
  Serial.print("Modbus data vaild:");
  for(int i=0; i++; i<SDM_SIZE){
    Serial.print(modbus_data_valid[i]);
  }
  Serial.println(" ");
  Serial.print("Spannungen: ");
  print_param(  0,  2);     // Parameter   1 -   3
  Serial.print("Leistungen: ");
  print_param(  3, 5);     // Parameter   7 -   9
  Serial.print("phase angle:");
  print_param( 6, 6);     // Parameter  34
  Serial.print("Freq:");
  print_param( 7, 7);     // Parameter  36 -  37
  //Serial.print("voltrs thd:");
  //print_param( 23, 25,118);     // Parameter 118 - 120
  //Serial.print("amp thd:");
  //print_param( 26, 28,121);     // Parameter 121 - 123
}
