#include <ArduinoBLE.h>

#include "TxRx.h"

class BLETxRx : TxRx {

private:
  String receivedJsonText;
  bool hasReceivedJsonText;

private:
  int baudRate;
  BLEService mService;
  BLECharacteristic sendCharacteristic;
  BLECharacteristic receiveCharacteristic;

public:
  // TODO: test this or init
  BLETxRx(int _baudRate, String _deviceName, String _serviceUUID, String _sendUUID, String _receiveUUID)
    : mService(_serviceUUID.c_str()), sendCharacteristic(_sendUUID.c_str(), BLERead, 128), receiveCharacteristic(_receiveUUID.c_str(), BLEWrite, 128) {
    init(_baudRate, _deviceName, _serviceUUID, _sendUUID, _receiveUUID);
  }

  void init(int _baudRate, String _deviceName, String _serviceUUID, String _sendUUID, String _receiveUUID) {
    baudRate = _baudRate;
    Serial.begin(baudRate);
    while (!Serial) {}

    BLE.begin();
    BLE.setLocalName(_deviceName.c_str());
    BLE.setAdvertisedService(mService);
    mService.addCharacteristic(sendCharacteristic);
    mService.addCharacteristic(receiveCharacteristic);
    BLE.addService(mService);
    BLE.advertise();
  }

  void send() {
    BLE.central();
    String& json = prepareJson();
    sendCharacteristic.writeValue(json.c_str(), json.length());
  }

  // https://github.com/arduino-libraries/ArduinoBLE/blob/master/examples/Peripheral/LED/LED.ino
  void receive(std::function< void(String) >&& fn) {
    // TODO: test this
    BLEDevice central = BLE.central();

    // TODO: test if need a while(connected)
    if (central && central.connected()) {
      if (receiveCharacteristic.written()) {
        // TODO: value is a pointer. need to use receiveCharacteristic.valueLength()
        String receivedJsonText = String(receiveCharacteristic.value(), receiveCharacteristic.valueLength());
        fn(receivedJsonText);
      }
    }
  }
};
