/*
  Created by Igor Jarc
 See http://iot-playground.com for details
 Please use community fourum on website do not contact author directly
 
 Code based on https://github.com/DennisSc/easyIoT-ESPduino/blob/master/sketches/ds18b20.ino
 
 External libraries:
 - https://github.com/adamvr/arduino-base64
 - https://github.com/milesburton/Arduino-Temperature-Control-Library
 
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */
//#include <ESP8266WiFi.h>
//#include <Base64.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//AP definitions
#define AP_SSID "your ssid"
#define AP_PASSWORD "ap password"

// EasyIoT server definitions
#define EIOT_USERNAME    "admin"
#define EIOT_PASSWORD    "test"
#define EIOT_IP_ADDRESS  "192.168.1.5"
#define EIOT_PORT        80
#define EIOT_NODE        "N13S0"

#define REPORT_INTERVAL 60 // in sec


#define ONE_WIRE_BUS 2  // DS18B20 pin
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);


#define USER_PWD_LEN 40
char unameenc[USER_PWD_LEN];
float oldTemp;


void setup() {
  Serial.begin(115200);
  
  //wifiConnect();
    
  //char uname[USER_PWD_LEN];
  //String str = String(EIOT_USERNAME)+":"+String(EIOT_PASSWORD);  
  //str.toCharArray(uname, USER_PWD_LEN); 
  //memset(unameenc,0,sizeof(unameenc));
  //base64_encode(unameenc, uname, strlen(uname));
  oldTemp = -1;
}

void loop() {
  float temp;
  
  do {
    DS18B20.requestTemperatures(); 
    temp = DS18B20.getTempCByIndex(0);
    Serial.print("Temperature: ");
    Serial.println(temp);
  } while (temp == 85.0 || temp == (-127.0));
  
  if (temp != oldTemp)
  {
    //sendTeperature(temp);
    oldTemp = temp;
  }
  
  int cnt = REPORT_INTERVAL;
  
  
    delay(1000);
}
