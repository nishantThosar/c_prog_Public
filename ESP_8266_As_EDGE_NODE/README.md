
---

## 📋 Requirements for Slave microcontroller

### 🧰 Hardware
- ESP-01 (ESP8266 module, flashed via USB-to-UART adapter)
- STM32F407 Discovery board (or compatible STM32)
- 3.3V supply (ESP-01 requires 3.3V, not 5V)
- Logic-level shifting (recommended for STM32 → ESP-01 TX)

### 💻 Software
- Arduino IDE (tested on Ubuntu 24.04)
- ESP8266 Board Package (via Board Manager)
- Required Libraries:
  - `ESP8266WiFi`
  - `PubSubClient`

---

## ⚡ Features

- 📶 ESP-01 connects to WiFi network (SSID + password sent by STM32).
- 🔗 Connects to MQTT broker (IP/hostname sent by STM32).
- 📤 Publishes messages received from STM32 over UART to broker.
- 📥 Subscribes to given MQTT topic and relays messages back to STM32 via UART.
- 🟢 Status feedback over Serial for debugging.

---

## 📡 MQTT Topics (Example)

| Topic                        | Direction  | Description                        |
|------------------------------|------------|------------------------------------|
| `edge_node/esp01/data`       | Publish    | Data sent from STM32 → Broker      |
| `edge_node/esp01/cmd`        | Subscribe  | Commands from Broker → STM32       |

---

## 🛠️ Arduino IDE Setup (ESP-01)

1. **Install ESP8266 board support**  
   - Add this URL to *Preferences → Additional Board URLs*:  
     ```
     http://arduino.esp8266.com/stable/package_esp8266com_index.json
     ```
   - Then install **ESP8266 by ESP8266 Community** in *Boards Manager*.

2. **Select your board**  
   - `Generic ESP8266 Module`

3. **Flash settings**  
   - Upload speed: `115200`  
   - Flash size: `1MB (512K SPIFFS)` or as per ESP-01 version  

4. **Install required libraries**  
   - `PubSubClient` (via Library Manager)  

---

## 🔌 STM32 → ESP-01 UART Interface

- Default Baud: **115200**  
- Data format: **8N1**  
- Communication:  
  - STM32 sends setup strings (WiFi credentials, broker IP, topics).  
  - ESP-01 responds with status and handles MQTT tasks.  

---

## 🔁 Behavior Summary
| State            | Behavior                          |
| ---------------- | --------------------------------- |
| WiFi Connecting  | Serial prints dots (`.`)          |
| WiFi Connected   | IP shown on Serial Monitor        |
| MQTT Connected   | Subscribes to configured topic    |
| Publishing       | Data from STM32 relayed to broker |
| Command Received | Relayed back to STM32 over UART   |

---

## 🧠 To-Do (Future Improvements)

 1)Add TLS support (MQTTS on port 8883).
 2)Implement Last Will & Testament (LWT).
 3)Add OTA firmware updates for ESP-01.
 4)Extend STM32 command set for sensor integration.

---

## ⚖️ License

MIT License
Use, modify, and distribute freely. Attribution appreciated.

## 🧪 Example Communication

**STM32 sends over UART:**
STM32 sending the wifi SSID and Password via UART4 : "wifi=Nishant;pass=Temp \n"

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

---

## 📋 Requirements for Master Microcontroller

### 🧰 Hardware
- STM32F407 Discovery (tested, but portable to other STM32 boards)  
- ESP-01 module (pre-flashed with `ESP_01_STM32_ver2_3.ino` from this repo)  
- UART4 wired between STM32 and ESP-01 (115200 baud, 8N1)  
- 3.3V logic-level compatibility (ESP-01 is not 5V tolerant)  

### 💻 Software
- STM32CubeIDE or STM32 HAL-based project  
- UART + DMA enabled for UART4  
- Example uses `USART2` for debugging (printf to serial terminal)  

---

## ⚡ Features

- 🛰️ Send WiFi SSID & password to ESP-01  
- 🔐 Send MQTT broker IP, port, and optional username/password  
- 📡 Publish messages to MQTT topic  
- 📥 Subscribe to MQTT topic and receive messages from ESP-01  
- 🧩 Publish structured JSON payloads  
- 🔄 Reset ESP-01 remotely via command  
- 📊 Debug counters for successful/failed transmissions  

---

## 🛠️ Example Usage

### Configure Credentials (inside `main.c`)

```c
const char *wifi_ssid       = "YourWiFiSSID";
const char *wifi_password   = "YourWiFiPassword";

const char *mqtt_broker_ip  = "192.168.1.100";
const char *mqtt_username   = "user";
const char *mqtt_password   = "pass";
int         mqtt_port       = 1883;

const char *mqtt_pub_topic  = "esp01/send";
const char *mqtt_sub_topic  = "esp01/rec";
```
---
stm32_mqtt_bridge/
├── main.c # Example application using the API
├── ESP_Communication.c # Communication logic implementation
├── ESP_Communication.h # Public API and definitions
---

## Initialize the communication (Inside main.c)

reset_mqtt(&huart4);   // Reset ESP-01
send_wifi_credentials(&huart4, wifi_ssid, wifi_password);
HAL_Delay(100);

send_subscribe_topic(&huart4, mqtt_sub_topic);

send_mqtt_credentials(&huart4, mqtt_broker_ip,
                      mqtt_username, mqtt_password,
                      mqtt_port, 1);  // 1 = authentication required
HAL_Delay(3000);

---

## Publish Messages (Inside main.c)
send_mqtt_message(&huart4, mqtt_pub_topic, "Hello from STM32", 0);

// Or send JSON-formatted payload
esp_publish_json_message(&huart4, "esp01/send/json", 0, 2,
                         "temperature", "25",
                         "humidity", "60");
---

The incoming subscribed message appears in the "AppBuffer" which can be seen in the "Live Expression" in STMcubeIDE.


### 🔁 Behavior Summary
STM32 Function	Effect on ESP-01 → MQTT Broker
1) send_wifi_credentials()	      Configures WiFi SSID/password.
2) send_mqtt_credentials()	      Configures broker info + auth.
3) send_mqtt_message()	          Publishes a plain text message.
4) esp_publish_json_message()	    Publishes JSON payload.
5) send_subscribe_topic()	        Subscribes to the broker topic.
6) reset_mqtt()	                  Resets ESP-01 module.

---

## 🧠 Future scope

 Add more robust message parsing for STM32 received data.

 Buffer incoming messages in a queue.

 Support dynamic reconfiguration at runtime.

 Add an optional encryption/authentication layer over UART.

---

## ⚖️ License

MIT License – free to use and modify. Attribution appreciated.

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
