/*
  File: esp8266_mqtt_edge_node.ino
  Date: July 12, 2025

  Description:
  This firmware is designed for ESP8266-based edge nodes to securely publish data to, 
  and receive commands from, a central MQTT broker (hosted on a Raspberry Pi). 

  Key Features:
  The credentials for wifi and MQTT is given by the STM32 via UART
  - Connects to WiFi.
  - Connects to MQTT Broker.
  - Publishes on topic.
  - Publishes mock sensor data to a specific topic
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define RETAIN_MSG true
// WiFi & MQTT setup
WiFiClient espClient;
PubSubClient client(espClient);

// Buffers and flags
String received = "";

String ssid = "";
String wifi_pass = "";
String mqtt_user = "";
String mqtt_pass = "";
int mqtt_port = 1883;
String topic  = "";
String mqtt_broker = "10.182.7.185";  // Default fallback for MQTT broker.


bool got_wifi = false;
bool got_mqtt = false;
bool connected = false;
bool auth_required = false;

void setup() {
  Serial.begin(115200);  // UART to STM32
  delay(10);
  while (!Serial);     // Wait for serial

  client.setCallback(callback);
  Serial.println("\n\r\nESP ready to receive credentials from STM32...\n");
}

void loop() {
  // Accumulate serial data from STM32
  if (Serial.available()) {
    char c = Serial.read();
    received += c;

    if (c == '\n') {
      parseCommand(received);  // Parse complete line
      received = "";
    }
  }

  // Try connecting if all credentials are received
  if (!connected && got_wifi && got_mqtt) {
    Serial.println("Connecting to WiFi...");
    WiFi.begin(ssid.c_str(), wifi_pass.c_str());

    int retries = 0;
    while (WiFi.status() != WL_CONNECTED && retries < 20) {
      delay(500);
      Serial.print(".");
      retries++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nWiFi connected!");
      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());

     IPAddress broker_ip;
    if (broker_ip.fromString(mqtt_broker)) 
    {
    client.setServer(broker_ip, mqtt_port);
    Serial.println("✓ Broker IP set from STM32: ");
    Serial.print(broker_ip);
    } 
    else
    {
    client.setServer(mqtt_broker.c_str(), mqtt_port); // fallback to domain string
    Serial.println("✓ Broker domain fallback used.");
  }


      if (auth_required) 
      {
        String clientId = "espClient-";
        clientId += String(ESP.getChipId()); // Guaranteed unique per ESP
        connected = client.connect(clientId.c_str(), mqtt_user.c_str(), mqtt_pass.c_str());
      } else {
        String clientId = "espClient-";
        clientId += String(ESP.getChipId()); // Guaranteed unique per ESP
        connected = client.connect(clientId.c_str());
      }

      if (connected) {
        Serial.println("Connected to MQTT broker!");
        Serial.println("ESP ready for topic publishing...");
      } else {
        Serial.println("MQTT connection failed!");
      }
    } else {
      Serial.println("\nWiFi connection failed!");
    }
  }

  // Maintain MQTT connection
  if (connected) {
    client.loop();
  }

  if (!client.connected() && got_wifi && got_mqtt) 
  {
    Serial.println("MQTT disconnected. Attempting reconnect...");
    reconnectMQTT();  // Create this function
  }
  client.loop();  // Must always be called when connected

}

// MQTT message reception callback (if needed)
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("MQTT Msg arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

// Parse command sent from STM32
void parseCommand(String cmd) {
  cmd.trim();

  if (cmd.startsWith("wifi=")) {
    int idx1 = cmd.indexOf('=');
    int idx2 = cmd.indexOf(';');
    ssid = cmd.substring(idx1 + 1, idx2);

    int idx3 = cmd.indexOf("pass=");
    int idx4 = cmd.indexOf(';', idx3 + 5);
    wifi_pass = cmd.substring(idx3 + 5, idx4);
    got_wifi = true;

    Serial.println("Received WiFi credentials");
  }

  else if (cmd.indexOf("auth=") != -1) {
    String auth_val = getValue(cmd, "auth=");
    auth_required = (auth_val == "1");

    mqtt_broker = getValue(cmd, "broker=");  // <-- NEW
    mqtt_port = getValue(cmd, "port=").toInt();

    if (auth_required) {
        mqtt_user = getValue(cmd, "mqtt_user=");
        mqtt_pass = getValue(cmd, "mqtt_pass=");
    } else {
        mqtt_user = "";
        mqtt_pass = "";
    }

    got_mqtt = true;

    Serial.println("Received MQTT setup from STM32");
    Serial.print("Broker: "); Serial.println(mqtt_broker);
    if (auth_required)
      Serial.println("Auth required");
    else
      Serial.println("Anonymous mode");
}


  else if (cmd.startsWith("topic=")) {
    topic = getValue(cmd, "topic=");
    String message = getValue(cmd, "msg=");
    String retainVal = getValue(cmd, "retain=");

    bool retain = (retainVal == "1");

    if (connected) {
      client.publish(topic.c_str(), message.c_str(), retain);
      Serial.println("Published to MQTT:");
      Serial.println("Topic: " + topic);
      Serial.println("Message: " + message);
      Serial.println("Retain: " + String(retain));
    } else {
      Serial.println("MQTT not connected. Cannot publish.");
    }
}
}

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT reconnect...");

    String clientId = "espClient-";
    clientId += String(ESP.getChipId());

    if (auth_required) {
      connected = client.connect(clientId.c_str(), mqtt_user.c_str(), mqtt_pass.c_str());
    } else {
      connected = client.connect(clientId.c_str());
    }

    if (connected) {
      Serial.println("✓ Reconnected.");
      client.subscribe(topic.c_str());  // Re-subscribe
    } else {
      Serial.print("✗ Failed. State: ");
      Serial.println(client.state());
      delay(2000);  // Wait before retrying
    }
  }
}

// Helper to extract value from a command string
String getValue(String data, String key) {
  int start = data.indexOf(key);
  if (start == -1) return "";
  start += key.length();
  int end = data.indexOf(';', start);
  if (end == -1) end = data.length();
  return data.substring(start, end);
}

  }
}
// ======================== END =============================
