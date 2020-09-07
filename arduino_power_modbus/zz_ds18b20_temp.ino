float tempHigh, tempLow;

#include <DS18B20.h>

DS18B20 ds(A7);

long lastUpdateDS;
const long updatePeriodDS = 5000;



void setup_temp(){
  lastUpdateDS  = millis(); 
  tempHigh      = 8.8;
  tempLow       = 9.9;
  Serial.println("=========================");
   Serial.println("DS Temp Sensors...");
  Serial.print("Devices: ");
  Serial.println(ds.getNumberOfDevices());
   Serial.println("=========================");

  
}


void handle_temp(){

    if((lastUpdateDS+updatePeriodDS)<millis()){
      lastUpdateDS = millis();

    //ds.selectNext();
    Serial.print("tempHigh: ");
    //tempHigh = ds.getTempC();
    Serial.print(tempHigh);
    Serial.println(" °C");

    //ds.selectNext();
    Serial.print("tempLow: ");
    //tempLow = ds.getTempC();
    Serial.print(tempLow);
    Serial.println(" °C");
  
      

    }
  
  
    
}
