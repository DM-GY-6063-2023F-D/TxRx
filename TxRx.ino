#include "SerialTxRx.h"
#include "WiFiTxRx.h"
#include "BLETxRx.h"

SerialTxRx mSerialTxRx(9600);
WiFiTxRx mWiFiTxRx(9600, String("id"), String("password"));
BLETxRx mBLETxRx(9600,
                 String("TxRx"),
                 String("250b0d00-1b4f-4f16-9171-f63c733d15ab"),
                 String("250b0d01-1b4f-4f16-9171-f63c733d15ab"),
                 String("250b0d02-1b4f-4f16-9171-f63c733d15ab"));

void setup() {
  mSerialTxRx.init(9600);
  mWiFiTxRx.init(9600, String("id"), String("password"));
  mBLETxRx.init(9600,
                String("TxRx"),
                String("250b0d00-1b4f-4f16-9171-f63c733d15ab"),
                String("250b0d01-1b4f-4f16-9171-f63c733d15ab"),
                String("250b0d02-1b4f-4f16-9171-f63c733d15ab"));
}

void loop() {
  // put your main code here, to run repeatedly:
}
