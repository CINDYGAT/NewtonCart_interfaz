const int trigPin = 5;  // GPIO5 para Trig
const int echoPin = 18; // GPIO18 para Echo

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Enviar pulso
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Medir el tiempo de respuesta
  long duration = pulseIn(echoPin, HIGH);

  // Calcular la distancia en cm
  float distance = duration * 0.034 / 2;

  // Mostrar la distancia en el monitor serial
  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(500);
}