/*
 * ESP8266 MQTT Serial Bridge
 * 
 * Author: Nishant Thosar
 * Date: July 12, 2025
 * 
 * PROGRAM DESCRIPTION:
 * This program creates a bridge between serial communication and MQTT messaging
 * using an ESP8266 microcontroller. The ESP8266 connects to a WiFi network and
 * establishes a connection to an MQTT broker. It then listens for serial commands
 * in a specific format and publishes them as MQTT messages.
 * 
 * FUNCTIONALITY:
 * 1. WiFi Connection: Connects to the specified WiFi network (CDAC)
 * 2. MQTT Connection: Establishes connection to MQTT broker at 10.182.7.185
 * 3. Serial Command Processing: Parses serial input in format "topic=<topic>;msg=<message>;"
 * 4. MQTT Publishing: Publishes parsed messages to specified MQTT topics
 * 5. Status Monitoring: Maintains connection status and handles reconnections
 * 
 * SERIAL COMMAND FORMAT:
 * topic=sensor/temperature;msg=25.6;
 * 
 * HARDWARE REQUIREMENTS:
 * - ESP8266 development board (ESP-01, NodeMCU, Wemos D1, etc.)
 * - USB to Serial adapter (if using ESP-01)
 * - Access to WiFi network "CDAC"
 * - MQTT broker running on network at 10.182.7.185:1883
 * 
 * DEPENDENCIES:
 * - ESP8266WiFi library (for WiFi connectivity)
 * - PubSubClient library (for MQTT communication)
 */

// Include required libraries for WiFi and MQTT functionality
#include <ESP8266WiFi.h>  // ESP8266 WiFi library for network connectivity
#include <PubSubClient.h> // MQTT client library for pub/sub messaging

// WiFi network credentials
const char* ssid = "CDAC";        // WiFi network name (SSID)
const char* password = "";        // WiFi password (empty for open network)

// MQTT broker configuration
const char* mqtt_server = "10.182.7.185";  // MQTT broker IP address
const int mqtt_port = 1883;                 // Standard MQTT port (unencrypted)

// Client objects for network communication
WiFiClient espClient;           // WiFi client for network connection
PubSubClient client(espClient); // MQTT client using WiFi connection

// Global variables for serial command processing and status tracking
String incomingSerial = "";     // Buffer to store incoming serial data
bool mqtt_ready = false;        // Flag to track MQTT connection status

/*
 * Function: connectWiFi()
 * Purpose: Establishes WiFi connection to the specified network
 * 
 * This function initiates the WiFi connection process and waits until
 * the ESP8266 successfully connects to the network. It provides status
 * updates via serial output and displays connection information once
 * connected including IP address and MAC address.
 */
void connectWiFi() 
{
  // Display connection attempt information
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  
  // Initiate WiFi connection with network credentials
  WiFi.begin(ssid, password);
  Serial.print("\n\r");
  
  // Wait for WiFi connection to establish
  // Display progress dots while connecting
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);           // Wait 500ms between connection attempts
    Serial.print(".");    // Print progress indicator
  }
  
  // Connection successful - display network information
  Serial.println("\nWiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());    // Show assigned IP address
  
  // Display MAC address for network identification
  Serial.print("\nESP MAC Address: ");
  Serial.println(WiFi.macAddress());
}

/*
 * Function: mqttCallback()
 * Purpose: Handles incoming MQTT messages from subscribed topics
 * Parameters:
 *   - topic: Character array containing the topic name
 *   - payload: Byte array containing the message data
 *   - length: Length of the payload in bytes
 * 
 * This callback function is automatically called whenever a message
 * is received on any subscribed MQTT topic. Currently, it only prints
 * the received message to serial output for monitoring purposes.
 */
void mqttCallback(char* topic, byte* payload, unsigned int length) 
{
  // Display the topic that received a message
  Serial.print("MQTT Message Received [");
  Serial.print(topic);
  Serial.print("]: ");
  
  // Convert payload bytes to characters and print message content
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();  // Add newline after message
}

/*
 * Function: connectMQTT()
 * Purpose: Establishes connection to the MQTT broker
 * 
 * This function attempts to connect to the MQTT broker using a unique
 * client ID. It will retry indefinitely until connection is successful.
 * Once connected, it sets the mqtt_ready flag to enable serial command
 * processing.
 */
void connectMQTT() {
  // Keep trying to connect until successful
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    
    // Attempt connection with unique client identifier
    if (client.connect("esp01_client")) {
      Serial.println("Connected to MQTT broker!\n");
      mqtt_ready = true;  // Enable serial command processing
    } else {
      // Connection failed - display error code and retry
      Serial.print("failed, rc=");
      Serial.print(client.state());  // Print MQTT client state for debugging
      Serial.println(". Retrying in 2s...");
      delay(2000);  // Wait 2 seconds before retry
    }
  }
}

/*
 * Function: parseAndPublish()
 * Purpose: Parses serial command and publishes MQTT message
 * Parameters:
 *   - cmd: String containing the serial command to parse
 * 
 * Expected command format: "topic=<topic_name>;msg=<message_content>;"
 * Example: "topic=sensor/temperature;msg=25.6;"
 * 
 * This function extracts the topic and message from the command string
 * and publishes the message to the specified MQTT topic. If the format
 * is invalid, it displays an error message.
 */
void parseAndPublish(String cmd) {
  String topic = "";  // Variable to store extracted topic
  String msg = "";    // Variable to store extracted message
  
  // Find the positions of topic and message parameters
  int topicIndex = cmd.indexOf("topic=");
  int msgIndex = cmd.indexOf("msg=");
  
  // Extract topic from command string
  if (topicIndex >= 0) {
    int end = cmd.indexOf(";", topicIndex);  // Find semicolon delimiter
    topic = cmd.substring(topicIndex + 6, end);  // Extract topic (skip "topic=")
  }
  
  // Extract message from command string
  if (msgIndex >= 0) {
    int end = cmd.indexOf(";", msgIndex);    // Find semicolon delimiter
    msg = cmd.substring(msgIndex + 4, end);  // Extract message (skip "msg=")
  }
  
  // Validate that both topic and message were successfully extracted
  if (topic.length() > 0 && msg.length() > 0) {
    // Publish message to MQTT broker
    client.publish(topic.c_str(), msg.c_str());
    Serial.println("Published: " + msg + " to " + topic);
  } else {
    // Invalid command format - display error
    Serial.println("Invalid command format!");
    Serial.println("Expected format: topic=<topic>;msg=<message>;");
  }
}

/*
 * Function: checkSerialCommand()
 * Purpose: Monitors serial input and processes complete commands
 * 
 * This function reads incoming serial data character by character,
 * building complete command strings. When a newline character is
 * detected, it processes the complete command. The function only
 * operates when MQTT connection is ready to prevent data loss.
 * 
 * Command processing is line-based - each command must end with '\n'
 */
void checkSerialCommand() {
  // Only process serial commands if MQTT connection is established
  // This prevents commands from being lost during connection attempts
  if (!mqtt_ready) return;
  
  // Process all available serial data
  while (Serial.available()) {
    char c = Serial.read();  // Read one character from serial buffer
    
    if (c == '\n') {
      // Complete command received (newline indicates end of command)
      parseAndPublish(incomingSerial);  // Process the complete command
      incomingSerial = "";              // Clear buffer for next command
    } else {
      // Build command string character by character
      incomingSerial += c;
    }
  }
}

/*
 * Function: setup()
 * Purpose: One-time initialization when ESP8266 starts
 * 
 * This function runs once when the ESP8266 powers up or resets.
 * It initializes serial communication, establishes WiFi connection,
 * and configures the MQTT client with broker details and callback function.
 */
void setup() {
  // Initialize serial communication at 115200 baud rate
  // High baud rate ensures fast communication with host device
  Serial.begin(115200);
  delay(10);  // Short delay to ensure serial is ready
  
  // Establish WiFi connection to network
  connectWiFi();
  
  // Configure MQTT client with broker server and port
  client.setServer(mqtt_server, mqtt_port);
  
  // Set callback function to handle incoming MQTT messages
  client.setCallback(mqttCallback);
}

/*
 * Function: loop()
 * Purpose: Main program loop - runs continuously
 * 
 * This function handles the main program execution cycle:
 * 1. Monitors MQTT connection status and reconnects if necessary
 * 2. Processes MQTT client operations (message handling, keep-alive)
 * 3. Checks for and processes serial commands
 * 
 * The loop ensures robust operation by automatically handling
 * connection failures and maintaining both MQTT and serial communication.
 */
void loop() {
  // Check if MQTT connection is lost and attempt reconnection
  if (!client.connected()) {
    mqtt_ready = false;  // Disable serial processing during reconnection
    connectMQTT();       // Attempt to reconnect to MQTT broker
  }
  
  // Process MQTT client operations
  // This handles incoming messages, sends keep-alive packets,
  // and maintains the connection to the broker
  client.loop();
  
  // Check for and process any incoming serial commands
  checkSerialCommand();
}

/*
 * USAGE INSTRUCTIONS:
 * 
 * 1. Hardware Setup:
 *    - Connect ESP8266 to power and programming interface
 *    - Ensure ESP8266 can access WiFi network "CDAC"
 *    - Verify MQTT broker is running on 10.182.7.185:1883
 * 
 * 2. Programming:
 *    - Upload this code to ESP8266 using Arduino IDE
 *    - Open Serial Monitor at 115200 baud rate
 *    - Wait for WiFi and MQTT connection messages
 * 
 * 3. Sending Commands:
 *    - Use serial interface to send commands
 *    - Format: topic=<your_topic>;msg=<your_message>;
 *    - Example: topic=sensors/temp;msg=23.5;
 *    - Press Enter after each command
 * 
 * 4. Monitoring:
 *    - Serial monitor shows connection status
 *    - Confirms successful message publishing
 *    - Displays any received MQTT messages
 * 
 * TROUBLESHOOTING:
 * - If WiFi won't connect: Check SSID and password
 * - If MQTT fails: Verify broker IP and port accessibility
 * - If commands ignored: Ensure MQTT connection is established first
 * - For connection issues: Check serial output for error codes
 */