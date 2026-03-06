# ESP32 WI-FI TELEMETRY — HOST/CLIENT ARCHITECTURE

Peer-to-peer Wi-Fi communication between two ESP32 microcontrollers, with real-time internal temperature and UTC timestamp served through an embedded web interface.

---

## Overview

This project demonstrates a host/client architecture using two ESP32 boards communicating over a local Wi-Fi network. The **host** ESP32 reads its internal temperature sensor and exposes the data via HTTP. The **client** ESP32 queries that endpoint and presents the data through its own embedded web server, accessible from any browser on the same network.

---

## Features

- Wireless HTTP communication between two ESP32 devices
- Internal temperature sensor reading via `readInternalTemperature()`
- UTC timestamp synchronization via NTP (`pool.ntp.org`)
- Lightweight embedded web interface served directly from the client ESP32
- Asynchronous data requests using the browser Fetch API
- Start/stop measurement control via the `/toggle` endpoint

---

## Hardware Requirements

| Component | Quantity |
|-----------|----------|
| ESP32 development board | 2 |
| USB cables (for flashing & power) | 2 |

---

## Project Structure

```
├── host.ino      # ESP32 host: temperature sensor + HTTP server
└── client.ino    # ESP32 client: HTTP requester + web interface
```

### `host.ino`
Sets up an HTTP server on port 80 that responds to `GET /temperature` with the current internal temperature and UTC time. Exposes a `GET /toggle?action=start|stop` endpoint to enable or disable readings. Synchronizes time on boot via NTP.

### `client.ino`
Connects to the host's `/temperature` endpoint on demand and serves an HTML page with a button to trigger temperature requests. Responses are appended dynamically to the page using the Fetch API. The web page is stored in flash memory via `PROGMEM` and served with `server.send_P()`.

---

## Setup & Configuration

### 1. Flash the host ESP32

Open `host.ino` in the Arduino IDE and set your credentials:

```cpp
const char* WIFI_SSID     = "YOUR_SSID";
const char* WIFI_PASSWORD = "YOUR_PASSWORD";
```

Flash the sketch and note the IP address printed to the Serial Monitor — you will need it for the client.

### 2. Flash the client ESP32

Open `client.ino` and set your credentials and the host's IP:

```cpp
const char* WIFI_SSID     = "YOUR_SSID";
const char* WIFI_PASSWORD = "YOUR_PASSWORD";
const char* HOST_IP       = "HOST_ESP32_IP";
```

Flash the sketch.

### 3. Access the web interface

Connect a device to the same Wi-Fi network, open a browser, and navigate to the client ESP32's IP address (printed to Serial Monitor on boot). Press **"Request Temperature"** to fetch a reading from the host.

---

## API Endpoints

### Host

| Method | Endpoint | Description |
|--------|----------|-------------|
| GET | `/temperature` | Returns current temperature and UTC time. Returns 503 if measurement is stopped. |
| GET | `/toggle?action=start` | Enables temperature readings |
| GET | `/toggle?action=stop` | Disables temperature readings |

### Client

| Method | Endpoint | Description |
|--------|----------|-------------|
| GET | `/` | Serves the web interface |
| GET | `/request` | Fetches a reading from the host and returns it |

---

## Dependencies

Both sketches use libraries included with the standard ESP32 Arduino core — no additional installations required.

| Library | Used in |
|---------|---------|
| `WiFi.h` | host, client |
| `WebServer.h` | host, client |
| `HTTPClient.h` | client |
| `time.h` | host |

---

## Demo

A working demonstration of this project is available on YouTube:
