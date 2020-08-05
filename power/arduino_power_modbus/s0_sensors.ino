void update_s0(){
//get timestamp
  now               = millis();
  
  //reset update flags
  powerUpdateUtility    = false;
  powerUpdatePV         = false;
  powerUpdateHeating    = false;
  
  // read the value from the sensor, LED for debbuging
  oldValue          = sensorValue;
  sensorValue       = !digitalRead(sensorPin);
  oldValue1          = sensorValue1;
  sensorValue1       = digitalRead(sensorPin1);
  oldValue2          = sensorValue2;
  sensorValue2       = digitalRead(sensorPin2);
  digitalWrite(ledPin, sensorValue);
  
  
  //Serial.print(sensorValue);
  //Serial.print(sensorValue1);
  //Serial.println(sensorValue2);
  
  //detect flanks
  bool negFlank     = oldValue & !sensorValue;
  bool posFlank     = !oldValue & sensorValue;
  bool negFlank1    = oldValue1 & !sensorValue1;
  bool posFlank1    = !oldValue1 & sensorValue1;
  bool negFlank2    = oldValue2 & !sensorValue2;
  bool posFlank2    = !oldValue2 & sensorValue2;

  //a positive flank just gets stored
  if (posFlank){
    lastPosFlank        = now;
    nextUpdateUtility      = now;
  }
  if (posFlank1){
    lastPosFlank1       = now;
    nextUpdateHeating   = now;
  }
  if (posFlank2){
    lastPosFlank2       = now;
    nextUpdatePV        = now;
  }

   //a short pulse (<500ms) signals kWh increment
  if (negFlank & (now - lastPosFlank < 500)){
    powerUtility          = meterConstant/(now - lastNegFlank);
    lastNegFlank          = now;
    powerUpdateUtility    = killUpdate;     //supress first update
    killUpdate            = true;
  }
  if (negFlank1 & (now - lastPosFlank1 < 500)){
    powerHeating          = meterConstant1/(now - lastNegFlank1);
    lastNegFlank1         = now;
    powerUpdateHeating    = killUpdate1;    //supress first update
    killUpdate1           = true;
  }
  if (negFlank2 & (now - lastPosFlank2 < 500)){
    powerPV               = meterConstant2/(now - lastNegFlank2);
    lastNegFlank2         = now;
    powerUpdatePV         = killUpdate2;    //supress first update
    killUpdate2           = true;
  }

  //a permanent on (>500ms) signals 0 kWh (Utility counter only)
  if (sensorValue & (now - lastPosFlank > 500)){
    powerUtility = 0;
    powerUpdateUtility     = nextUpdateUtility < now;
    if (powerUpdateUtility) nextUpdateUtility = now + 5000;   
  }

  //force update after 90 sec (<20W) // 200 sec (<45W) (single phase counter only)
  if (!sensorValue1 & (now - lastNegFlank1 > 90000)){
    powerHeating            = 0;
    powerUpdateHeating      = nextUpdateHeating < now;
    if (powerUpdateHeating) nextUpdateHeating = now + 90000;   
  }
  if (!sensorValue2 & (now - lastNegFlank2 > 90000)){
    powerPV           = 0;
    powerUpdatePV     = nextUpdatePV < now;
    if (powerUpdatePV) nextUpdatePV = now + 90000;   
  }

  /*
  //a permanent off (> 600s) could be an error condition (Utility counter only)
  if (!sensorValue & (now - lastNegFlank > 600000)){
    errorUtility   = true;
    powerUpdateUtility   = nextUpdateUtility < now;
    if (powerUpdateUtility) nextUpdateUtility    = now + 5000;   
  }
  */
  
  //a permanent on (> 2s) could be an error condition (single phase counter/ eastron)
  if (sensorValue1 & (now - lastPosFlank1 > 2000)){
    errorHeating          = true;
    powerUpdateHeating    = nextUpdateHeating < now;
    if (powerUpdateHeating) nextUpdateHeating = now + 90000;   
  }
  if (sensorValue2 & (now - lastPosFlank2 > 2000)){
    errorPV               = true;
    powerUpdatePV         = nextUpdatePV < now;
    if (powerUpdatePV) nextUpdatePV = now + 90000;   
  }
}
