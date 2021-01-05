const int waterPin              = 2;
bool waterWarning              = false;     
long lastUpdateWater;
const long updatePeriodWater    = 10000;

void setup_ww(){
    Serial.println("===============================");
    Serial.println("Setting up water sensor.");
    pinMode(waterPin, INPUT);
    digitalWrite(waterPin, HIGH);
    lastUpdateWater  = millis(); 
    Serial.println("===============================");
}

void handle_ww(){

     if((lastUpdateWater+updatePeriodWater)<millis()){
        lastUpdateWater = millis();
        waterWarning = digitalRead(waterPin);
        Serial.println("===============================");
        Serial.print("Water Warning:");
        Serial.println(waterWarning);
        Serial.println("===============================");
     } 
}
