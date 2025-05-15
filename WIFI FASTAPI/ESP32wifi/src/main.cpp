#include <WiFi.h>
#include <HTTPClient.h>

// ===== Configuración WiFi =====
const char* ssid = "CRT-649693_2.4G";  // Nombre de la red WiFi
const char* password = "902465524"; 

// ====== IP del servidor que tiene la API FastAPI =====
const char* serverIP = "192.168.0.24"; // IP de la PC con el servidor FastAPI
const int port = 8000; // Puerto del servidor FastAPI

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado al WiFi.");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "http://" + String(serverIP) + ":" + String(port) + "/recolectar/";

    float tiempo = millis() / 1000.0;
    float aceleracion = random(1, 10) / 10.0;
    float distancia = random(1, 100);
    float fuerza = random(5, 20);
    float velocidad = random(2, 15);

    http.begin(url);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String postData = "tiempo=" + String(tiempo) +
                      "&aceleracion=" + String(aceleracion) +
                      "&distancia=" + String(distancia) +
                      "&fuerza=" + String(fuerza) +
                      "&velocidad=" + String(velocidad);

    int httpResponseCode = http.POST(postData);

    Serial.print("Código HTTP: ");
    Serial.println(httpResponseCode);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Respuesta: " + response);
    } else {
      Serial.println("Error en la conexión");
    }

    http.end();
  }

  delay(5000); // cada 5 segundos
}
