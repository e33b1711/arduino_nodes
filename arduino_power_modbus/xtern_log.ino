//server throttle timer
unsigned long lastExtLog;
const unsigned long extLogPeriod = 60000;
IPAddress log_ip(192, 168, 178, 222);
unsigned int log_port = 1999;

float extEnergyImport, extEnergyPV, extEnergyHeat, extEnergyImportUnsal, extEnergyExportUnsal;


void setup_extern_log(){
  lastExtLog = millis();
  extEnergyImport       = energyImport();
  extEnergyPV           = energyExport;
  extEnergyHeat         = energyHeat();
  extEnergyImportUnsal  = sdm_data[3];
  extEnergyExportUnsal  = sdm_data[4];
}
  

  



void handle_extern_log(){
  if (lastExtLog+extLogPeriod<millis()){
    lastExtLog = millis();

    //calc diffs
    float energyIm          = energyImport(); //need to sample this, could change will interput
    unsigned int import    = (energyIm - extEnergyImport)*1000000;
    unsigned int pv        = (energyPV() - extEnergyPV)*1000000;
    unsigned int heat      = (energyHeat() - extEnergyHeat)*1000000;
    unsigned int im_unsal  = (sdm_data[3] - extEnergyImportUnsal)*1000000;
    unsigned int ex_unsal  = (sdm_data[4] - extEnergyExportUnsal)*1000000;

    //set reference
    extEnergyImport       = energyIm;
    extEnergyPV           = energyExport;
    extEnergyHeat         = energyHeat();
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
                            "%d\n", epoch, import, pv, heat, im_unsal, ex_unsal);     

    //send packet
    Udp.beginPacket(log_ip, log_port);
    Udp.write(packetBuffer, NTP_PACKET_SIZE);
    Udp.endPacket();
  }
}
