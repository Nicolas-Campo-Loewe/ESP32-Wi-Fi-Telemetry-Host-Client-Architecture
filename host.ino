#include <WiFi.h>
#include <WebServer.h>
#include <time.h>

const char* WIFI_SSID     = "XXXX";
const char* WIFI_PASSWORD = "XXXX";

WebServer server(80);
bool measuring = true;

float readInternalTemperature() {
  return temperatureRead();
}

String getUTCTime() {
  time_t now;
  struct tm timeinfo;
  time(&now);
  gmtime_r(&now, &timeinfo);

  char buffer[32];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return String(buffer);
}

void handleTemperature() {
  if (!measuring) {
    server.send(503, "text/plain", "Measurement is stopped");
    return;
  }

  float temperature = readInternalTemperature();
  String utcTime    = getUTCTime();
  String response   = "Internal temperature: " + String(temperature, 2) + " C, UTC time: " + utcTime;

  server.send(200, "text/plain", response);
  Serial.println("Reading sent to client");
}

void handleToggle() {
  String action = server.arg("action");

  if (action == "start") {
    measuring = true;
    server.send(200, "text/plain", "Measurement started");
    Serial.println("Measurement started");
  } else if (action == "stop") {
    measuring = false;
    server.send(200, "text/plain", "Measurement stopped");
    Serial.println("Measurement stopped");
  } else {
    server.send(400, "text/plain", "Invalid action");
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("WiFi connected");
  Serial.print("Host IP: ");
  Serial.println(WiFi.localIP());

  configTime(0, 0, "pool.ntp.org", "time.nist.gov");

  server.on("/temperature", handleTemperature);
  server.on("/toggle", handleToggle);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
