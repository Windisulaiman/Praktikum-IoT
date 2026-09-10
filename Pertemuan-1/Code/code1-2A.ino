#include <DHT.h>

#define DHTPIN 4          // DATA DHT22 → GPIO 2
#define DHTTYPE DHT11
#define RELAYPIN 5        // Relay → GPIO 5

DHT dht(DHTPIN, DHTTYPE);

const float suhuThreshold = 30.0;

void setup() {
  Serial.begin(115200);
  pinMode(DHTPIN, INPUT_PULLUP); // pull-up internal, tanpa resistor
  dht.begin();
  pinMode(RELAYPIN, OUTPUT);
  digitalWrite(RELAYPIN, LOW);
}

void loop() {
  delay(2000); // DHT22 butuh 2 detik antar pembacaan

  float suhu = dht.readTemperature();

  if (isnan(suhu)) {
    Serial.println("Gagal membaca data sensor!");
    delay(1000);
  } else {
    Serial.print("Suhu: ");
    Serial.print(suhu);
    Serial.print(" °C -> ");

    if (suhu > suhuThreshold) {
      digitalWrite(RELAYPIN, HIGH);
      Serial.println("Aktuator: ON");
    } else {
      digitalWrite(RELAYPIN, LOW);
      Serial.println("Aktuator: OFF");
    }
  }
}
