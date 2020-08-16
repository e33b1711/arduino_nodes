//server throttle timer
unsigned long lastExtLog;
const unsigned long extLogPeriod = 60000;
IPAddress log_ip(192, 168, 178, 222);
unsigned int log_port = 1999;

float extPulseCount0, extPulseCount1, extPulseCount2, extEnergyImportUnsal, extEnergyExportUnsal;


void setup_extern_log(){
  lastExtLog = millis();
  extPulseCount0        = pulseCount0;
  extPulseCount1        = pulseCount1;
  extPulseCount2        = pulseCount2;
  extEnergyImportUnsal  = sdm_data[3];
  extEnergyExportUnsal  = sdm_data[4];
}
  

  



void handle_extern_log(){
  if (lastExtLog+extLogPeriod<millis()){
    lastExtLog = millis();

    //calc diffs
    unsigned long pc0        = pulseCount0;
    unsigned long pc1        = pulseCount1;
    unsigned long pc2        = pulseCount2;

    unsigned long d_pc0      = extPulseCount0 - pc0;
    unsigned long d_pc1      = extPulseCount1 - pc1;
    unsigned long d_pc2      = extPulseCount2 - pc2;
    unsigned long im_unsal  = (sdm_data[3] - extEnergyImportUnsal)*1000000;
    unsigned long ex_unsal  = (sdm_data[4] - extEnergyExportUnsal)*1000000;

    //set reference
    extPulseCount0        = pc0;
    extPulseCount1        = pc1;
    extPulseCount2        = pc2;
    extEnergyImportUnsal  = sdm_data[3];
    extEnergyExportUnsal  = sdm_data[4];
    
    //epoch, energy since last packet, import, pv, heat, unsal im, unsal ex
    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    sprintf(packetBuffer,   "\n\n" 
                            "%6ld\n"
                            "%d\n"
                            "%d\n"
                            "%d\n"
                            "%d\n"
                            "%d\n", epoch, d_pc0, d_pc1, d_pc2, im_unsal, ex_unsal);     

    //send packet
    Udp.beginPacket(log_ip, log_port);
    Udp.write(packetBuffer, NTP_PACKET_SIZE);
    Udp.endPacket();
  }
}
