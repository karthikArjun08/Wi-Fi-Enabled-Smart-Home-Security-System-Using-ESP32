/*
  Smart Home Security System using ESP32
  --------------------------------------
  Components:
    - PIR Motion Sensor (HC-SR501)
    - LED indicator
    - Active Buzzer
    - ESP32 Dev Board

  Functions:
    - Detects motion using PIR sensor
    - Activates LED + Buzzer alarm
    - Hosts a live web dashboard (auto-updating every second)
    - Shows motion status and LED state

  Author: Karthik
  License: MIT
*/

#include <WiFi.h>
#include <WebServer.h>

// ---------- USER CONFIG ----------
const char* ssid = "YOUR_WIFI_SSID";       // 🔸 Replace with your Wi-Fi SSID
const char* password = "YOUR_WIFI_PASSWORD"; // 🔸 Replace with your Wi-Fi password
// --------------------------------

#define PIR_PIN 13     // PIR output pin
#define BUZZER_PIN 14  // Buzzer pin
#define LED_PIN 2      // LED pin

WebServer server(80);
bool motionDetected = false;
unsigned long lastTrigger = 0;
const unsigned long displayDuration = 5000; // keep "Motion Detected" visible for 5 seconds

// ---------------------- WEB PAGE ----------------------
void handleRoot() {
  String html = R"rawliteral(
  <!DOCTYPE html>
  <html>
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width,initial-scale=1">
    <title>ESP32 Smart Security</title>
    <style>
      body {font-family: Arial; text-align: center; background: #f2f2f2; margin: 0; padding: 0;}
      .card {margin: 60px auto; background: white; padding: 25px; border-radius: 12px; width: 320px; box-shadow: 2px 2px 8px #aaa;}
      h2 {color: #333;}
      #status {font-size: 20px; font-weight: bold;}
    </style>
  </head>
  <body>
    <div class="card">
      <h2>🏠 Smart Home Security</h2>
      <p id="status">Loading...</p>
      <p>LED: <span id="led">OFF</span></p>
    </div>

    <script>
      async function updateStatus() {
        try {
          const res = await fetch('/status');
          const data = await res.json();
          document.getElementById('status').innerHTML = data.motion ? "⚠️ <b>Motion Detected!</b>" : "✅ No Motion";
          document.getElementById('status').style.color = data.motion ? "red" : "green";
          document.getElementById('led').innerText = data.led ? "ON" : "OFF";
        } catch (err) {
          console.log("Error fetching status:", err);
        }
      }
      setInterval(updateStatus, 1000);
      updateStatus();
    </script>
  </body>
  </html>
  )rawliteral";

  server.send(200, "text/html; charset=UTF-8", html);
}

// JSON endpoint for AJAX requests
void handleStatus() {
  bool ledOn = digitalRead(LED_PIN);
  String json = "{\"motion\":";
  json += motionDetected ? "true" : "false";
  json += ",\"led\":";
  json += ledOn ? "true" : "false";
  json += "}";
  server.send(200, "application/json", json);
}

// ---------------------- SETUP ----------------------
void setup() {
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  Serial.println("\nConnecting to Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ Wi-Fi connected!");
  Serial.print("🌐 IP Address: ");
  Serial.println(WiFi.localIP());

  // Start web server
  server.on("/", handleRoot);
  server.on("/status", handleStatus);
  server.begin();
  Serial.println("📡 Web server started");
}

// ---------------------- LOOP ----------------------
void loop() {
  server.handleClient();
  int pirState = digitalRead(PIR_PIN);
  unsigned long now = millis();

  if (pirState == HIGH) {
    motionDetected = true;
    lastTrigger = now;
    Serial.println("⚠️ Motion Detected!");
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
  }

  // Turn off buzzer after 3 seconds
  if (motionDetected && (now - lastTrigger > 3000)) {
    digitalWrite(BUZZER_PIN, LOW);
  }

  // Reset motion status after 5 seconds
  if (motionDetected && (now - lastTrigger > displayDuration)) {
    motionDetected = false;
    digitalWrite(LED_PIN, LOW);
  }
}

