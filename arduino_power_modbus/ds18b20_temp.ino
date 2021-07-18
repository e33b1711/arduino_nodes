float tempHigh, tempLow;

#include <DS18B20.h>

DS18B20 ds(A7);

long lastUpdateDS;
const long updatePeriodDS = 10000;



void setup_temp(){
    lastUpdateDS  = millis(); 
    tempHigh      = 8.8;
    tempLow       = 9.9;
    Serial.println("=========================");
    Serial.println("DS Temp Sensors...");
    Serial.print("Devices: ");
    Serial.println(ds.getNumberOfDevices());
    while (ds.selectNext()) {
        switch (ds.getFamilyCode()) {
        case MODEL_DS18S20:
            Serial.println("Model: DS18S20/DS1820");
            break;
        case MODEL_DS1822:
            Serial.println("Model: DS1822");
            break;
        case MODEL_DS18B20:
            Serial.println("Model: DS18B20");
            break;
        default:
            Serial.println("Unrecognized Device");
            break;
        }
        
        uint8_t address[8];
        ds.getAddress(address);
        Serial.print("Address:");
        for (uint8_t i = 0; i < 8; i++) {
            Serial.print(" ");
            Serial.print(address[i]);
        }
        
        Serial.println();
    }
    
    Serial.println("=========================");
}


void handle_temp(){

    if((lastUpdateDS+updatePeriodDS)<millis()){
        lastUpdateDS = millis();

        while (ds.selectNext()) {
            uint8_t address[8];
            ds.getAddress(address);
            switch (address[7]){
            case 51:
                //Serial.print("tempHigh: ");
                tempHigh = ds.getTempC();
                //Serial.println(tempHigh);
                break;
            case 189:
                //Serial.print("tempLow: ");
                tempLow = ds.getTempC();
                //Serial.println(tempLow);
                break;
            default:
                Serial.println("ERROR: DS Address unknown."); 
            }
        }   
    }
}

void print_temp_info(){
    Serial.print("tempHigh: ");
    Serial.println(tempHigh);
    Serial.print("tempLow: ");
    Serial.println(tempLow);
}
