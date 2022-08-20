float tempHigh, tempLow;

#include <DS18B20.h>

DS18B20 ds(A7);





void setup_t(){
    lastUpdateDS_t  = millis(); 
     while (ds.selectNext()) {
            uint8_t address[8];
            ds.getAddress(address);
            switch (address[7]){
            case 51:
                Serial.print("tempHigh: ");
                value_t[0] = ds.getTempC()*10;
                send_state(t_address[0], String(ds.getTempC()));
                Serial.print(tempHigh);
                Serial.println(" °C");
                break;
            case 189:
                Serial.print("tempLow: ");
                value_t[1] = ds.getTempC()*10;
                send_state(t_address[1], String(ds.getTempC()));
                Serial.print(tempLow);
                Serial.println(" °C");
                break;
            default:
                Serial.println("ERROR: DS Address unknown."); 
            }
        }   
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
                send_state(t_address[0], String(ds.getTempC()));
                Serial.print(tempHigh);
                Serial.println(" °C");
                break;
            case 189:
                Serial.print("tempLow: ");
                value_t[1] = ds.getTempC()*10;
                send_state(t_address[1], String(ds.getTempC()));
                Serial.print(tempLow);
                Serial.println(" °C");
                break;
            default:
                Serial.println("ERROR: DS Address unknown."); 
            }
        }   
    }
}
