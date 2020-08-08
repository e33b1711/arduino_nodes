
#include <ModbusMaster.h> // EXTREN LIB...  have to be included
ModbusMaster node; //MODBUS NODE NUMBER OF SDM630 .. must be the same as configured in SDM630
const int SDM_SIZE                    = 8; 
float sdm_data[SDM_SIZE];
bool sdm_data_valid[SDM_SIZE];
static int MAX_ITERATION              = 1; //maximum read MODBUS Value if checksum fails
//                                      Power L1-3              Voltage L1-3            Phase A. Freq
const uint16_t sdm_adresses[SDM_SIZE] = {0x000C, 0x000E, 0x0010, 0x0000, 0x0002, 0x0004,  0x0042, 0x0046};
long lastModbusUpdate;
long modbusPeriod = 11000;



void setup_modbus(){
  //modbus
  Serial2.begin(9600);
  node.begin(1, Serial2);
  Serial.println("===============================");
  Serial.println("Setting up modbus.");
  Serial.println("===============================");

  lastModbusUpdate=millis();
}

//called form heat control
bool modbus_get_bal_power(){
  sdm_data_valid[0]  = getRTUMore(sdm_adresses[0],1,0);
  sdm_data_valid[1]  = getRTUMore(sdm_adresses[1],1,1);
  sdm_data_valid[2]  = getRTUMore(sdm_adresses[2],1,2);
  bal_power             = sdm_data[0] + sdm_data[1] + sdm_data[2];
  bal_power_valid =  sdm_data_valid[0] & sdm_data_valid[1] & sdm_data_valid[2];
  return bal_power_valid;
}

//internal
float reform_uint16_2_float32(uint16_t u1, uint16_t u2)
{  
   uint32_t num = ((uint32_t)u1 & 0xFFFF) << 16 | ((uint32_t)u2 & 0xFFFF);
    float numf;
    memcpy(&numf, &num, 4);
    return numf;
}

//internal
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

//called form server
void handle_modbus() {
  if (lastModbusUpdate+modbusPeriod<millis()) {
    lastModbusUpdate = millis();
    for (int i=3; i<SDM_SIZE; i++){
      sdm_data_valid[i] = getRTUMore(sdm_adresses[i],1,i);
    }
  }
}

//internal
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

//called from debug
void modbus_to_serial()
{
  Serial.println("=============MODBUS INFO==========");
  Serial.print("Modbus data vaild:");
  for(int i=0; i<SDM_SIZE; i++){
    Serial.print(sdm_data_valid[i]);
  }
  Serial.println(" ");
  Serial.print("Leistungen: ");
  print_param(  0,  2);     // Parameter   1 -   3
  Serial.print("Spannungen: ");
  print_param(  3, 5);     // Parameter   7 -   9
  Serial.print("phase angle:");
  print_param( 6, 6);     // Parameter  34
  Serial.print("Freq:");
  print_param( 7, 7);     // Parameter  36 -  37
  Serial.println("==================================");
}