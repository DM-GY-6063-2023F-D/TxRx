#pragma once

#include <ArduinoJson.h>

class TxRx {

private:
  static const int MAX_ANALOG{ 16 };
  static const int MAX_DIGITAL{ 16 };

private:
  int analogCount = 0;
  int digitalCount = 0;
  int analogPins[TxRx::MAX_ANALOG];
  int digitalPins[TxRx::MAX_DIGITAL];
  String jsonTextTx;

protected:
  std::function< void(String) > onReceiveFn;

protected:
  TxRx() {}

public:
  virtual void init(int baudRate) = 0;
  virtual void txRx() = 0;

  void startSerial(int baudRate) {
    if (!Serial) {
      Serial.begin(baudRate);
    }
    while (!Serial) {}
  }

  void registerAnalog(int pin) {
    // TODO: dynamic array
    if (analogCount >= TxRx::MAX_ANALOG) return;

    analogPins[analogCount] = pin;
    analogCount++;
  }

  void registerDigital(int pin) {
    // TODO: dynamic array
    if (digitalCount >= TxRx::MAX_DIGITAL) return;

    pinMode(pin, INPUT);
    digitalPins[digitalCount] = pin;
    digitalCount++;
  }

  void registerOnReceive(std::function< void(String) >&& fn) {
    onReceiveFn = fn;
  }


protected:
  const String& getJsonTextTx() {
    // TODO: dynamic size of JSON based on counts
    StaticJsonDocument<128> mJson;
    JsonObject data = mJson.createNestedObject("data");

    for (int di = 0; di < digitalCount; di++) {
      int pin = digitalPins[di];
      data[String("D") + String(pin)] = digitalRead(pin);
    }

    for (int ai = 0; ai < analogCount; ai++) {
      int pin = analogPins[ai];
      data[String("A") + String(pin - A0)] = analogRead(pin);
    }

    jsonTextTx = "";
    serializeJson(mJson, jsonTextTx);

    return jsonTextTx;
  }
};
