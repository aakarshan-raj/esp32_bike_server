#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SERVICE_UUID        "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define RX_CHARACTERISTIC   "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define TX_CHARACTERISTIC   "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"


void setup() {

    BLEDevice::init("ESP32-NAVIGATOR");

    BLEServer *server = BLEDevice::createServer();

    BLEService *service = server->createService(SERVICE_UUID);

    BLECharacteristic *rxCharacteristic = service->createCharacteristic( RX_CHARACTERISTIC, BLECharacteristic::PROPERTY_WRITE);
    BLECharacteristic *txCharacteristic = service->createCharacteristic(TX_CHARACTERISTIC, BLECharacteristic::PROPERTY_NOTIFY);

    txCharacteristic->addDescriptor(new BLE2902());

    service->start();
    server->getAdvertising()->start();

}

void loop() {
    delay(1000);
}