#include <OneWire.h>
#include <DallasTemperature.h>



OneWire oneWire_in(A7);
OneWire oneWire_out(A8);

DallasTemperature sensor_inhouse(&oneWire_in);
DallasTemperature sensor_outhouse(&oneWire_out);

long lastUpdateDS;
const long updatePeriodDS = 5000;

float tempHigh, tempLow;

void setup_temp(){
  lastUpdateDS  = millis(); 
  tempHigh      = 8.8;
  tempLow       = 9.9;

      sensor_inhouse.begin();
    sensor_outhouse.begin();
  
}


void handle_temp(){

    if((lastUpdateDS+updatePeriodDS)<millis()){
      lastUpdateDS = millis();
 

   Serial.print("Requesting temperatures...");
    sensor_inhouse.requestTemperatures();
    sensor_outhouse.requestTemperatures();
    Serial.println(" done");

    Serial.print("Inhouse: ");
    tempHigh = sensor_inhouse.getTempCByIndex(0);
    Serial.println(tempHigh);

    Serial.print("Outhouse: ");
    tempLow = sensor_inhouse.getTempCByIndex(0);
    Serial.println(tempLow);
      

    }
  
  
    
}
