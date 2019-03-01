#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,178, 200);
IPAddress server(192,168,178,222);
int port=8888;


// telnet defaults to port 23
EthernetClient client;
boolean Connected = false; // whether or not the client was connected previously


String com_out_buffer="";    
String com_in_buffer="";       
String rs485_in_buffer=""; 
String rs485_out_buffer=""; 

long timer;




void setup()  
{
  
  com_in_buffer.reserve(40);
  rs485_in_buffer.reserve(40);
  rs485_out_buffer.reserve(40);
  Serial.begin(9600);
  Serial.println("hello");
  Ethernet.begin(mac, ip);
  Serial.println(Ethernet.hardwareStatus());
  Connected = client.connect(server, port);
  if (Connected){
    Serial.println("connected");
  }else{
    Serial.println("failure");}
  timer=millis();
}

//token ring frame:  enthält nur ganze !w!LI_OG_L1!3$ kommandos, andet mit <nxt_address>&



void loop(){
  char in_char;
  int index;
  boolean listening=true;
  
  if(Connected){
  //listening on inserver and rs485
    while(listening){
      in_char='a';
      if(client.available()){
        in_char = (byte)client.read();
        com_in_buffer += in_char;
        if (in_char=='$'){
          rs485_out_buffer += com_in_buffer;
          com_in_buffer="";
        }
      }
    
      if(Serial.available()){
        in_char = (byte)Serial.read();
        rs485_in_buffer += in_char;
        if (in_char=='&'){
          listening=false;
        }
      }
    
    //check for token ring timeout
      if (timer+5000<millis()){
        Serial.print(nxt_address);
        Serial.print('&');
        timer=millis();
      }
    }

    //sending on com and rs485
    index=rs485_in_buffer.length();
    //bin ich mit senden dran? ja, falls frame beendet und nxt_address == address
    if ((rs485_in_buffer.charAt(index-2)==address) && (rs485_in_buffer.charAt(index-1)=='&')){
      delay(10);
      Serial.print(rs485_out_buffer);
      Serial.write(nxt_address);
      Serial.write("&");
      timer=millis();
      rs485_out_buffer="";
    }
    //senden auf com
    if (index>2){
      client.println(rs485_in_buffer.substring(0,index-2));
    }
    rs485_in_buffer="";
    
  }else{ // try to connect
    Connected = client.connect(server, port);
  }

}




