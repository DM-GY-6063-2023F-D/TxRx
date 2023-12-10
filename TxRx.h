#pragma once

#include <ArduinoJson.h>

// TODO: static ??
const int MAX_ANALOG = 16;
const int MAX_DIGITAL = 16;

class TxRx {

private:
  int analogCount = 0;
  int digitalCount = 0;
  int analogPins[MAX_ANALOG];
  int digitalPins[MAX_DIGITAL];
  String jsonText;

public:
  TxRx() {}
  virtual void send() = 0;
  virtual void receive(std::function< void(String) >&& fn) = 0;

  void registerAnalog(int pin) {
    // TODO: dynamic array ?
    if (analogCount >= MAX_ANALOG) return;

    analogPins[analogCount] = pin;
    analogCount++;
  }

  void registerDigital(int pin) {
    // TODO: dynamic array ?
    if (digitalCount >= MAX_DIGITAL) return;

    digitalPins[digitalCount] = pin;
    digitalCount++;
  }

protected:
  String& prepareJson() {
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

    serializeJson(mJson, jsonText);

    return jsonText;
  }
};
