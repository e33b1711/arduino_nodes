#include <EEPROM.h>

/*
struct log_entry{
  long epoch;
  unsigned long pulse_count_0;
  unsigned long pulse_count_1;
  unsigned long pulse_count_2;
  float energy_export_unsal;
  float energy_import_unsal;
};
*/

const unsigned int num_entries = EEPROM.length()/sizeof(log_entry);
unsigned long lastLogUpdate;
const unsigned int max_age = 3600;  //TODO: make me shorter later
unsigned int log_pointer = 0;
bool checked_log = false;

void print_log_entry(log_entry l_ent){
  Serial.print("epoch:               "); Serial.println(l_ent.epoch); 
  Serial.print("pulseCount0:         "); Serial.println(l_ent.pulseCount0); 
  Serial.print("pulseCount1:         "); Serial.println(l_ent.pulseCount1); 
  Serial.print("pulseCount2:         "); Serial.println(l_ent.pulseCount2); 
  Serial.print("unsalEnergyImport: "); Serial.println(l_ent.unsalEnergyImport); 
  Serial.print("unsalEnergyExport: "); Serial.println(l_ent.unsalEnergyExport); 
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




void setup_log(){

  Serial.println("==========================");
  Serial.println("Setting up EEPROM log. This may take some secondes.");

  //get epoch & check if valid
  //look if there is a entry, that is younger than 600 secondes (ten minutes)
  //load that log entry to the energy base variables (enegry_counter0-2, unsalEnergyImport/Export)
  //set log counter to next entry (if no log entry set to 0)

  //check if valid time base
  if(timebase_valid){
    checked_log  = true;
    
    //get epoch
    unsigned long epoch = epoch_at_millis0 + millis()/1000;
    //Serial.print("DEBUG: epoch: "); Serial.println(epoch);
    bool got_entry = false;

    //look if there is a entry, that is younger than max_age secondes (ten minutes)
    //set log counter to next entry (if no log entry set to 0)
    log_entry this_le;
    log_pointer = (num_entries)*sizeof(log_entry);
    for(int i= 0; i<num_entries; i++){
      log_pointer -=sizeof(log_entry);
      //Serial.print("DEBUG: log_pointer: "); Serial.println(log_pointer);
      EEPROM.get(log_pointer,this_le);
      //Serial.print("DEBUG: this_le.epoch: "); Serial.println(this_le.epoch);
      if (this_le.epoch + max_age > epoch){
        got_entry = true;
        break;
      }
    }

    //load that log entry to the energy base variables (enegry_counter0-2, unsalEnergyImport/Export)
    if (got_entry){
      Serial.print("Found log entry at:"); Serial.println(log_pointer);
      print_log_entry(this_le);
      
      //renew log
      Serial.println("Renewing log...");
      this_le.epoch = epoch;
      EEPROM.put(log_pointer,this_le);
      
      //set pointer / restore data
      log_pointer +=sizeof(log_entry);
      pulseCount0 = this_le.pulseCount0;
      pulseCount1 = this_le.pulseCount1;
      pulseCount2 = this_le.pulseCount2;
      unsalEnergyImport = this_le.unsalEnergyImport;
      unsalEnergyExport = this_le.unsalEnergyExport;
    }else{
       Serial.println("Found log no entry. Setting log pointer to 0.");
       log_pointer = 0;
    }
    
  }else{
    Serial.println("ERROR: no timebase.");
  }
  lastLogUpdate = millis();
  Serial.println("==========================");

}





void handle_log(){

  //TODO: write only if day is whole / no other relevant Error status

  //if we are setup write a log entry every max_age/2 seconds
  if (lastLogUpdate+(max_age*500)<millis()){
    lastLogUpdate = millis();
    if(checked_log){
      unsigned long epoch = epoch_at_millis0 + millis()/1000;
      log_entry this_le;
      this_le.epoch       = epoch;
      this_le.pulseCount0 = pulseCount0;
      this_le.pulseCount1 = pulseCount1;
      this_le.pulseCount2 = pulseCount2;
      this_le.unsalEnergyImport = unsalEnergyImport;
      this_le.unsalEnergyExport = unsalEnergyExport;
      EEPROM.put(log_pointer, this_le);
      Serial.print("DEBUG: Wrote log entry to: "); Serial.println(log_pointer);
      log_pointer = (log_pointer + sizeof(log_entry)) % (sizeof(log_entry)*num_entries);
    }else{
      Serial.println("ERROR: Logging is not setup. Restart.");
    }
  }
  
  
}
