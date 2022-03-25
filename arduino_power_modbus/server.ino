#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoOTA.h>
#include <ArduinoRS485.h> // ArduinoModbus depends on the ArduinoRS485 library
#include <ArduinoModbus.h>


//EthernetClient client;

EthernetServer server(502);
ModbusTCPServer modbusTCPServer;

void init_comm()
{
  // initialize
  Ethernet.init(ethernet_sc_pin);
  Ethernet.begin(mac, ip, gateway, subnet);
  // give the Ethernet shield a second to initialize:
  delay(500);
  Serial.println("init_comm: connecting...");
  //OTA
  ArduinoOTA.begin(Ethernet.localIP(), unit_name, password, InternalStorage);

  server.begin();
  

  modbusTCPServer.configureCoils(0x00, 1);
  modbusTCPServer.configureHoldingRegisters(0x00, 1);

  
}

void handle_comm(){
  //OTA
  ArduinoOTA.poll();

EthernetClient client = server.available();

  // when the client sends the first byte, say hello:
  if (client) {
    // a new client connected
    Serial.println("new client");

    // let the Modbus TCP accept the connection 
    modbusTCPServer.accept(client);

    while (client.connected()) {
      // poll for Modbus TCP requests, while client connected
      modbusTCPServer.poll();
      Serial.print("LED: ");
      Serial.print(modbusTCPServer.coilRead(0));
      Serial.print(" Chan: ");
      Serial.print(modbusTCPServer.holdingRegisterRead(0));

      // update the LED
      //updateLED();
    }

    Serial.println("client disconnected");
  }
  
}
