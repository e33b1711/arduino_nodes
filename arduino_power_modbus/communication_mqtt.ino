#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoOTA.h>
#include <ArduinoMqttClient.h>


EthernetClient client;
MqttClient mqtt_client(client);

int conn_error_count = 0;

//these are defined unit spefic file
//byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
//IPAddress ip(192,168,1,177);
//IPAddress server(1,1,1,1)
//int port = 8888;


void init_comm()
{
  // initialize
  Ethernet.init(ethernet_sc_pin);
  Ethernet.begin(mac, ip, gateway, subnet);
  // give the Ethernet shield a second to initialize:
  delay(500);
  Serial.println("init_comm: connecting...");



  //OTA
  //ArduinoOTA.begin(Ethernet.localIP(), unit_name, password, InternalStorage);

  //mqtt
  mqtt_client.connect(server, broker_port);
  mqtt_client.onMessage(message_rx);
  mqtt_client.subscribe(command_prefix + "#");
}


void post_state(String out_address, int out_value) {
  mqtt_client.beginMessage(state_prefix + out_address);
  mqtt_client.print(out_value);
  mqtt_client.endMessage();
}

void message_rx(int message_size) {
  String topic = mqtt_client.messageTopic();
  int index = topic.indexOf("/");
  String prefix = topic.substring(0, index);
  String address = topic.substring(index + 1);
  //Serial.println(prefix);
  //Serial.println(address);

  // we received a message, print out the topic and contents
  //Serial.println("Received a message with topic '");
  //Serial.println(mqtt_client.messageTopic());
  //Serial.println("', length ");
  //Serial.println(message_size);
  //Serial.println(" bytes:");

  // use the Stream interface to print the contents
  String payload;
  while (mqtt_client.available()) {
    payload += (char)mqtt_client.read();
  }
  //Serial.println(payload);
  //Serial.println(payload.toInt());
  write_state(address, payload.toInt());
}


void handle_comm() {

  //OTA
  ArduinoOTA.poll();

  //mqtt
  mqtt_client.poll();

  //reconnect ethernet
  //TODO

  //reconnect mqtt
  //TODO




}


void post_all() {
  int i;
  for (i = 0; i < num_t_states; i++) {
    post_state(t_address[i], value_t[i]);
  }
  for (i = 0; i < num_p_states; i++) {
    post_state(p_address[i], value_p[i]);
  }
}
