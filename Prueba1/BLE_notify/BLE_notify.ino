#include <NimBLEDevice.h>

void setup() {
  Serial.begin(115200);

  // Inicializa BLE
  NimBLEDevice::init("ESP32_Sensor");

  // Obtén la dirección MAC en modo Bluetooth
  uint8_t mac[6];
  esp_read_mac(mac, ESP_MAC_BT);  // Lee la dirección MAC en modo Bluetooth
  Serial.print("Dirección MAC BLE: ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", mac[i]);
    if (i < 5) Serial.print(":");
  }
  Serial.println();
}

void loop() {
  // No es necesario hacer nada en el loop
}
