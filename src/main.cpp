#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SERVICE_UUID        "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define RX_CHARACTERISTIC   "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define TX_CHARACTERISTIC   "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"


BLECharacteristic *rxCharacteristic;
BLECharacteristic *txCharacteristic;


class ServerCallbacks : public BLEServerCallbacks {

    void onConnect(BLEServer* server) override {

        Serial.println("Device connected!");
    }

    void onDisconnect(BLEServer* server) override {
        Serial.println("Device disconnected");
    }
};

class RxCallback : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *characteristic) {
        std::string value = characteristic->getValue();

        Serial.print("Received: ");
        Serial.println(value.c_str());
    }
};

void setup() {
    Serial.begin(115200);
    BLEDevice::init("ESP32-NAVIGATOR");

    BLEServer *server = BLEDevice::createServer();

    BLEService *service = server->createService(SERVICE_UUID);

    rxCharacteristic = service->createCharacteristic( RX_CHARACTERISTIC, BLECharacteristic::PROPERTY_WRITE);
    txCharacteristic = service->createCharacteristic(TX_CHARACTERISTIC, BLECharacteristic::PROPERTY_NOTIFY);

    txCharacteristic->addDescriptor(new BLE2902());
    rxCharacteristic->setCallbacks( new RxCallback());
    service->start();
    server->setCallbacks(new ServerCallbacks());
    server->getAdvertising()->start();

    BLEAdvertising *advertising = server->getAdvertising();
    advertising->addServiceUUID(SERVICE_UUID);
    advertising->start();

}

void loop() {
    delay(1000);
    txCharacteristic->setValue("Hello From Esp32 navigator");
    txCharacteristic->notify();
}