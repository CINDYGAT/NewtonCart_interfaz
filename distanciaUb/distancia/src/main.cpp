#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// Pines del sensor ultrasónico
#define TRIG_PIN 5
#define ECHO_PIN 18

// Configuración de BLE
#define SERVICE_UUID        "12345678-1234-5678-1234-56789abcdef0"
#define CHARACTERISTIC_UUID "abcdef12-3456-789a-bcde-f0123456789a"

BLECharacteristic *pCharacteristic;

void setup() {
    Serial.begin(115200);

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    
    // Inicializar BLE
    BLEDevice::init("ESP32_Ultrasonico");
    Serial.print("MAC del ESP32: ");
    BLEServer *pServer = BLEDevice::createServer();
    BLEService *pService = pServer->createService(SERVICE_UUID);
    Serial.println(BLEDevice::getAddress().toString().c_str());  // Imprime la MAC

    pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_NOTIFY
                    );

    pService->start();
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->start();
}

// Función para medir distancia con el sensor ultrasónico
float medirDistancia() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH);
    float distance = (duration * 0.0343) / 2;  // Convertir a cm
    return distance;
}

void loop() {
    float distancia = medirDistancia();
    Serial.print("Distancia: ");
    Serial.print(distancia);
    Serial.println(" cm");

    // Convertir la distancia en String y enviarla por BLE
    char distanciaStr[10];
    dtostrf(distancia, 6, 2, distanciaStr);
    pCharacteristic->setValue(distanciaStr);
    pCharacteristic->notify();  // Notificar a dispositivos conectados

    delay(1000);  // Enviar datos cada segundo
}