#include "SerialTxRx.h"
#include "WiFiTxRx.h"
#include "BLETxRx.h"

SerialTxRx mSerialTxRx(9600);
WiFiTxRx mWiFiTxRx(9600, String("id"), String("password"));
BLETxRx mBLETxRx(9600, String("TxRx"));

void setup() {
  mSerialTxRx.init(9600);
  mWiFiTxRx.init(9600, String("id"), String("password"));
  mBLETxRx.init(9600, String("TxRx"));
}

void loop() {
  // put your main code here, to run repeatedly:
}
