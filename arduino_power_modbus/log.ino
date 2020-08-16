#include <EEPROM.h>


const unsigned int num_entries      = EEPROM.length()/sizeof(log_entry);
unsigned long lastLogUpdate;
const unsigned long max_age         = 3600;  //TODO: make me shorter later
unsigned int log_pointer            = 0;
bool day_intact                     = false;
const int max_modbus_iter           = 5;

void print_log_entry(log_entry l_ent){
  Serial.print("day_intact:            "); Serial.println(l_ent.day_intact); 
  Serial.print("epoch:                 "); Serial.println(l_ent.epoch); 
  Serial.print("pulseCount0:           "); Serial.println(l_ent.pulseCount0); 
  Serial.print("pulseCount1:           "); Serial.println(l_ent.pulseCount1); 
  Serial.print("pulseCount2:           "); Serial.println(l_ent.pulseCount2); 
  Serial.print("unsalEnergyImportZero: "); Serial.println(l_ent.unsalEnergyImportZero); 
  Serial.print("unsalEnergyExportZero: "); Serial.println(l_ent.unsalEnergyExportZero); 
}

void clear_eeprom(){
  Serial.println("==============================================================================");
  Serial.println("Clearing EEPROM. This may take some secondes. Watchdog may bark afterwards.");
  for(int i=0; i<EEPROM.length(); i++){
    EEPROM[i] = 0;  
  }
  log_pointer = 0;
  Serial.println("done.");
  Serial.println("==============================================================================");
}

void readback_log(){
   Serial.println("=======================================");
   Serial.print("num_entries: "); Serial.println(num_entries);
   Serial.print("sizeof(log_entry): "); Serial.println(sizeof(log_entry));
   unsigned int log_pointer_m1 = (log_pointer+((num_entries-1)*sizeof(log_entry))) % (num_entries*sizeof(log_entry));
   Serial.print("Current Log Entry: log pointer: "); Serial.println(log_pointer);
   Serial.print("Current Log Entry: log pointer-sizeof(): "); Serial.println(log_pointer_m1);
   Serial.print("Milis til next log entry: "); Serial.println(lastLogUpdate+(max_age*500)-millis());
   log_entry this_le;
   EEPROM.get(log_pointer_m1,this_le);
   print_log_entry(this_le);
   Serial.print("Actual epoch: "); Serial.println(epoch);
   Serial.print("max_age: "); Serial.println(max_age);
   Serial.println("=======================================");
}

void readback_log(int val){
   Serial.println("=======================================");
   unsigned int rp = val % (num_entries*sizeof(log_entry));
   Serial.print("Reading back Log Entry at: "); Serial.println(rp);
   log_entry this_le;
   EEPROM.get(rp,this_le);
   print_log_entry(this_le);
   Serial.println("=======================================");
}




void setup_log(){

  Serial.println("==========================");
  Serial.println("Setting up EEPROM log. This may take some secondes.");

  //get epoch & check if valid
  //look if there is a entry, that is younger than 600 secondes (ten minutes)
  //load that log entry to the energy base variables (enegry_counter0-2, unsalEnergyImport/Export)
  //set log counter to next entry (if no log entry set to 0)

  //check if valid time base
  
    
  //look if there is a entry, that is younger than max_age secondes (ten minutes)
  bool got_entry = false;
  log_entry this_le;
  log_pointer = (num_entries)*sizeof(log_entry);
  for(int i= 0; i<num_entries; i++){
    log_pointer -=sizeof(log_entry);
    EEPROM.get(log_pointer,this_le);
    if (this_le.epoch + max_age > epoch){
      got_entry = true;
      break;
    }
  }

  //load that log entry to the energy base variables (enegry_counter0-2, unsalEnergyImport/Export)
  //set log counter to next entry (if no log entry set to 0)
  if (got_entry){
    Serial.print("Found log entry at:"); Serial.println(log_pointer);
    print_log_entry(this_le);
    
    //renew log
    Serial.println("Renewing log...");
    this_le.epoch = epoch;
    EEPROM.put(log_pointer,this_le);
    
    //set pointer / restore data
    log_pointer = (log_pointer + sizeof(log_entry)) % (sizeof(log_entry)*num_entries);
    Serial.print("Setting log pointer to: "); Serial.println(log_pointer);
    day_intact  = this_le.day_intact;
    pulseCount0 = this_le.pulseCount0;
    pulseCount1 = this_le.pulseCount1;
    pulseCount2 = this_le.pulseCount2;
    unsalEnergyImportZero = this_le.unsalEnergyImportZero;
    unsalEnergyExportZero = this_le.unsalEnergyExportZero;
  }else{
     Serial.println("Found log no entry. Setting log pointer to 0.");
     log_pointer = 0;
     unsalEnergyImportZero = sdm_data[3];
     unsalEnergyExportZero = sdm_data[4];
     day_intact  = false;
  }
    
  lastLogUpdate = millis();
  Serial.println("==========================");

}

void new_day_log(){

  //write a old day log
  write_log(epoch-10);

  //try to make modbus valid
  if(!sdm_data_valid){
    for(int i=0; i<max_modbus_iter; i++){
      handle_modbus();
    }
  }
  if(!sdm_data_valid){
    Serial.println("FATAL ERROR: no valid modbus data, restart!");
    while(true){};
  }

  //reinit all statefull data
  unsalEnergyImportZero   = sdm_data[3];
  unsalEnergyExportZero   = sdm_data[4];
  pulseCount0             = 0;
  pulseCount1             = 0;
  pulseCount2             = 0;
  day_intact              = true;

  //write a new day log
  write_log(epoch);
}

void handle_log(){
  //TODO: write only if day is whole / no other relevant Error status
  //if we are setup write a log entry every max_age/2 seconds
  if (lastLogUpdate+(max_age*500)<millis()){
    lastLogUpdate = millis();
      write_log(epoch);
  } 
}




void write_log(unsigned long epoch_override){
  log_entry this_le;
  this_le.day_intact  = day_intact;
  this_le.epoch       = epoch_override;
  this_le.pulseCount0 = pulseCount0;
  this_le.pulseCount1 = pulseCount1;
  this_le.pulseCount2 = pulseCount2;
  this_le.unsalEnergyImportZero = unsalEnergyImportZero;
  this_le.unsalEnergyExportZero = unsalEnergyExportZero;
  EEPROM.put(log_pointer, this_le);
  Serial.print("DEBUG: Wrote log entry to: "); Serial.println(log_pointer);
  log_pointer = (log_pointer + sizeof(log_entry)) % (sizeof(log_entry)*num_entries);
}






      
