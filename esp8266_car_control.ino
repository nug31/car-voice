#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

// WiFi credentials
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// Web server on port 80
ESP8266WebServer server(80);

// Relay pins (adjust based on your wiring)
const int RELAY_PINS[8] = {D1, D2, D3, D4, D5, D6, D7, D8};

// Relay states
bool relayStates[8] = {false, false, false, false, false, false, false, false};

// Relay functions mapping
enum RelayFunction {
  ENGINE_START = 0,
  TRUNK_OPEN = 1,
  TRUNK_CLOSE = 2,
  LIGHTS = 3,
  HORN = 4,
  DOOR_LOCK = 5,
  DOOR_UNLOCK = 6,
  SPARE = 7
};

void setup() {
  Serial.begin(115200);
  
  // Initialize relay pins
  for(int i = 0; i < 8; i++) {
    pinMode(RELAY_PINS[i], OUTPUT);
    digitalWrite(RELAY_PINS[i], HIGH); // Relay OFF (assuming active LOW)
  }
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  
  // Setup web server routes
  setupRoutes();
  
  // Start server
  server.begin();
  Serial.println("Car Control Server started!");
  Serial.println("Available endpoints:");
  Serial.println("GET  /status - Get all relay states");
  Serial.println("POST /control - Control relays");
  Serial.println("POST /engine/start - Start engine");
  Serial.println("POST /engine/stop - Stop engine");
  Serial.println("POST /trunk/open - Open trunk");
  Serial.println("POST /trunk/close - Close trunk");
}

void loop() {
  server.handleClient();
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
  
  // Engine control
  server.on("/engine/start", HTTP_POST, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    controlRelay(ENGINE_START, true, 1000); // 1 second pulse
    server.send(200, "application/json", "{\"success\":true,\"action\":\"engine_start\"}");
  });
  
  server.on("/engine/stop", HTTP_POST, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    controlRelay(ENGINE_START, false);
    server.send(200, "application/json", "{\"success\":true,\"action\":\"engine_stop\"}");
  });
  
  // Trunk control
  server.on("/trunk/open", HTTP_POST, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    controlRelay(TRUNK_OPEN, true, 500); // 500ms pulse
    server.send(200, "application/json", "{\"success\":true,\"action\":\"trunk_open\"}");
  });
  
  server.on("/trunk/close", HTTP_POST, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    controlRelay(TRUNK_CLOSE, true, 500); // 500ms pulse
    server.send(200, "application/json", "{\"success\":true,\"action\":\"trunk_close\"}");
  });
}

void controlRelay(int channel, bool state, int duration = 0) {
  if (channel < 0 || channel >= 8) return;
  
  relayStates[channel] = state;
  digitalWrite(RELAY_PINS[channel], state ? LOW : HIGH); // Assuming active LOW relays
  
  Serial.print("Relay ");
  Serial.print(channel + 1);
  Serial.print(" (");
  Serial.print(getRelayFunctionName(channel));
  Serial.print("): ");
  Serial.println(state ? "ON" : "OFF");
  
  // If duration specified, turn off after delay
  if (duration > 0 && state) {
    delay(duration);
    relayStates[channel] = false;
    digitalWrite(RELAY_PINS[channel], HIGH);
    Serial.print("Relay ");
    Serial.print(channel + 1);
    Serial.println(" auto OFF");
  }
}

String getRelayFunctionName(int channel) {
  switch(channel) {
    case ENGINE_START: return "Engine Start/Stop";
    case TRUNK_OPEN: return "Trunk Open";
    case TRUNK_CLOSE: return "Trunk Close";
    case LIGHTS: return "Lights";
    case HORN: return "Horn";
    case DOOR_LOCK: return "Door Lock";
    case DOOR_UNLOCK: return "Door Unlock";
    case SPARE: return "Spare";
    default: return "Unknown";
  }
}
