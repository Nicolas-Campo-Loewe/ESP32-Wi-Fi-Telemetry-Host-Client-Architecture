#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>

const char* WIFI_SSID     = "XXXX";
const char* WIFI_PASSWORD = "XXXX";
const char* HOST_IP       = "XXX.XXX.XXX.XXX";

WebServer server(80);

const char* WEB_PAGE PROGMEM = R"(
<html>
<head>
  <title>ESP32 Temperature Monitor</title>
  <script>
    function requestReading() {
      fetch('/request')
        .then(response => response.text())
        .then(data => {
          const container = document.getElementById('readings');
          const entry = document.createElement('div');
          entry.textContent = data;
          container.appendChild(entry);
        })
        .catch(error => console.error('Failed to fetch temperature:', error));
    }
  </script>
</head>
<body>
  <h1>ESP32 Temperature Monitor</h1>
  <button onclick="requestReading()">Request Temperature</button>
  <div id="readings"></div>
</body>
</html>
)";

String fetchTemperatureFromHost() {
  HTTPClient http;

  String url = String("http://") + HOST_IP + "/temperature";
  http.begin(url);

  int status = http.GET();
  String response;

  if (status > 0) {
    response = http.getString();
    Serial.println("Reading received from host");
  } else {
    response = "Error: could not connect to host";
    Serial.println("Error: connection to host failed");
  }

  http.end();
  return response;
}

void handleRequest() {
  String reading = fetchTemperatureFromHost();
  server.send(200, "text/plain", reading);
}

void handleRoot() {
  server.send_P(200, "text/html", WEB_PAGE);
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("WiFi connected");
  Serial.print("Client IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/request", handleRequest);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
