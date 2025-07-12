/*
  File: esp8266_mqtt_edge_node.ino
  Date: July 12, 2025

  Description:
  This firmware is designed for ESP8266-based edge nodes to securely publish data to, 
  and receive commands from, a central MQTT broker (hosted on a Raspberry Pi). 

  Key Features:
  - Connects to WiFi and MQTT broker
  - Publishes mock sensor data to a specific topic
  - Subscribes to command topic to control onboard GPIO (LED)
  - LED indications for connection status and activity
*/
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// ================== WiFi and MQTT Configuration ==================

// Replace with your actual WiFi SSID and Password.
const char* ssid = "CDAC";
const char* password = "";

// Replace with the IP address of your Raspberry Pi running the MQTT broker
const char* mqtt_server = "10.182.7.174";                 // ← Update with real broker IP
const int mqtt_port = 1883;                               // Default MQTT port

// Define MQTT topics for this edge node
const char* publish_topic = "edge_node/esp8266/data";     // Publishing data to broker
const char* subscribe_topic = "edge_node/esp8266/cmd";    // Subscribed command topic

// =============== MQTT and WiFi Client Objects ====================
WiFiClient espClient;                                     // Raw TCP client
PubSubClient client(espClient);                           // MQTT client over WiFi

// =============== GPIO Definitions ===============================
const int ledPin = LED_BUILTIN;                           // GPIO pin for onboard LED (usually GPIO2 on ESP-12)

// =================== WiFi Connection ============================
void connectWiFi() 
{
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
    blink();                                              //LED indication.
  }

  Serial.println("\nWiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

// =============== MQTT Message Callback ==========================
void mqttCallback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("MQTT Command Received [");
  Serial.print(topic);
  Serial.print("]: ");

  // Convert payload byte array to a clean string
  String command;
  for (unsigned int i = 0; i < length; i++) 
  {
    command += (char)payload[i];
  }
  command.trim();                                         // Remove trailing newline/space
  Serial.println(command);

  // Command handling logic
  if (command == "LED_ON") 
  {
    digitalWrite(ledPin, LOW);                            // Turn ON LED (active LOW)
  } else if (command == "LED_OFF") 
  {
    digitalWrite(ledPin, HIGH);                           // Turn OFF LED
  } else 
  {
    Serial.println("Unknown command");
  }
}

// ================== MQTT Connection =============================
void connectMQTT() 
{
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    digitalWrite(ledPin, LOW);                           // Brief LED ON to indicate MQTT retry attempt

    // Attempt to connect with a client ID (can be unique per node)
    if (client.connect("esp8266_client")) 
    {
      Serial.println("connected!");
      client.subscribe(subscribe_topic);                // Subscribe to command topic
    } else 
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());                     // Display connection error code
      Serial.println(". Retrying in 2s...");
      delay(2000);
    }
  }
}

// =============== LED Blink Helper Function ======================
void blink(void)
{
  digitalWrite(ledPin, LOW);                            // LED ON (active LOW)
  delay(50);
  digitalWrite(ledPin, HIGH);                           // LED OFF
}

// ======================= Setup Function =========================
void setup() 
{
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);                           // Ensure LED is OFF initially

  Serial.begin(115200);                                 // Start UART for debugging
  delay(10);                                            // Give time for UART to settle

  connectWiFi();                                        // Connect to WiFi

  client.setServer(mqtt_server, mqtt_port);             // Set MQTT broker details
  client.setCallback(mqttCallback);                     // Set MQTT receive callback
}

// ======================== Main Loop =============================
void loop() 
{
  // Maintain MQTT connection and process incoming messages
  if (!client.connected()) 
  {
    connectMQTT();
  }
  client.loop();

  // Periodic publishing of dummy data every 5 seconds
  static unsigned long lastPublish = 0;
  if (millis() - lastPublish > 5000) 
  {
    lastPublish = millis();

    static int dummySensor = 1;
    char payload[64];
    snprintf(payload, sizeof(payload), "{\"adc\": %d}", ++dummySensor);// Simulated sensor payload

    Serial.print("Publishing: ");
    Serial.println(payload);      
    client.publish(publish_topic, payload);             // Send data to broker
    blink();                                            // Blink on publish
  }
}
// ======================== END =============================