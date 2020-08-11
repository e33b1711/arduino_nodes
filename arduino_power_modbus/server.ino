//for udp communication
//watch out for the pins needed for the ethernet schield (always 10, 11 12 13 on uno, 50 51 52 53 on mega!)
#include <Ethernet.h>
#include <ArduinoOTA.h>
#include <EthernetUdp.h>



const int ethernet_sc_pin     = 53;
const int ethernet_reset_pin  = 50;
const byte mac[] = {0x4E, 0xAB, 0x7E, 0xEF, 0xFE, 0x04 };
const IPAddress ip(192,168,178,213);
const IPAddress gateway(192,168,178,1);
const IPAddress myDns(192,168,178,1);
const IPAddress subnet(255,255,255,0);
const IPAddress remIP(192,168,178,222);
const int remPort    = 8889;
const int localPort     = 8888;
bool alreadyConnected = false; // whether or not the client was connected previously
unsigned long lastServerUpdate;
const unsigned long serverUpdatePeriod = 11500;


//ntp
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
const char timeServer[] = "time.nist.gov"; // time.nist.gov NTP server
unsigned long  epoch_at_millis0;
const unsigned long seconds_per_day = 86400;
unsigned long last_unix_day;
bool day_is_whole = false;
bool last_day_is_whole = false;

EthernetUDP Udp;
// buffers for receiving and sending data
char packetBuffer[NTP_PACKET_SIZE];  // buffer to hold incoming packet,


void setup_server(){
  Serial.println("===============================");
  Serial.println("Setting up server...");

  lastServerUpdate = millis();

  //reset via pin
  pinMode(ethernet_reset_pin, OUTPUT);
  digitalWrite(ethernet_reset_pin, HIGH);
  delay(10);
  digitalWrite(ethernet_reset_pin, LOW);

  //setup sever
  Ethernet.init(ethernet_sc_pin);
  Ethernet.begin(mac, ip, myDns, gateway, subnet);
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("ERROR: Ethernet shield was not found.");
  }else{
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("ERROR: Ethernet cable is not connected.");
    }else{
      Udp.begin(localPort);
      ArduinoOTA.begin(Ethernet.localIP(), unit_name, password, InternalStorage);
      Serial.print("Server address:");
      Serial.println(Ethernet.localIP());
      Serial.print("Port:");
      Serial.println(localPort);
      Serial.println("SUCESS!");
      Serial.print("The gateway IP address is: ");
      Serial.println(Ethernet.gatewayIP());
      Serial.print("The DNS server IP address is: ");
      Serial.println(Ethernet.dnsServerIP());
      update_time();
      unsigned long epoch           = epoch_at_millis0 + millis()/1000;
      unsigned long secondes_today  = epoch % seconds_per_day;
      unsigned long unix_day        = epoch / seconds_per_day;
      last_unix_day                 = unix_day;
  }
  Serial.println("===============================");
  }
  
}

//called form main loop
void handle_server(){

  //look for OTA Update
  ArduinoOTA.poll();

  //update the time before millis() overflows
  if(millis() > 0x0FFFFFFF){
    update_time();
  }

  
  


    if (lastServerUpdate+serverUpdatePeriod<millis()) {

      //check new day / get time
      unsigned long epoch           = epoch_at_millis0 + millis()/1000;
      unsigned long secondes_today  = epoch % seconds_per_day;
      unsigned long unix_day        = epoch / seconds_per_day;
      if (last_unix_day < unix_day){
        Serial.println("New UTC/unix day.");
        last_unix_day = unix_day;
        new_day_S0();
        new_day_sdm();
        last_day_is_whole = day_is_whole;
        day_is_whole  = true;
      }

     sprintf(packetBuffer,  "===================================\n");                 send_packet();
     //
     sprintf(packetBuffer,  "unit name:          %s\n", unit_name);                   send_packet();
     sprintf(packetBuffer,  "Version:            %s\n", vers);                        send_packet();
     sprintf(packetBuffer,  "Unix epoch:         %lu\n", epoch );                     send_packet();
     sprintf(packetBuffer,  "Unix seconds today: %lu\n",secondes_today );             send_packet();
     sprintf(packetBuffer,  "Unix day :          %lu\n", unix_day);                   send_packet();
     //
     sprintf(packetBuffer,  "===================================\n");                 send_packet();
     sprintf(packetBuffer,  "bal_power:          %i\n", (int)bal_power);              send_packet();
     sprintf(packetBuffer,  "bal_power_valid:    %i\n", bal_power_valid);             send_packet();
     sprintf(packetBuffer,  "pwm_setpoint:       %i\n", pwm_setpoint);                send_packet();
     sprintf(packetBuffer,  "watchdog_counter:   %i\n", watchdog_counter);            send_packet();
     //
     sprintf(packetBuffer,  "===================================\n");                 send_packet();
     sprintf(packetBuffer,  "powerUtility:       %i\n", powerUtility);                send_packet();
     sprintf(packetBuffer,  "errorUtility:       %i\n", errorUtility);                send_packet();
     sprintf(packetBuffer,  "powerHeating:       %i\n", powerHeating);                send_packet();
     sprintf(packetBuffer,  "errorHeating:       %i\n", errorHeating);                send_packet();
     sprintf(packetBuffer,  "powerPV:            %i\n", powerPV);                     send_packet();
     sprintf(packetBuffer,  "errorPV:            %i\n", errorPV);                     send_packet();
     //
     sprintf(packetBuffer,  "===================================\n");                 send_packet();
     sprintf(packetBuffer,  "All energies in mWh.\n");                                send_packet();
     sprintf(packetBuffer,  "Day is whole :      %i\n", day_is_whole);                send_packet();
     sprintf(packetBuffer,  "energyUtility:      %lu\n", energyUtility);              send_packet();
     sprintf(packetBuffer,  "energyHeat:         %lu\n", energyHeat);                 send_packet();
     sprintf(packetBuffer,  "energyPV:           %lu\n", energyPV);                   send_packet();
     sprintf(packetBuffer,  "energyExport:       %lu\n", energyExport);               send_packet();
     sprintf(packetBuffer,  "unsalEnergyExport:  %lu\n", unsalEnergyExport);          send_packet();
     sprintf(packetBuffer,  "unsalEnergyImport:  %lu\n", unsalEnergyImport);          send_packet();
     //
     sprintf(packetBuffer,  "===================================\n");                 send_packet();
     sprintf(packetBuffer,  "Last day is whole : %i\n", last_day_is_whole);           send_packet();
     sprintf(packetBuffer,  "lastEnergyUtility:  %lu\n", lastEnergyUtility);          send_packet();
     sprintf(packetBuffer,  "lastEnergyHeat:     %lu\n", lastEnergyHeat);             send_packet();
     sprintf(packetBuffer,  "lastEnergyPV:       %lu\n", lastEnergyPV);               send_packet();
     sprintf(packetBuffer,  "lastEngeryExport:   %lu\n", lastEngeryExport);           send_packet();
     sprintf(packetBuffer,  "sdm_data_valid:     %i, %i, %i, %i, %i\n", sdm_data_valid[0],sdm_data_valid[1], sdm_data_valid[2], sdm_data_valid[3], sdm_data_valid[4]); send_packet();
     //
     sprintf(packetBuffer,  "===================================\n");                 send_packet();
      
      lastServerUpdate = millis(); 
    }
  


 
  
}


void send_packet(){

Udp.beginPacket(remIP, remPort);
      Udp.write(packetBuffer);
      Udp.endPacket();
}


// send an NTP request to the time server at the given address
void sendNTPpacket(const char * address) {
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); // NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

void update_time(){
  Serial.println("=================="); 
  Serial.println("Getting NTP time...");
  sendNTPpacket(timeServer); // send an NTP packet to a time server
  delay(1000);
  if (Udp.parsePacket()) {
    // We've received a packet, read the data from it
    Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

    // the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, extract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    //Serial.print("Seconds since Jan 1 1900 = ");
    //Serial.println(secsSince1900);

    // now convert NTP time into everyday time:
    Serial.print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;
    epoch_at_millis0 = epoch - millis()/1000;
    // print Unix time:
    Serial.println(epoch);

    /*
    // print the hour, minute and second:
    Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
    Serial.print((epoch  % 86400L) / 3600); // print the hour (86400 equals secs per day)
    Serial.print(':');
    if (((epoch % 3600) / 60) < 10) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.print((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
    Serial.print(':');
    if ((epoch % 60) < 10) {
      // In the first 10 seconds of each minute, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.println(epoch % 60); // print the second
    */
  }else{
    Serial.println("ERROR: Got no ntp response");
  }
  Serial.println("==================");
}
