
#include <ModbusMaster.h> // EXTREN LIB...  have to be included
ModbusMaster node; //MODBUS NODE NUMBER OF SDM630 .. must be the same as configured in SDM630
const int SDM_SIZE                    = 3; 
float sdm_data[SDM_SIZE];
static int MAX_ITERATION              = 1; //maximum read MODBUS Value if checksum fails
//                                      Power L1-3             
const uint16_t sdm_adresses[SDM_SIZE] = {0x000C, 0x000E, 0x0010};
float bal_power                       = 0;
bool bal_power_valid                  = false;

unsigned long last_time;

float energyImport                      = 0;
float energyExport                      = 0;




void setup_modbus(){
    //modbus
    Serial.println("===============================");
    Serial.println("Setting up modbus.");
    Serial3.begin(9600);
    node.begin(1, Serial3);
    bool all_valid = true;
    for (int i=0; i<SDM_SIZE; i++){
        all_valid &= getRTUMore(sdm_adresses[i],1,i);
    }
    if (!all_valid){
        Serial.println("FATAL ERROR: no modbus, restart!");
        while(true){};
    }
    Serial.println("===============================");
}


//called form heat control
bool modbus_get_bal_power(){

    //get modbus data
    bool valid      = true;
    valid           &= getRTUMore(sdm_adresses[0],1,0);
    valid           &= getRTUMore(sdm_adresses[1],1,1);
    valid           &= getRTUMore(sdm_adresses[2],1,2);
    bal_power       = sdm_data[0] + sdm_data[1] + sdm_data[2];
    long this_time  = millis();

    //increment energy counters
    if (bal_power_valid and valid){
        if (bal_power>0){
            energyImport += bal_power*(this_time-last_time)*0.001;
        }else{
            energyExport -= bal_power*(this_time-last_time)*0.001;
        }
    }

    //store time / valid
    bal_power_valid     = valid;
    last_time           = this_time;
    return valid;
}


//internal
float reform_uint16_2_float32(uint16_t u1, uint16_t u2){  
    uint32_t num = ((uint32_t)u1 & 0xFFFF) << 16 | ((uint32_t)u2 & 0xFFFF);
    float numf;
    memcpy(&numf, &num, 4);
    return numf;
}


//internal
boolean getRTUMore(uint16_t m_startAddress,uint8_t m_length,int arrayPosition){
    uint16_t result;
    int counter = 0;
    while(counter < MAX_ITERATION){
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
void handle_modbus(){

}


//called from debug
void print_modbus_info(){
    Serial.println("=============MODBUS INFO==========");
    Serial.print("energyImport: ");         Serial.println(energyImport);
    Serial.print("energyExport: ");         Serial.println(energyExport);
    Serial.print("Balanced power:    ");    Serial.println(bal_power,3);
    Serial.print("bal_power_valid:   ");    Serial.println(bal_power_valid);
    Serial.println("==================================");
}
