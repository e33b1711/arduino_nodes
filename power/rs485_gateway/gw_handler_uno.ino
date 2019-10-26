 
String com_in_buffer="";       
String rs485_in_buffer=""; 
String rs485_out_buffer="";    




void setup()  
{

  Serial.begin(9600);

//
  Serial1.begin(9600);


}

void loop(){
  Serial.print("rs484>>");
 while(Serial1.available()){
  byte c = Serial1.read();
  Serial.println(c);
 }
 Serial.println("<<");

  delay(1000);

  //if(Serial.available()){
  if(true){
    Serial.read();
    Serial.println("writting modbus command.");
  Serial1.write(0x01);
  Serial1.write(0x04);
  Serial1.write(0x00);
  Serial1.write(0x00);
  Serial1.write(0x04);
 }

  
 

}
