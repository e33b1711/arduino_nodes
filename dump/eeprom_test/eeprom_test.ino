#include <EEPROM.h>

struct log_entry{
  long epoch;
  unsigned long pulse_count_0;
  unsigned long pulse_count_1;
  unsigned long pulse_count_2;
  float energy_export_unsal;
  float energy_import_unsal;
};

const unsigned int num_entries = EEPROM.length()/sizeof(log_entry);

void print_log_entry(log_entry l_ent){
  Serial.print("epoch:               "); Serial.println(l_ent.epoch); 
  Serial.print("pulse_count_0:       "); Serial.println(l_ent.pulse_count_0); 
  Serial.print("pulse_count_1:       "); Serial.println(l_ent.pulse_count_1); 
  Serial.print("pulse_count_2:       "); Serial.println(l_ent.pulse_count_2); 
  Serial.print("energy_export_unsal: "); Serial.println(l_ent.energy_export_unsal); 
  Serial.print("energy_import_unsal: "); Serial.println(l_ent.energy_import_unsal); 
}




void setup() {

  Serial.begin(9600);
  Serial.print("EEPROM.lenght(): "); Serial.println(EEPROM.length()); 
  Serial.print("sizeof(log_entry): "); Serial.println(sizeof(log_entry));
  Serial.print("num_entries: "); Serial.println(num_entries);

  log_entry log_a;
  log_a.epoch               = 11111111;
  log_a.pulse_count_0       = 11111111;
  log_a.pulse_count_1       = 11111111;
  log_a.pulse_count_2       = 11111111;
  log_a.energy_export_unsal = 11111111;
  log_a.energy_import_unsal = 11111111;

  log_entry log_b;
  log_b.epoch               = 2222;
  log_b.pulse_count_0       = 2222;
  log_b.pulse_count_1       = 2222;
  log_b.pulse_count_2       = 2222;
  log_b.energy_export_unsal = 2222;
  log_b.energy_import_unsal = 2222;
 
  
  unsigned int index = 0;
  //EEPROM.put(index, log_a);
  index += sizeof(log_entry);
  //EEPROM.put(index, log_b);

  log_entry log_c;
  index = 0;
  EEPROM.get(index, log_c);
  Serial.print("Printing log entry at: "); Serial.println(index);
  print_log_entry(log_c);
  index += sizeof(log_entry);
  EEPROM.get(index, log_c);
  Serial.print("Printing log entry at: "); Serial.println(index);
  print_log_entry(log_c);


  
  

  /***
    Iterate the EEPROM using a do-while loop.
  ***/

  int idx = 0;  //Used 'idx' to avoid name conflict with 'index' above.

  do {

    //Add one to each cell in the EEPROM
    EEPROM[ idx ] += 1;
    idx++;
  } while (idx < EEPROM.length());


} //End of setup function.

void loop() {}
