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
  TxRx() {}

public:
  virtual void send() = 0;
  virtual void receive(std::function< void(String) >&& fn) = 0;

  void registerAnalog(int pin) {
    // TODO: dynamic array ?
    if (analogCount >= TxRx::MAX_ANALOG) return;

    analogPins[analogCount] = pin;
    analogCount++;
  }

  void registerDigital(int pin) {
    // TODO: dynamic array ?
    if (digitalCount >= TxRx::MAX_DIGITAL) return;

    digitalPins[digitalCount] = pin;
    digitalCount++;
  }

protected:
  const String& getJsonTextTx() {
    // TODO: dynamic size of JSON
    StaticJsonDocument<128> mJson;
    JsonObject data = mJson.createNestedObject("data");

    for (int di = 0; di < digitalCount; di++) {
      int pin = digitalPins[di];
      data[String(pin)] = digitalRead(pin);
    }

    for (int ai = 0; ai < analogCount; ai++) {
      int pin = analogPins[ai];
      data[String(pin)] = analogRead(pin);
    }

    serializeJson(mJson, jsonTextTx);

    return jsonTextTx;
  }
};
