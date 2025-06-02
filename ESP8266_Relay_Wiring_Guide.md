# 🔌 ESP8266 to Relay Module Wiring Guide

## 📋 **Your Pin Configuration (From Code)**

Based on your existing code, here's the exact pin mapping:

```cpp
#define REM                16  // Remote/Engine control
#define ENGINE             14  // Engine start
#define KACA1              15  // Window down
#define KACA2              0   // Window up
#define BAGASI1            4   // Trunk open
#define BAGASI2            5   // Trunk close
#define LIGHTS_PIN         12  // Lights control (optional)
#define HORN_PIN           13  // Horn control (optional)
```

## 🔧 **Wiring Diagram**

### **ESP8266 NodeMCU to Relay Modules:**

```
ESP8266 NodeMCU          Relay Module
================         ============
3.3V        ────────────► VCC (if 3.3V relay)
VIN (5V)    ────────────► VCC (if 5V relay)
GND         ────────────► GND

D0 (GPIO16) ────────────► IN1 (REM - Remote Control)
D5 (GPIO14) ────────────► IN2 (ENGINE - Engine Start)
D8 (GPIO15) ────────────► IN3 (KACA1 - Window Down)
D3 (GPIO0)  ────────────► IN4 (KACA2 - Window Up)
D2 (GPIO4)  ────────────► IN5 (BAGASI1 - Trunk Open)
D1 (GPIO5)  ────────────► IN6 (BAGASI2 - Trunk Close)
D6 (GPIO12) ────────────► IN7 (LIGHTS - Optional)
D7 (GPIO13) ────────────► IN8 (HORN - Optional)
```

## 🎯 **Visual Wiring Diagram**

```
                    ESP8266 NodeMCU
                    ┌─────────────────┐
                    │                 │
               3.3V │●               ●│ VIN (5V)
                GND │●               ●│ GND
                 D0 │●               ●│ D1 ──────► IN6 (BAGASI2)
                 D1 │●               ●│ D2 ──────► IN5 (BAGASI1)
                 D2 │●               ●│ D3 ──────► IN4 (KACA2)
                 D3 │●               ●│ D4
                 D4 │●               ●│ D5 ──────► IN2 (ENGINE)
                 D5 │●               ●│ D6 ──────► IN7 (LIGHTS)
                 D6 │●               ●│ D7 ──────► IN8 (HORN)
                 D7 │●               ●│ D8 ──────► IN3 (KACA1)
                 D8 │●               ●│ RX
                    │                 │
                    └─────────────────┘
                            │
                           D0 ──────► IN1 (REM)

                    8-Channel Relay Module
                    ┌─────────────────────────┐
                    │ IN1 IN2 IN3 IN4 IN5 IN6 │
                    │  ●   ●   ●   ●   ●   ●  │
                    │ IN7 IN8 VCC GND         │
                    │  ●   ●   ●   ●          │
                    └─────────────────────────┘
```

## ⚡ **Power Connections**

### **Option 1: 3.3V Relay Module**
```
ESP8266 3.3V ────► Relay VCC
ESP8266 GND  ────► Relay GND
```

### **Option 2: 5V Relay Module (Recommended)**
```
ESP8266 VIN (5V) ────► Relay VCC
ESP8266 GND      ────► Relay GND
```

**Note:** Most relay modules work better with 5V for reliable switching.

## 🔌 **Detailed Pin Connections**

| ESP8266 Pin | GPIO | Relay Input | Function | Car Connection |
|-------------|------|-------------|----------|----------------|
| D0 | GPIO16 | IN1 | REM | Remote Control Signal |
| D5 | GPIO14 | IN2 | ENGINE | Engine Start Signal |
| D8 | GPIO15 | IN3 | KACA1 | Window Down |
| D3 | GPIO0 | IN4 | KACA2 | Window Up |
| D2 | GPIO4 | IN5 | BAGASI1 | Trunk Open |
| D1 | GPIO5 | IN6 | BAGASI2 | Trunk Close |
| D6 | GPIO12 | IN7 | LIGHTS | Headlights/Interior |
| D7 | GPIO13 | IN8 | HORN | Car Horn |

## 🚗 **Relay to Car Connections**

### **Each Relay Output:**
```
Relay 1 (REM):     NO ──► Car Remote Control Circuit
                   COM ──► Ground/Common
                   NC ──► (Not used)

Relay 2 (ENGINE):  NO ──► Car Engine Start Circuit  
                   COM ──► Ground/Common
                   NC ──► (Not used)

Relay 3 (KACA1):   NO ──► Window Down Circuit
                   COM ──► Ground/Common
                   NC ──► (Not used)

Relay 4 (KACA2):   NO ──► Window Up Circuit
                   COM ──► Ground/Common
                   NC ──► (Not used)

Relay 5 (BAGASI1): NO ──► Trunk Open Circuit
                   COM ──► Ground/Common
                   NC ──► (Not used)

Relay 6 (BAGASI2): NO ──► Trunk Close Circuit
                   COM ──► Ground/Common
                   NC ──► (Not used)
```

## ⚠️ **Safety Considerations**

### **IMPORTANT SAFETY NOTES:**
1. **Never connect directly to car's main electrical system**
2. **Use relays to isolate ESP8266 from car's 12V system**
3. **Test with LEDs first before connecting to car**
4. **Use fuses on car connections**
5. **Consider optocouplers for additional isolation**

### **Testing Setup (Safe):**
```
Relay Output ──► LED + Resistor ──► Ground
```
Use 12V LED strips to simulate car accessories safely.

## 🔧 **Step-by-Step Wiring Process**

### **Step 1: Power Connections**
1. Connect ESP8266 **VIN** to Relay **VCC** (5V)
2. Connect ESP8266 **GND** to Relay **GND**

### **Step 2: Control Signals**
1. **D0** → **IN1** (REM)
2. **D5** → **IN2** (ENGINE)  
3. **D8** → **IN3** (KACA1)
4. **D3** → **IN4** (KACA2)
5. **D2** → **IN5** (BAGASI1)
6. **D1** → **IN6** (BAGASI2)
7. **D6** → **IN7** (LIGHTS) - Optional
8. **D7** → **IN8** (HORN) - Optional

### **Step 3: Test Connections**
1. Upload the code to ESP8266
2. Open Serial Monitor (115200 baud)
3. Check for successful WiFi connection
4. Test relay activation with voice commands

## 🧪 **Testing Commands**

### **Google Assistant/Alexa:**
- "Hey Google, turn on Device 1" → REM + ENGINE relays
- "Hey Google, turn on Device 2" → KACA1 + KACA2 relays
- "Hey Google, turn on Device 3" → BAGASI1 relay

### **Car Voice Web App:**
- "Start engine" → REM + ENGINE relays
- "Open trunk" → BAGASI1 relay
- "Close trunk" → BAGASI2 relay

## 📊 **Expected Relay Behavior**

| Voice Command | Relays Activated | Duration |
|---------------|------------------|----------|
| Engine Start | REM + ENGINE | 2 seconds |
| Engine Stop | None | - |
| Window Control | KACA1 + KACA2 | 3 seconds |
| Trunk Open | BAGASI1 | 3 seconds |
| Trunk Close | BAGASI2 | 0.8 seconds |

## 🔍 **Troubleshooting**

### **Relays Not Clicking:**
- Check power connections (VCC/GND)
- Verify 5V supply to relay module
- Test with multimeter on relay inputs

### **ESP8266 Not Responding:**
- Check WiFi connection
- Verify code upload successful
- Check Serial Monitor for errors

### **Relays Clicking But No Car Response:**
- Check car circuit connections
- Verify relay output wiring (NO/COM)
- Test with LED first

Your ESP8266 should now be properly wired to control your car systems through both Google Assistant and the Car Voice web app! 🚗⚡
