#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// WiFi Configuration 
const char* ssid = "";
const char* password = "";

// OLED Configuration
#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   64
#define OLED_RESET      -1
#define OLED_ADDR       0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Valve configuration 
#define VALVE_PIN1    25  // LEFT
#define VALVE_PIN2    21  // RIGHT

// Web server configuration 
WebServer server(80);

void showMessage(const String& line1, const String& line2 = "") {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.println(line1);
  if (line2 != "") display.println(line2);
  display.display();
}

// Valve Control
void handleValve() {
  if (server.hasArg("left")) {
    String state = server.arg("left");
    if (state == "on") {
      digitalWrite(VALVE_PIN1, HIGH);
      Serial.println("[VALVE1] ON");
      showMessage("VALVE1 ON", WiFi.localIP().toString());
      server.send(200, "text/plain", "VALVE1 turned ON");
    } else if (state == "off") {
      digitalWrite(VALVE_PIN1, LOW);
      Serial.println("[VALVE1] OFF");
      showMessage("VALVE1 OFF", WiFi.localIP().toString());
      server.send(200, "text/plain", "VALVE1 turned OFF");
    } else {
      server.send(400, "text/plain", "Invalid state. Use on/off.");
    }
  } else if (server.hasArg("right")) {
    String state = server.arg("right");
    if (state == "on") {
      digitalWrite(VALVE_PIN2, HIGH);
      Serial.println("[VALVE2] ON");
      showMessage("VALVE2 ON", WiFi.localIP().toString());
      server.send(200, "text/plain", "VALVE2 turned ON");
    } else if (state == "off") {
      digitalWrite(VALVE_PIN2, LOW);
      Serial.println("[VALVE2] OFF");
      showMessage("VALVE2 OFF", WiFi.localIP().toString());
      server.send(200, "text/plain", "VALVE2 turned OFF");
    } else {
      server.send(400, "text/plain", "Invalid state. Use on/off.");
    }
  } else {
    server.send(400, "text/plain", "Missing 'left' or 'right' parameter.");
  }
}

// WiFi Connection 
void connectWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  int retry = 0;
  while (WiFi.status() != WL_CONNECTED && retry < 20) {
    delay(500);
    Serial.print(".");
    retry++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    showMessage("WiFi connected!", WiFi.localIP().toString());
  } else {
    Serial.println("\nWiFi failed.");
    showMessage("WiFi failed");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(VALVE_PIN1, OUTPUT);
  pinMode(VALVE_PIN2, OUTPUT);
  digitalWrite(VALVE_PIN1, LOW);
  digitalWrite(VALVE_PIN2, LOW);

  // OLED Initialization 
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("SSD1306 OLED init failed");
    while (true);
  }

  showMessage("Connecting to", ssid);
  connectWiFi();

  // HTTP Handler registration 
  server.on("/valve", handleValve);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
