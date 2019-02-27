////constants and variables for a states (2 R-codierte Schalter an einem analogen Eingang)
////                              0               1
////                              strang wz       strang wz
////                              grün            gelb
//const int num_a_states=2;
//const int a_pin[]={             0,              1};        //analoger pin
//const int threshold1[]={        500,            500};      //schwelle zwischen 00 - 01
//const int threshold2[]={        400,            400};      //schwelle zwischen 01 - 10
//const int threshold3[]={        200,            200};      //schwelle zwischen 10 - 11
//int value_a[]={                 0,               0,};       //00 - 01 - 10 -11 msb= größerer Parallelwiderstand
//


void setup_a_b_c()
{
  int i;
  for (i=0; i<num_b_states; i++){
     pinMode(b_pin[i], INPUT);
     digitalWrite(b_pin[i], HIGH);
  }
}


void update_a_b_c()
{
  int i;
 
  
  
  i=counter_a;
  
    //b, lesen und wert zuweisen
    prev_value_b[i]=value_b[i];
    int val;
    
    val = digitalRead(b_pin[i]);
    //Serial.println(val);
    //Serial.println(b_pin[i]);
    if (val==1){
      value_b[i]=0;}
    else{
      value_b[i]=1;}
    
    
    
  
  //c
  if (value_b[i]==1 && prev_value_b[i]==0){
    value_c[i]=1;
    time_c_pos[i]=millis();
  }
  else if(value_b[i]==0 && prev_value_b[i]==1)
  {
    value_c[i]=-1;
    time_c_neg[i]=millis();
  }
  else
  {
    value_c[i]=0;
  }
 
 
  //
   counter_a=counter_a+1;
  if (counter_a==num_b_states){
    counter_a=0;
    user_logic();
  }
}
  


