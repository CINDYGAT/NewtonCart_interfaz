#include <SD.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <SPI.h>

// ===== Configuración WiFi =====
const char* ssid = "CRT-649693_2.4G";  // Reemplaza con el nombre de tu red WiFi
const char* password = "902465524";  

// ===== Configuración NextCloud =====
const char* serverUrl = "http://192.168.0.39:8080/remote.php/dav/files/sensor_user/";
const char* username = "sensor_user";
const char* password_nc = "Sensordata123*";

// ===== Pines SPI de la SD en ESP32 =====
#define SD_CS 5
#define SD_MOSI 23
#define SD_MISO 19
#define SD_SCK 18

String nombreArchivo = "";
String rutaNextCloud = "";
//File archivo;
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
    Serial.println("Dirección IP: "+ WiFi.localIP().toString());

    // ===== Inicializar SD =====
    SPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
    if (!SD.begin(SD_CS, SPI,4000000)) {
        Serial.println("Fallo al inicializar la tarjeta SD.");
        return;
    }
    Serial.println("Tarjeta SD inicializada correctamente.");


     // ===== Crea un archivo nuevo y Generar nombre de archivo =====
    unsigned long tiempoInicio = millis();
    nombreArchivo = "/datos_" + String(tiempoInicio) + ".csv";
    rutaNextCloud = String(serverUrl) + "datos_" + String(tiempoInicio) + ".csv";

    // Crear archivo en SD con cabeceras
    csvData = SD.open(nombreArchivo.c_str(), FILE_WRITE);
    if (csvData) {
        csvData.println("Tiempo,Aceleracion,Distancia,Fuerza,Velocidad");
        csvData.close();
        Serial.println("Archivo creado: " + nombreArchivo);
    } else {
        Serial.println("Error al crear el archivo en la SD.");
    }
}

void loop() {

    // Simulación de datos del sensor
    float tiempo = millis() / 1000.0;
    float aceleracion = random(1, 10) / 10.0;
    float distancia = random(1, 100);
    float fuerza = random(5, 20);
    float velocidad = random(2, 15);

    // ===== Guardar datos en SD =====
    File csvFile = SD.open(nombreArchivo.c_str(), FILE_APPEND);
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
        http.begin(rutaNextCloud);
        http.setAuthorization(username, password_nc);
        http.addHeader("Content-Type", "text/csv");

    // Abrimos el archivo y lo leemos todo
    csvFile = SD.open(nombreArchivo.c_str());
    if (csvFile) {
      String contenido = "";
      while (csvFile.available()) {
        contenido += (char)csvFile.read();
      }
      csvFile.close();

        int httpResponseCode = http.PUT(contenido);

        if (httpResponseCode > 0) {
            Serial.print("Archivo enviado a NextCloud. Código: ");
            Serial.println(httpResponseCode);
        } else {
            Serial.print("Error al subir datos: ");
            Serial.println(http.errorToString(httpResponseCode).c_str());
        }
        }
        http.end();
    }

    delay(5000);  // Envía los datos cada 5 segundos (ajústalo según necesites)
}