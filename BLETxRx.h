#include <ArduinoBLE.h>

#include "TxRx.h"

class BLETxRx : TxRx {

private:
  static String randomUUID;
  static String serviceUUID;
  static String txUUID;
  static String rxUUID;

private:
  int baudRate;
  BLEService mService;
  BLECharacteristic txCharacteristic;
  BLECharacteristic rxCharacteristic;

public:
  // TODO: test this or init
  BLETxRx(int _baudRate, String _deviceName)
    : mService(serviceUUID.c_str()), txCharacteristic(txUUID.c_str(), BLERead, 128), rxCharacteristic(rxUUID.c_str(), BLEWrite, 128) {
    init(_baudRate, _deviceName);
  }

  void init(int _baudRate, String _deviceName) {
    baudRate = _baudRate;
    Serial.begin(baudRate);
    while (!Serial) {}

    BLE.begin();
    BLE.setDeviceName(_deviceName.c_str());
    BLE.setLocalName(_deviceName.c_str());
    BLE.setAdvertisedService(mService);
    mService.addCharacteristic(txCharacteristic);
    mService.addCharacteristic(rxCharacteristic);
    BLE.addService(mService);
    BLE.advertise();
  }

  void send() {
    BLE.central();
    String jsonTextTx = getJsonTextTx();
    txCharacteristic.writeValue(jsonTextTx.c_str(), jsonTextTx.length());
  }

  // https://github.com/arduino-libraries/ArduinoBLE/blob/master/examples/Peripheral/LED/LED.ino
  void receive(std::function< void(String) >&& fn) {
    // TODO: test this
    BLEDevice central = BLE.central();

    // TODO: test if need a while(connected)
    if (central && central.connected()) {
      if (rxCharacteristic.written()) {
        String jsonTextRx = String(rxCharacteristic.value(), rxCharacteristic.valueLength());
        fn(jsonTextRx);
      }
    }
  }
};

// TODO: test random UUID
String BLETxRx::randomUUID = String(random(1000, 9999));
String BLETxRx::serviceUUID = String("250b0d00-1b4f-4f16-9171-f63c733d" + randomUUID);
String BLETxRx::txUUID = String("250b0d01-1b4f-4f16-9171-f63c733d" + randomUUID);
String BLETxRx::rxUUID = String("250b0d02-1b4f-4f16-9171-f63c733d" + randomUUID);

// TODO: test p5js connect by service name
// https://developer.mozilla.org/en-US/docs/Web/API/Bluetooth/requestDevice
// let options = {
//   filters: [
//     { services: [0x1802] },
//     { name: "TxRx" },
//     { namePrefix: "TxRx" },
//   ],
// };
// mBLE.connect(options, gotCharacteristics);
