#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

const char* ssid          = "asa";
const char* password      = "12345789";
const char* mqttServer    = "broker.hivemq.com";
const int   mqttPort      = 1883;

const char* topicData           = "unsoed/tk245004/AB3/data";
const char* topicPerintahLED    = "unsoed/tk245004/AB3/perintah";
// MODIFIKASI: Menambahkan topik perintah baru khusus untuk buzzer
const char* topicPerintahBuzzer = "unsoed/tk245004/AB3/perintah/buzzer";

#define DHTPIN 4
#define DHTTYPE DHT11

const int ledPin    = 14; // GPIO 14 (Pin D5)
// MODIFIKASI: Menambahkan buzzer (GPIO 12 / Pin D6)
const int buzzerPin = 12;

DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);

// Manajemen Waktu Non-Blocking
unsigned long waktuTerakhirPublish = 0;
const long intervalPublish = 5000; // Interval 5 detik

void callback(char* topic, byte* payload, unsigned int length) {
  String pesan = "";
  for (unsigned int i = 0; i < length; i++) {
    pesan += (char)payload[i];
  }

  Serial.print("Pesan masuk [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(pesan);

  JsonDocument doc;
  if (deserializeJson(doc, pesan)) return; // Abaikan jika parsing JSON gagal

  const char* perintah = doc["perintah"];
  if (perintah == NULL) return;

  // MODIFIKASI: Membedakan aksi berdasarkan topik penerima pesan
  // Memeriksa apakah pesan berasal dari topik perintah LED
  if (String(topic) == topicPerintahLED) {
    if (String(perintah) == "ON") {
      digitalWrite(ledPin, HIGH);
      Serial.println("Aktuator LED: ON");
    } else if (String(perintah) == "OFF") {
      digitalWrite(ledPin, LOW);
      Serial.println("Aktuator LED: OFF");
    }
  } 
  // Memeriksa apakah pesan berasal dari topik perintah Buzzer
  else if (String(topic) == topicPerintahBuzzer) {
    if (String(perintah) == "ON") {
      digitalWrite(buzzerPin, HIGH); // Menyalahkan buzzer
      Serial.println("Aktuator BUZZER: ON");
    } else if (String(perintah) == "OFF") {
      digitalWrite(buzzerPin, LOW);  // Mematikan buzzer
      Serial.println("Aktuator BUZZER: OFF");
    }
  }
}

void hubungkanWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println("WiFi berhasil terhubung!");
}

void hubungkanMQTT() {
  while (!client.connected()) {
    String clientId = "ESP8266Client-" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      // MODIFIKASI: Subscribe ke kedua topik perintah secara bersamaan
      client.subscribe(topicPerintahLED);
      client.subscribe(topicPerintahBuzzer);
      
      Serial.println("Terhubung dan subscribe ke topik LED & Buzzer!");
    } else {
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  // Mengatur mode pin sebagai output
  pinMode(ledPin, OUTPUT);
  
  // MODIFIKASI: Inisialisasi pin buzzer sebagai OUTPUT dan atur kondisi awal mati (LOW)
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(ledPin, LOW);

  dht.begin();
  
  hubungkanWiFi();
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    hubungkanMQTT();
  }
  client.loop(); // Memproses pesan masuk secara real-time

  // Non-blocking publish menggunakan millis()
  if (millis() - waktuTerakhirPublish >= intervalPublish) {
    waktuTerakhirPublish = millis();
    
    float suhu = dht.readTemperature();
    if (!isnan(suhu)) {
      JsonDocument doc;
      doc["suhu"] = suhu;
      
      char buffer[128];
      serializeJson(doc, buffer);
      
      client.publish(topicData, buffer);
      Serial.print("Data terkirim: ");
      Serial.println(buffer);
    }
  }
}