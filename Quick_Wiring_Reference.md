# ⚡ Quick Wiring Reference

## 🔌 **ESP8266 to Relay Module - Quick Connect**

### **Power (Choose One):**
```
ESP8266 VIN (5V) ──► Relay VCC    (Recommended for 5V relays)
ESP8266 3.3V     ──► Relay VCC    (For 3.3V relays only)
ESP8266 GND      ──► Relay GND    (Always required)
```

### **Control Signals:**
```
ESP8266 Pin    Relay Input    Function
===========    ===========    ========
D0 (GPIO16) ──► IN1        ──► REM (Remote)
D5 (GPIO14) ──► IN2        ──► ENGINE (Start)
D8 (GPIO15) ──► IN3        ──► KACA1 (Window Down)
D3 (GPIO0)  ──► IN4        ──► KACA2 (Window Up)
D2 (GPIO4)  ──► IN5        ──► BAGASI1 (Trunk Open)
D1 (GPIO5)  ──► IN6        ──► BAGASI2 (Trunk Close)
D6 (GPIO12) ──► IN7        ──► LIGHTS (Optional)
D7 (GPIO13) ──► IN8        ──► HORN (Optional)
```

## 🎯 **Minimum Required Connections**

For basic engine and trunk control, you only need:

```
Power:
ESP8266 VIN ──► Relay VCC
ESP8266 GND ──► Relay GND

Control (Minimum 4 wires):
D0 ──► IN1 (REM)
D5 ──► IN2 (ENGINE)  
D2 ──► IN5 (BAGASI1 - Trunk Open)
D1 ──► IN6 (BAGASI2 - Trunk Close)
```

## 🚗 **Voice Commands After Wiring**

### **Google Assistant:**
- "Hey Google, turn on Device 1" → Engine Start
- "Hey Google, turn on Device 3" → Trunk Open/Close

### **Car Voice App:**
- "Start engine" → Engine Start
- "Open trunk" / "Buka bagasi" → Trunk Open
- "Close trunk" / "Tutup bagasi" → Trunk Close

## ⚠️ **Safety First**
1. **Test with LEDs first** before connecting to car
2. **Use fuses** on car connections
3. **Never connect directly** to car's main electrical system
4. **Double-check polarity** before powering on

## 🔧 **Testing Steps**
1. **Wire ESP8266 to relay module**
2. **Upload code** (`esp8266_car_control.ino`)
3. **Check Serial Monitor** for IP address
4. **Test relays** with voice commands
5. **Connect to car circuits** (with proper safety measures)

Ready to wire and test! 🎉
