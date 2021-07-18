unsigned long last_update_debug;
const unsigned long debug_update_interval       = 10000;
String inString                                 = "";

void setup_debug(){
  Serial.begin(9600);
  Serial.println("===============================");
  Serial.println("Power controller unit started.");
  Serial.println("===============================");
  last_update_debug = millis();
}

//called form main loop
void handle_debug(){
  //print sensor values & setpoint periodically
    if (last_update_debug+debug_update_interval < millis()){
        last_update_debug = millis();
        print_heating_info();
        print_s0_info();
        print_temp_info();
    } 


    while (Serial.available() > 0) {
        char inChar = Serial.read();

        inString += (char)inChar;
    
        // if you get a newline, print the string, then the string's value:
        if (inChar == '\n') {            
            //Serial.print("inString: " + inString);
            if (inString.substring(0,13)=="pwm_setpoint:"){
                int val = inString.substring(13).toInt();
                set_pwm(val);
                //Serial.print("Set pwm_setpoint: ");
                //Serial.println(pwm_setpoint);
            }
        }   
    }
}
