
---

## 📋 Requirements

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
