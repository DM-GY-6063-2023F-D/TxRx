#include <WebServer.h>
#include <WiFi.h>

#include "TxRx.h"

class WiFiTxRx : TxRx {

private:
  String jsonTextRx;
  bool hasJsonTextRx;

private:
  int baudRate;
  WebServer mServer;

public:
  // TODO: test this or init
  WiFiTxRx(int _baudRate, String _ssid, String _password)
    : mServer(80) {
    init(_baudRate, _ssid, _password);
  }

  void init(int _baudRate, String _ssid, String _password) {
    baudRate = _baudRate;
    Serial.begin(baudRate);
    while (!Serial) {}

    // WiFi setup
    WiFi.disconnect(true);
    WiFi.mode(WIFI_STA);
    WiFi.begin(_ssid, _password);

    int tryConnectCount = 0;
    while (WiFi.status() != WL_CONNECTED && tryConnectCount < 60) {
      delay(500);
      Serial.print(".");
      tryConnectCount++;
    }
    Serial.println("");

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
    } else {
      Serial.println("WiFi is NOT connected! 💩");
    }

    mServer.enableCORS();
    // TODO: tesst this !!!!
    // inspired by: https://stackoverflow.com/a/29286957
    mServer.on("/data", HTTP_GET, [&]() {
      handleData();
    });
    mServer.on("/post", HTTP_POST, [&]() {
      handlePost();
    });
    mServer.onNotFound([&]() {
      handleNotFound();
    });
    mServer.begin();
  }

private:
  void handleData() {
    String jsonTextTx = getJsonTextTx();
    mServer.send(200, "application/json", jsonTextTx);
  }

  // WebServer "documentation":
  // https://github.com/espressif/arduino-esp32/blob/master/libraries/WebServer/src/WebServer.h
  void handlePost() {
    if (hasJsonTextRx) {
      // TODO: Arduino not processing POST fast enough
    }

    jsonTextRx = String(mServer.arg(0));
    hasJsonTextRx = true;
    mServer.send(200, "text/plain", "POSTed");
  }

  // Handle CORS pre-flight on POST requests, according to this:
  // https://stackoverflow.com/a/74116471
  void handleNotFound() {
    if (mServer.method() == HTTP_OPTIONS) {
      mServer.send(200);
    } else {
      mServer.send(404, "text/plain", "404! ADDRESS NOT FOUND");
    }
  }

public:
  void send() {
    mServer.handleClient();
  }

  void receive(std::function< void(String) >&& fn) {
    mServer.handleClient();

    if (hasJsonTextRx) {
      hasJsonTextRx = false;
      fn(jsonTextRx);
    } else {
      fn(String(""));
    }
  }
};
