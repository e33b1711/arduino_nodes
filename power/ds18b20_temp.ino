float tempHigh, tempLow;

#include <DS18B20.h>

DS18B20 ds(A7);





void setup_t(){
    lastUpdateDS_t  = millis(); 
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


void update_t(){

    if((lastUpdateDS_t+updatePeriodDS_t)<millis()){
        lastUpdateDS_t = millis();

        while (ds.selectNext()) {
            uint8_t address[8];
            ds.getAddress(address);
            switch (address[7]){
            case 51:
                Serial.print("tempHigh: ");
                value_t[0] = ds.getTempC()*10;
                Serial.print(tempHigh);
                Serial.println(" °C");
                break;
            case 189:
                Serial.print("tempLow: ");
                value_t[1] = ds.getTempC()*10;
                Serial.print(tempLow);
                Serial.println(" °C");
                break;
            default:
                Serial.println("ERROR: DS Address unknown."); 
            }
        }   
        for (int i=0; i<num_t_states; i++){
            send_state(t_address[i], value_t[i]);
        }
    }
}
