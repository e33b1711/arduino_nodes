
#include <ModbusMaster.h> // EXTREN LIB...  have to be included
ModbusMaster node; //MODBUS NODE NUMBER OF SDM630 .. must be the same as configured in SDM630
const int SDM_SIZE                    = 5; 
float sdm_data[SDM_SIZE];
bool sdm_data_valid[SDM_SIZE];
static int MAX_ITERATION              = 1; //maximum read MODBUS Value if checksum fails
//                                      Power L1-3              Import        Export  (nicht salierend!!)
const uint16_t sdm_adresses[SDM_SIZE] = {0x000C, 0x000E, 0x0010, 0x000048, 0x00004A};
unsigned long lastModbusUpdate;
unsigned long modbusPeriod = 31000;

//energy counter
float unsalEnergyImportZero               = 0;
float unsalEnergyExportZero               = 0;
float unsalEnergyImport                   = 0;
float unsalEnergyExport                   = 0;
float energyExport,lastEnergyExport       = 0;



void setup_modbus(){
  //modbus
  Serial.println("===============================");
  Serial.println("Setting up modbus.");
  Serial3.begin(9600);
  node.begin(1, Serial3);
  for (int i=0; i<SDM_SIZE; i++){
      sdm_data_valid[i] = getRTUMore(sdm_adresses[i],1,i);
  }
  lastModbusUpdate=millis();
  Serial.println("===============================");
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
    update_sdm_energy();
  }
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
  Serial.print("energyExport: ");
  Serial.println(energyExport,3);
  Serial.print("unsalEnergyExport: ");
  Serial.println(unsalEnergyExport,3);
  Serial.print("unsalEnergyImport: ");
  Serial.println(unsalEnergyImport,3);
  Serial.println("==================================");
}



void update_sdm_energy(){
  unsalEnergyImport =  sdm_data[3] - unsalEnergyImportZero;
  unsalEnergyExport =  sdm_data[4] - unsalEnergyExportZero;
  energyExport = unsalEnergyExport - unsalEnergyImport + energyImport;
  if (energyExport<0) energyExport=0; //to compensate errors between utility counter and modbus counter
}

void new_day_sdm(){
  unsalEnergyImportZero   = sdm_data[3];
  unsalEnergyExportZero   = sdm_data[4];
  lastEnergyExport = energyExport;
}
