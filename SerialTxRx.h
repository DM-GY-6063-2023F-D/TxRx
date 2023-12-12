#include "TxRx.h"

class SerialTxRx : public TxRx {

public:
  SerialTxRx() {}

  void init(int baudRate) {
    startSerial(baudRate);
  }

  void txRx() {
    if (Serial.available() > 0) {
      int byteIn = Serial.read();
      if (byteIn == 'G') {
        Serial.flush();
        String jsonTextTx = getJsonTextTx();
        Serial.println(jsonTextTx);
      } else if (byteIn == 'P' && onReceiveFn) {
        String jsonTextRx = Serial.readStringUntil('\n');
        onReceiveFn(jsonTextRx);
      }
    }
  }
};
