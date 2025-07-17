#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "CDAC";
const char* password = "";
const char* mqtt_server = "10.182.7.185";  // Replace with your broker IP
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

String incomingSerial = "";
bool mqtt_ready = false;  

void connectWiFi() 
{
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
   Serial.print("\n\r");
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  
  // Print MAC address
  Serial.print("\nESP MAC Address: ");
  Serial.println(WiFi.macAddress());
}

void mqttCallback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("MQTT Message Received [");
  Serial.print(topic);
  Serial.print("]: ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void connectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("esp01_client")) {
      Serial.println("Connected to MQTT broker!\n");
      mqtt_ready = true;  // ✅ Set flag after connection
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(". Retrying in 2s...");
      delay(2000);
    }
  }
}

void parseAndPublish(String cmd) {
  String topic = "";
  String msg = "";

  int topicIndex = cmd.indexOf("topic=");
  int msgIndex = cmd.indexOf("msg=");

  if (topicIndex >= 0) {
    int end = cmd.indexOf(";", topicIndex);
    topic = cmd.substring(topicIndex + 6, end);
  }

  if (msgIndex >= 0) {
    int end = cmd.indexOf(";", msgIndex);
    msg = cmd.substring(msgIndex + 4, end);
  }

  if (topic.length() > 0 && msg.length() > 0) {
    client.publish(topic.c_str(), msg.c_str());
    Serial.println("Published: " + msg + " to " + topic);
  } else {
    Serial.println("Invalid command format!");
  }
}

void checkSerialCommand() {
  // ✅ Only process UART commands if MQTT is connected
  if (!mqtt_ready) return;

  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      parseAndPublish(incomingSerial);
      incomingSerial = "";
    } else {
      incomingSerial += c;
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(10);

  connectWiFi();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(mqttCallback);
}

void loop() {
  if (!client.connected()) {
    mqtt_ready = false;
    connectMQTT();
  }
  client.loop();
  checkSerialCommand();
}
