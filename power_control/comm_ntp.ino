//for ntp time & OTA
#include <EthernetUdp.h>
EthernetUDP Udp;
unsigned int localPort = 8888;                // local port to listen for UDP packets
const int NTP_PACKET_SIZE = 48;               // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[NTP_PACKET_SIZE];           //buffer to hold incoming and outgoing packets
const char timeServer[] = "time.nist.gov";    // time.nist.gov NTP server

unsigned long  epoch_at_millis0;          //timebase referneced to millis=0
unsigned long  epoch_at_timebase;         //time we got the timebase
unsigned long startup_unix_day;           //unix day at statrtup
unsigned long startup_seconds_today;      //unix secondes at statrtup
bool timebase_valid = false;              //do we have a timebase?
unsigned long epoch;                      //epoch updated every cycle
unsigned long seconds_today;              //recent uninx seconds of this day
unsigned long unix_day;                   // this unix day 

const unsigned long seconds_per_day = 86400;  //exactly what it says 

void setup_ntp(){
    //time & OTA
    Serial.println("===============================");
    Serial.println("Setting up ntp....");
    Udp.begin(localPort);
    update_time();
    if(!timebase_valid){
        //stop
        Serial.println("FATAL ERROR: no timebase, restart!");
        while(true){};
    }
    epoch           = epoch_at_millis0 + millis()/1000;
    seconds_today   = epoch % seconds_per_day;
    unix_day        = epoch / seconds_per_day;
    startup_unix_day              = unix_day;
    startup_seconds_today         = seconds_today;
}


void handle_ntp(){

    //update the time before millis() overflows
    //TO DO: how good is the arduinos clock??
    if(millis() > 0x0FFFFFFF){
        update_time();
    }

    //update time / detect new day
    epoch                          = epoch_at_millis0 + millis()/1000;
    seconds_today                  = epoch % seconds_per_day;
    unsigned long unix_day_update  = (epoch / seconds_per_day);
    unix_day                       = unix_day_update;
}

void print_time_info(){
    Serial.println("=============DS18B20 INFO==========");
    Serial.print("epoch:         ");    Serial.println(epoch);
    Serial.print("seconds_today: ");  Serial.println(seconds_today);
    Serial.print("unix_day:      ");  Serial.println(unix_day);
    Serial.println("==================================");
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
    if (Udp.parsePacket()){
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
        epoch_at_timebase = epoch;
        timebase_valid = true;
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
        timebase_valid = false;
    }
    Serial.println("==================");
}
