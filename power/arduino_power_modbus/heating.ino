void update_heating(){

while (Serial.available() > 0) {
    char inChar = Serial.read();

      inString += (char)inChar;
    
    // if you get a newline, print the string, then the string's value:
    if (inChar == '\n') {
      if (inString.substring(0,4)=="pwm:"){
        int val = inString.substring(4).toInt();
        Serial.print("setPWM:");
        Serial.println(val);
        analogWrite(pwmPin, val);
        watchDog = millis();
        // clear the string for new input:
      }
       inString = "";
    }
  }

  if (watchDog + 60000 < millis()){
     Serial.print("setPWM:");
        Serial.println(0);
        analogWrite(pwmPin, 0);
        watchDog = millis();
  }

}
