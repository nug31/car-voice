# ESP8266 Car Voice Control Setup Guide

## 🔧 Hardware Requirements

### Components Needed:
- **ESP8266** (NodeMCU or Wemos D1 Mini)
- **8-Channel Relay Module** (5V or 3.3V compatible)
- **Jumper wires**
- **Breadboard** (optional)
- **12V car accessories** to control (optional for testing)

## 📋 Wiring Diagram

### ESP8266 to 8-Channel Relay Module:

```
ESP8266 Pin  →  Relay Module Pin  →  Function
D1           →  IN1               →  Engine Start/Stop
D2           →  IN2               →  Trunk Open
D3           →  IN3               →  Trunk Close
D4           →  IN4               →  Lights
D5           →  IN5               →  Horn
D6           →  IN6               →  Door Lock
D7           →  IN7               →  Door Unlock
D8           →  IN8               →  Spare/Custom

3.3V         →  VCC               →  Power (if 3.3V relay)
GND          →  GND               →  Ground
```

**Note**: If using 5V relay module, connect VCC to VIN (5V) on ESP8266.

## 💻 Software Setup

### 1. Install Arduino IDE
- Download from: https://www.arduino.cc/en/software
- Install ESP8266 board package

### 2. Install Required Libraries
In Arduino IDE, go to **Tools → Manage Libraries** and install:
- `ESP8266WiFi` (usually pre-installed)
- `ArduinoJson` by Benoit Blanchon

### 3. Upload the Code
1. Open `esp8266_car_control.ino` in Arduino IDE
2. **IMPORTANT**: Update WiFi credentials:
   ```cpp
   const char* ssid = "YOUR_WIFI_NAME";        // Replace with your WiFi name
   const char* password = "YOUR_WIFI_PASSWORD"; // Replace with your WiFi password
   ```
3. Select your ESP8266 board:
   - **Tools → Board → ESP8266 Boards → NodeMCU 1.0** (or your specific board)
4. Select the correct COM port: **Tools → Port**
5. Click **Upload** button

### 4. Find ESP8266 IP Address
1. Open **Tools → Serial Monitor** (set baud rate to 115200)
2. Reset the ESP8266
3. Look for output like: `Connected! IP address: 192.168.1.100`
4. **Write down this IP address** - you'll need it for the web app

## 🌐 Web App Configuration

### 1. Connect to ESP8266
1. Open your car voice app: https://car-voice.netlify.app
2. Go to **Setup** section
3. Select **IoT Sensors** card
4. Choose **ESP8266 WiFi (8-Channel Relay)** from dropdown
5. Enter the IP address from step 4 above
6. Click **Connect IoT Sensors**

### 2. Test Connection
- You should see: `✅ Connected to ESP8266 at [IP_ADDRESS]`
- Check browser console (F12) for connection logs

## 🎮 Testing Your Setup

### Voice Commands That Control Hardware:
- **"Start engine"** → Activates Relay 1 (Engine Start)
- **"Stop engine"** → Deactivates Relay 1 (Engine Stop)
- **"Open trunk"** or **"Buka bagasi"** → Pulse Relay 2 (Trunk Open)
- **"Close trunk"** or **"Tutup bagasi"** → Pulse Relay 3 (Trunk Close)
- **"Active all"** → Runs full sequence with real hardware

### Manual Testing:
You can also test individual relays by sending HTTP requests:

```bash
# Get status
curl http://192.168.1.100/status

# Start engine (Relay 1 ON)
curl -X POST http://192.168.1.100/engine/start

# Stop engine (Relay 1 OFF)
curl -X POST http://192.168.1.100/engine/stop

# Open trunk (Relay 2 pulse)
curl -X POST http://192.168.1.100/trunk/open

# Close trunk (Relay 3 pulse)
curl -X POST http://192.168.1.100/trunk/close

# Control any relay manually
curl -X POST http://192.168.1.100/control \
  -H "Content-Type: application/json" \
  -d '{"channel": 4, "state": true, "duration": 1000}'
```

## 🔧 Relay Functions Mapping

| Relay | Function | Voice Command | Usage |
|-------|----------|---------------|-------|
| 1 | Engine Start/Stop | "start engine", "stop engine" | Car ignition control |
| 2 | Trunk Open | "open trunk", "buka bagasi" | Trunk release mechanism |
| 3 | Trunk Close | "close trunk", "tutup bagasi" | Trunk close mechanism |
| 4 | Lights | - | Headlights/interior lights |
| 5 | Horn | - | Car horn |
| 6 | Door Lock | - | Central door locking |
| 7 | Door Unlock | - | Central door unlocking |
| 8 | Spare | - | Custom function |

## 🚗 Connecting to Real Car Systems

### Safety First! ⚠️
- **Never connect directly to car's main electrical system**
- Use relays to isolate ESP8266 from car's 12V system
- Test with LEDs first before connecting real car components
- Consider using optocouplers for additional isolation

### Example Car Connections:
1. **Engine Start**: Connect relay to car's starter button circuit
2. **Trunk**: Connect to trunk release solenoid
3. **Lights**: Connect to headlight relay circuit
4. **Horn**: Connect to horn relay

### Testing Setup (Safe):
- Connect LEDs to relay outputs for visual testing
- Use 12V LED strips to simulate car accessories
- Test all functions before car installation

## 🐛 Troubleshooting

### ESP8266 Won't Connect to WiFi:
- Check WiFi credentials in code
- Ensure 2.4GHz WiFi (ESP8266 doesn't support 5GHz)
- Check WiFi signal strength

### Web App Can't Connect:
- Verify ESP8266 IP address
- Check if ESP8266 and computer are on same network
- Try accessing `http://[ESP8266_IP]/status` directly in browser

### Relays Not Working:
- Check wiring connections
- Verify relay module voltage (3.3V vs 5V)
- Test with multimeter
- Check relay module LED indicators

### Voice Commands Not Working:
- Ensure ESP8266 is connected in web app
- Check browser console for errors
- Test with manual HTTP requests first

## 📱 Advanced Features

### Add More Voice Commands:
Edit the web app to add custom voice commands that control specific relays.

### MQTT Integration:
Modify ESP8266 code to publish status to MQTT broker for integration with home automation systems.

### Mobile App:
Create a dedicated mobile app that connects directly to ESP8266 for car control.

## 🔗 API Endpoints

Your ESP8266 provides these REST API endpoints:

- `GET /status` - Get all relay states
- `POST /control` - Control specific relay
- `POST /engine/start` - Start engine (Relay 1)
- `POST /engine/stop` - Stop engine (Relay 1)
- `POST /trunk/open` - Open trunk (Relay 2)
- `POST /trunk/close` - Close trunk (Relay 3)

Perfect for integration with other systems!
