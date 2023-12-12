#include "TxRx.h"

class SerialTxRx : TxRx {

private:
  int baudRate;

public:
  // TODO: test this or init
  SerialTxRx(int _baudRate) {
    init(_baudRate);
  }

  void init(int _baudRate) {
    baudRate = _baudRate;
    Serial.begin(baudRate);
    while (!Serial) {}
  }

  void send() {
    if (Serial.available() > 0) {
      int byteIn = Serial.peek();
      if (byteIn == 'G') {
        Serial.read();
        Serial.flush();
        String jsonTextTx = getJsonTextTx();
        Serial.println(jsonTextTx);
      }
    }
  }

  void receive(std::function< void(String) >&& fn) {
    if (Serial.available() > 0) {
      int byteIn = Serial.peek();
      if (byteIn == 'P') {
        Serial.read();
        String jsonTextRx = Serial.readStringUntil('\n');
        fn(jsonTextRx);
      }
    }
  }
};
