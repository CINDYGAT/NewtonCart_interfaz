#include <SD.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <SPI.h>

// ===== Configuración WiFi =====
const char* ssid = "CRT-649693_2.4G";  // Reemplaza con el nombre de tu red WiFi
const char* password = "902465524";  

// ===== Configuración NextCloud =====
const char* serverUrl = "http://192.168.0.39:8080/remote.php/dav/files/sensor_user/datos.csv";
const char* username = "sensor_user";
const char* password_nc = "Sensordata123*";

// ===== Pines SPI de la SD en ESP32 =====
#define SD_CS 5
#define SD_MOSI 23
#define SD_MISO 19
#define SD_SCK 18

File csvData;

void setup() {
    Serial.begin(115200);

    // ===== Conexión WiFi =====
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" Conectado al WiFi!");
    Serial.println("IP address: "+ WiFi.localIP().toString());

    // ===== Inicializar SD =====
    SPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
    if (!SD.begin(SD_CS, SPI,4000000)) {
        Serial.println("Fallo al inicializar la tarjeta SD.");
        return;
    }
    Serial.println("Tarjeta SD inicializada correctamente.");
}

void loop() {

    // Simulación de datos del sensor
    float tiempo = millis() / 1000.0;
    float aceleracion = random(1, 10) / 10.0;
    float distancia = random(1, 100);
    float fuerza = random(5, 20);
    float velocidad = random(2, 15);

    // ===== Guardar datos en SD =====
    File csvFile = SD.open("/datos.csv", FILE_APPEND);
    if (csvFile) {
        csvFile.printf("%.2f,%.2f,%.2f,%.2f,%.2f\n", tiempo, aceleracion, distancia, fuerza, velocidad);
        csvFile.close();
        Serial.println("Datos guardados en SD.");
    } else {
        Serial.println("Error al abrir el archivo CSV en la SD.");
    }

    // ===== Enviar datos a NextCloud =====
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(serverUrl);
        http.setAuthorization(username, password_nc);
        http.addHeader("Content-Type", "text/csv");

        String csvData = "Tiempo,Aceleracion,Distancia,Fuerza,Velocidad\n";
        csvData += String(tiempo) + "," + String(aceleracion) + "," + String(distancia) + "," + 
                   String(fuerza) + "," + String(velocidad) + "\n";

        int httpResponseCode = http.PUT(csvData);

        if (httpResponseCode > 0) {
            Serial.print("Datos enviados con código: ");
            Serial.println(httpResponseCode);
        } else {
            Serial.print("Error en la conexión: ");
            Serial.println(http.errorToString(httpResponseCode).c_str());
        }

        http.end();
    }

    delay(5000);  // Envía los datos cada 5 segundos (ajústalo según necesites)
}
