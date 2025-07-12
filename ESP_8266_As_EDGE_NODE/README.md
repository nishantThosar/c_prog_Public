# ESP8266 MQTT Edge Node Firmware

📅 **Date**: July 12, 2025  
✍️ **Author**: [Nishant Thosar]  
📦 **Platform**: ESP8266 (NodeMCU, ESP-01, etc.)  
📡 **Broker**: Raspberry Pi running Mosquitto (local MQTT broker)

---

## 🔧 Overview

This firmware enables an ESP8266-based IoT edge node to connect to a local MQTT broker hosted on a Raspberry Pi. It supports:

- 📤 Periodic **publishing of sensor data**
- 📥 **Subscribing to commands** from the broker (e.g., controlling onboard LED)
- 📡 LED indications for status feedback and activity
- 🔐 Easily configurable for secure or open MQTT brokers

---

## 📁 Project Structure

```
esp8266_mqtt_edge_node/
├── esp8266_mqtt_edge_node.ino   # Main firmware sketch
├── README.md                    # This documentation
```

---

## 📋 Requirements

### 🧰 Hardware

- ESP8266 board (NodeMCU or ESP-01)
- USB-to-Serial adapter (for ESP-01)
- Raspberry Pi with Mosquitto installed
- Common components (LED for testing, optional sensors)

### 💻 Software

- Arduino IDE (tested on Ubuntu 22.04+)
- ESP8266 Board Package (via Board Manager)
- Required Libraries:
  - `ESP8266WiFi` (built-in with ESP8266 core)
  - `PubSubClient` (install via Library Manager)

---

## 🖥️ Raspberry Pi Broker Configuration (Anonymous Access)

If you're running an **open MQTT broker** (without password), follow these steps:

1. **Install Mosquitto (if not done yet)**:
   ```bash
   sudo apt update
   sudo apt install mosquitto mosquitto-clients
   ```

2. **Edit the config file**:
   ```bash
   sudo nano /etc/mosquitto/mosquitto.conf
   ```

3. **Add or ensure the following lines**:
   ```conf
   listener 1883
   allow_anonymous true
   ```

4. **Restart Mosquitto**:
   ```bash
   sudo systemctl restart mosquitto
   ```

5. **Get your Raspberry Pi IP**:
   ```bash
   hostname -I
   ```

> 🔐 If you later want password-protected access, we support that too (see Secure Mode section below).

---

## 📡 MQTT Topics

| Topic                          | Direction | Description                         |
|--------------------------------|-----------|-------------------------------------|
| `edge_node/esp8266/data`       | Publish   | Sends sensor data to broker         |
| `edge_node/esp8266/cmd`        | Subscribe | Receives commands (e.g., `LED_ON`)  |

📦 Example topic hierarchy for future expansion:
```
edge_nodes/
├── esp8266/
│   ├── data
│   └── cmd
├── esp01/
│   ├── data
│   └── cmd
├── node2/
│   └── ...
```

---

## 🧪 Example MQTT Messages

- **Published JSON Payload**:
  ```json
  {"adc": 5}
  ```

- **Received Command** (sent from MQTT client):
  ```
  LED_ON
  ```

---

## 🛠️ Arduino IDE Setup

1. **Install ESP8266 Board Support**:
   - Go to `File > Preferences` → paste this URL into **Additional Board URLs**:
     ```
     http://arduino.esp8266.com/stable/package_esp8266com_index.json
     ```
   - Then go to `Tools > Board > Boards Manager` → install **"ESP8266 by ESP8266 Community"**

2. **Select Your Board**:
   - ESP-01 → `Generic ESP8266 Module`
   - NodeMCU → `NodeMCU 1.0 (ESP-12E Module)`

3. **Select Correct Port and Upload Settings**:
   - Baud rate: `115200`
   - Flash size: Match your ESP type
   - Programmer: `AVR ISP` (default is fine)

4. **Install PubSubClient Library**:
   - Go to `Sketch > Include Library > Manage Libraries...`
   - Search for **PubSubClient** and install it

---

## ⚡ How to Flash ESP8266

### For **ESP-01**:
1. Connect USB-to-Serial as:
   - TX ↔ RX
   - RX ↔ TX
   - GND ↔ GND
   - VCC ↔ 3.3V (⚠️ Not 5V!)
   - CH_PD ↔ 3.3V
   - GPIO0 ↔ GND (for flashing mode)

2. Open Arduino IDE → select board/port → upload the sketch.

3. After flashing:
   - Disconnect GPIO0 from GND
   - Power cycle the ESP-01

### For **NodeMCU**:
- Just connect via USB and click upload. Auto-flash is handled by onboard circuitry.

---

## 🔁 Behavior Summary

| State             | Behavior                                |
|------------------|------------------------------------------|
| WiFi Connecting  | Blinking LED with serial dots (`.`)      |
| WiFi Connected   | Shows IP address on Serial Monitor       |
| MQTT Connected   | Subscribes to command topic              |
| Publishing       | Publishes mock ADC data every 5 seconds |
| Command Received | Controls LED based on command (`LED_ON`) |

---

## 🛡️ Optional: Secure Mode (Username/Password)

Edit Mosquitto config:
```conf
listener 1883
allow_anonymous false
password_file /etc/mosquitto/passwd
```

Then set password:
```bash
sudo mosquitto_passwd -c /etc/mosquitto/passwd espuser
sudo systemctl restart mosquitto
```

Update Arduino code:
```cpp
client.connect("esp8266_client", "espuser", "your_password");
```

---

## 🧠 To-Do (for expansion)

- [ ] Replace dummy ADC with actual sensor (DHT, analogRead, etc.)
- [ ] Add TLS support (MQTTS on port 8883)
- [ ] Integrate LWT (Last Will and Testament)
- [ ] Add OTA updates

---

## ⚖️ License

MIT License  
Feel free to use, modify, and distribute this project. Attribution is appreciated but not required.

---

## 💬 Support & Contributions

- Need help? Found a bug? Want to contribute improvements?
- Feel free to open issues or submit pull requests.

📧 Contact: `you@example.com` (replace with your actual address)

---

## 🙏 Acknowledgements

- [ESP8266 Arduino Core](https://github.com/esp8266/Arduino)
- [PubSubClient by Nick O'Leary](https://github.com/knolleary/pubsubclient)
- Mosquitto MQTT by Eclipse Foundation

---

🚀 **Built for scalable, secure edge-node IoT systems**.  
Ready for production. Ready for expansion.
