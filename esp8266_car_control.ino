#ifdef ENABLE_DEBUG
   #define DEBUG_ESP_PORT Serial
   #define NODEBUG_WEBSOCKETS
   #define NDEBUG
#endif

#include <Arduino.h>
#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>
#elif defined(ESP32) || defined(ARDUINO_ARCH_RP2040)
  #include <WiFi.h>
  #include <WebServer.h>
#endif

#include <ArduinoJson.h>
#include "SinricPro.h"
#include "SinricProSwitch.h"

// WiFi credentials
#define WIFI_SSID         "Fortuner"
#define WIFI_PASS         "12345678"

// SinricPro credentials
#define APP_KEY           "e39fcbea-0652-4191-95de-b0ab5bcc8c85"
#define APP_SECRET        "8d1aa170-4289-45b9-9859-cde4b43c5c37-67142a78-8e1b-48b4-a39c-fa89489032fe"

// SinricPro device IDs
#define SWITCH_ID_1       "65f5c3d460d81635d1c92c37"  // Engine control
#define SWITCH_ID_2       "65f5c3f038f6f4a3cdc35605"  // Window control
#define SWITCH_ID_3       "65f5c41360d81635d1c92ceb"  // Trunk control

// Web server on port 80
#if defined(ESP8266)
  ESP8266WebServer server(80);
#elif defined(ESP32) || defined(ARDUINO_ARCH_RP2040)
  WebServer server(80);
#endif

// Pin definitions (matching your existing setup)
#define REM                16  // Remote/Engine control
#define ENGINE             14  // Engine start
#define KACA1              15  // Window down
#define KACA2              0   // Window up
#define BAGASI1            4   // Trunk open
#define BAGASI2            5   // Trunk close

// Additional pins for extended functionality
#define LIGHTS_PIN         12  // Lights control
#define HORN_PIN           13  // Horn control

#define BAUD_RATE         115200

// Relay states for web interface
bool relayStates[8] = {false, false, false, false, false, false, false, false};

// Relay functions mapping for web interface
enum RelayFunction {
  ENGINE_START = 0,
  TRUNK_OPEN = 1,
  TRUNK_CLOSE = 2,
  WINDOW_CONTROL = 3,
  LIGHTS = 4,
  HORN = 5,
  DOOR_LOCK = 6,
  SPARE = 7
};

// SinricPro callback functions (from your existing code)
bool onPowerState1(const String &deviceId, bool &state) {
  Serial.printf("Device 1 (Engine) turned %s\r\n", state?"on":"off");

  if (state) {
    // Start engine sequence
    digitalWrite(REM, HIGH);
    digitalWrite(ENGINE, HIGH);
    delay(2000);
    digitalWrite(REM, LOW);
    digitalWrite(ENGINE, LOW);
    relayStates[ENGINE_START] = true;
  } else {
    // Stop engine
    relayStates[ENGINE_START] = false;
  }

  return true;
}

bool onPowerState2(const String &deviceId, bool &state) {
  Serial.printf("Device 2 (Window) turned %s\r\n", state?"on":"off");

  digitalWrite(KACA1, state ? HIGH : LOW);
  digitalWrite(KACA2, state ? LOW : HIGH);
  delay(3000);
  digitalWrite(KACA1, LOW);
  digitalWrite(KACA2, LOW);
  relayStates[WINDOW_CONTROL] = state;

  return true;
}

bool onPowerState3(const String &deviceId, bool &state) {
  Serial.printf("Device 3 (Trunk) turned %s\r\n", state?"on":"off");

  if (state) {
    // Open trunk sequence
    digitalWrite(BAGASI1, HIGH);
    delay(3000);
    digitalWrite(BAGASI1, LOW);
    relayStates[TRUNK_OPEN] = true;
  } else {
    // Close trunk sequence
    digitalWrite(BAGASI2, HIGH);
    delay(800);
    digitalWrite(BAGASI2, LOW);
    relayStates[TRUNK_CLOSE] = true;
  }

  return true;
}

// WiFi setup function
void setupWiFi() {
  Serial.printf("\r\n[Wifi]: Connecting");

  #if defined(ESP8266)
    WiFi.setSleepMode(WIFI_NONE_SLEEP);
    WiFi.setAutoReconnect(true);
  #elif defined(ESP32)
    WiFi.setSleep(false);
    WiFi.setAutoReconnect(true);
  #endif

  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf(".");
    delay(250);
  }

  Serial.printf("connected!\r\n[WiFi]: IP-Address is %s\r\n", WiFi.localIP().toString().c_str());
}

// SinricPro setup function
void setupSinricPro() {
  // Initialize pins
  pinMode(REM, OUTPUT);
  pinMode(ENGINE, OUTPUT);
  pinMode(KACA1, OUTPUT);
  pinMode(KACA2, OUTPUT);
  pinMode(BAGASI1, OUTPUT);
  pinMode(BAGASI2, OUTPUT);
  pinMode(LIGHTS_PIN, OUTPUT);
  pinMode(HORN_PIN, OUTPUT);

  // Setup SinricPro switches
  SinricProSwitch& mySwitch1 = SinricPro[SWITCH_ID_1];
  mySwitch1.onPowerState(onPowerState1);

  SinricProSwitch& mySwitch2 = SinricPro[SWITCH_ID_2];
  mySwitch2.onPowerState(onPowerState2);

  SinricProSwitch& mySwitch3 = SinricPro[SWITCH_ID_3];
  mySwitch3.onPowerState(onPowerState3);

  // Setup SinricPro callbacks
  SinricPro.onConnected([](){ Serial.printf("Connected to SinricPro\r\n"); });
  SinricPro.onDisconnected([](){ Serial.printf("Disconnected from SinricPro\r\n"); });

  SinricPro.begin(APP_KEY, APP_SECRET);
}

void setup() {
  Serial.begin(BAUD_RATE);
  Serial.printf("\r\n\r\n=== Car Voice Control with SinricPro Integration ===\r\n");

  // Setup WiFi
  setupWiFi();

  // Setup SinricPro for Google Assistant/Alexa
  setupSinricPro();

  // Setup web server routes for car voice app
  setupRoutes();

  // Start web server
  server.begin();
  Serial.println("\r\n=== Car Control Server Started! ===");
  Serial.printf("Web Interface: http://%s\r\n", WiFi.localIP().toString().c_str());
  Serial.println("Available endpoints:");
  Serial.println("GET  /status - Get all relay states");
  Serial.println("POST /control - Control relays");
  Serial.println("POST /engine/start - Start engine");
  Serial.println("POST /engine/stop - Stop engine");
  Serial.println("POST /trunk/open - Open trunk");
  Serial.println("POST /trunk/close - Close trunk");
  Serial.println("\r\nSinricPro: Ready for Google Assistant/Alexa commands");
  Serial.println("Car Voice App: Ready for web interface commands");
}

void loop() {
  SinricPro.handle();  // Handle SinricPro (Google Assistant/Alexa)
  server.handleClient(); // Handle web server (Car Voice App)
}

void setupRoutes() {
  // Enable CORS for web app
  server.onNotFound([]() {
    if (server.method() == HTTP_OPTIONS) {
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.sendHeader("Access-Control-Allow-Methods", "GET,POST,OPTIONS");
      server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
      server.send(200);
    } else {
      server.send(404, "text/plain", "Not found");
    }
  });

  // Get status of all relays
  server.on("/status", HTTP_GET, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");

    DynamicJsonDocument doc(1024);
    doc["device"] = "ESP8266 Car Control";
    doc["ip"] = WiFi.localIP().toString();
    doc["connected"] = true;

    JsonArray relays = doc.createNestedArray("relays");
    for(int i = 0; i < 8; i++) {
      JsonObject relay = relays.createNestedObject();
      relay["channel"] = i + 1;
      relay["state"] = relayStates[i];
      relay["function"] = getRelayFunctionName(i);
    }

    String response;
    serializeJson(doc, response);
    server.send(200, "application/json", response);
  });

  // Control specific relay
  server.on("/control", HTTP_POST, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");

    if (server.hasArg("plain")) {
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, server.arg("plain"));

      int channel = doc["channel"];
      bool state = doc["state"];
      int duration = doc["duration"] | 0; // Optional duration in ms

      if (channel >= 1 && channel <= 8) {
        controlRelay(channel - 1, state, duration);

        DynamicJsonDocument response(512);
        response["success"] = true;
        response["channel"] = channel;
        response["state"] = state;
        response["message"] = "Relay " + String(channel) + " " + (state ? "ON" : "OFF");

        String responseStr;
        serializeJson(response, responseStr);
        server.send(200, "application/json", responseStr);
      } else {
        server.send(400, "application/json", "{\"error\":\"Invalid channel\"}");
      }
    } else {
      server.send(400, "application/json", "{\"error\":\"No data received\"}");
    }
  });

  // Engine control (using your existing pin setup)
  server.on("/engine/start", HTTP_POST, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    Serial.println("Web API: Starting engine...");

    // Use same sequence as SinricPro
    digitalWrite(REM, HIGH);
    digitalWrite(ENGINE, HIGH);
    delay(2000);
    digitalWrite(REM, LOW);
    digitalWrite(ENGINE, LOW);
    relayStates[ENGINE_START] = true;

    server.send(200, "application/json", "{\"success\":true,\"action\":\"engine_start\"}");
  });

  server.on("/engine/stop", HTTP_POST, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    Serial.println("Web API: Stopping engine...");

    relayStates[ENGINE_START] = false;
    server.send(200, "application/json", "{\"success\":true,\"action\":\"engine_stop\"}");
  });

  // Trunk control (using your existing pin setup)
  server.on("/trunk/open", HTTP_POST, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    Serial.println("Web API: Opening trunk...");

    // Use same sequence as SinricPro
    digitalWrite(BAGASI1, HIGH);
    delay(3000);
    digitalWrite(BAGASI1, LOW);
    relayStates[TRUNK_OPEN] = true;

    server.send(200, "application/json", "{\"success\":true,\"action\":\"trunk_open\"}");
  });

  server.on("/trunk/close", HTTP_POST, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    Serial.println("Web API: Closing trunk...");

    // Use same sequence as SinricPro
    digitalWrite(BAGASI2, HIGH);
    delay(800);
    digitalWrite(BAGASI2, LOW);
    relayStates[TRUNK_CLOSE] = true;

    server.send(200, "application/json", "{\"success\":true,\"action\":\"trunk_close\"}");
  });
}

void controlRelay(int channel, bool state, int duration = 0) {
  if (channel < 0 || channel >= 8) return;

  relayStates[channel] = state;

  Serial.print("Relay ");
  Serial.print(channel + 1);
  Serial.print(" (");
  Serial.print(getRelayFunctionName(channel));
  Serial.print("): ");
  Serial.println(state ? "ON" : "OFF");

  // Control actual pins based on channel
  switch(channel) {
    case ENGINE_START:
      if (state) {
        digitalWrite(REM, HIGH);
        digitalWrite(ENGINE, HIGH);
        delay(2000);
        digitalWrite(REM, LOW);
        digitalWrite(ENGINE, LOW);
      }
      break;

    case TRUNK_OPEN:
      if (state) {
        digitalWrite(BAGASI1, HIGH);
        delay(3000);
        digitalWrite(BAGASI1, LOW);
      }
      break;

    case TRUNK_CLOSE:
      if (state) {
        digitalWrite(BAGASI2, HIGH);
        delay(800);
        digitalWrite(BAGASI2, LOW);
      }
      break;

    case WINDOW_CONTROL:
      digitalWrite(KACA1, state ? HIGH : LOW);
      digitalWrite(KACA2, state ? LOW : HIGH);
      delay(3000);
      digitalWrite(KACA1, LOW);
      digitalWrite(KACA2, LOW);
      break;

    case LIGHTS:
      digitalWrite(LIGHTS_PIN, state ? HIGH : LOW);
      break;

    case HORN:
      if (state && duration > 0) {
        digitalWrite(HORN_PIN, HIGH);
        delay(duration);
        digitalWrite(HORN_PIN, LOW);
      } else {
        digitalWrite(HORN_PIN, state ? HIGH : LOW);
      }
      break;

    default:
      // For other channels, just log
      Serial.printf("Channel %d not implemented\r\n", channel);
      break;
  }
}

String getRelayFunctionName(int channel) {
  switch(channel) {
    case ENGINE_START: return "Engine Start/Stop";
    case TRUNK_OPEN: return "Trunk Open";
    case TRUNK_CLOSE: return "Trunk Close";
    case WINDOW_CONTROL: return "Window Control";
    case LIGHTS: return "Lights";
    case HORN: return "Horn";
    case DOOR_LOCK: return "Door Lock";
    case SPARE: return "Spare";
    default: return "Unknown";
  }
}
