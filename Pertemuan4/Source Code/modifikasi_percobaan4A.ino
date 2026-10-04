#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid          = "asa";
const char* password      = "12345789";
const char* mqttServer    = "broker.hivemq.com";
const int   mqttPort      = 1883;
const char* topicPerintah = "unsoed/tk245004/AB3/perintah";
// Pin LED (Pin D2 / GPIO 4 pada ESP8266)
const int ledPin = 14;

WiFiClient espClient;
PubSubClient client(espClient);

// Callback otomatis dipanggil saat ada pesan masuk pada topik
void callback(char* topic, byte* payload, unsigned int length) {
  String pesan = "";
  for (unsigned int i = 0; i < length; i++) {
    pesan += (char)payload[i];
  }

  Serial.print("Pesan diterima [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(pesan);

  // Deserialisasi data JSON
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, pesan);

  if (error) {
    Serial.print("Gagal parsing JSON: ");
    Serial.println(error.c_str());
    return;
  }

  // Reading status perintah dari JSON
  const char* perintah = doc["perintah"];

  
  if (String(perintah) == "ON") {
    // Membaca kunci "intensitas" dari data JSON. Jika kunci tidak ada di JSON,
    // fungsi doc["intensitas"] | 1023 memberikan nilai default maksimum (1023).
    int intensitas = doc["intensitas"] | 1023;

    // Membatasi nilai intensitas agar tetap berada di rentang PWM valid (0 - 1023)
    intensitas = constrain(intensitas, 0, 1023);

    // Mengatur kecerahan LED menggunakan PWM sesuai nilai intensitas
    analogWrite(ledPin, intensitas);

    Serial.print("Aktuator: ON | Intensitas PWM: ");
    Serial.println(intensitas);

  } else if (String(perintah) == "OFF") {
    // Mematikan LED dengan memberikan sinyal PWM 0 (duty cycle 0%)
    analogWrite(ledPin, 0);

    Serial.println("Aktuator: OFF | Intensitas PWM: 0");
  }
}

void hubungkanWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan ke WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi berhasil terhubung!");
}

void hubungkanMQTT() {
  while (!client.connected()) {
    Serial.print("Menghubungkan ke broker MQTT...");
    String clientId = "ESP8266Client-" + String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      Serial.println("berhasil terhubung!");
      client.subscribe(topicPerintah);
      Serial.print("Subscribe ke topic: ");
      Serial.println(topicPerintah);
    } else {
      Serial.print("gagal, rc=");
      Serial.print(client.state());
      Serial.println(" coba lagi dalam 2 detik");
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  
  // Menginisialisasi LED dalam kondisi mati menggunakan PWM
  analogWrite(ledPin, 0);
  
  hubungkanWiFi();
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    hubungkanMQTT();
  }
  client.loop();
}