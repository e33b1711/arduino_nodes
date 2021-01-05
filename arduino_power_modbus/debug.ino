unsigned long last_update_debug;
const unsigned long debug_update_interval       = 20000;
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
        Serial.println("===============================");
        Serial.print("Unit name:");
        Serial.println(unit_name);
        Serial.print("Version:");
        Serial.println(vers);
        Serial.println("===============================");
        print_server_info();
        print_heating_info();
        print_modbus_info();
        print_s0_info();
    } 


    while (Serial.available() > 0) {
        char inChar = Serial.read();

        inString += (char)inChar;
    
        // if you get a newline, print the string, then the string's value:
        if (inChar == '\n') {
            if (inString.substring(0,13)=="target_power:"){
                int val = inString.substring(13).toInt();
                target_power = val;
                Serial.print("Set target_power: ");
                Serial.println(target_power);
            }
            if (inString.substring(0,13)=="control_mode:"){
                int val = inString.substring(13).toInt();
                control_mode = val;
                Serial.print("Set control_mode:");
                Serial.println(control_mode);
            }
            inString = "";
        }   
    }
}
