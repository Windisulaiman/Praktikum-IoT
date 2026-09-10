#include <ESP8266WiFi.h>

// Konfigurasi WiFi
const char* ssid = "Galasih";
const char* password = "2345678901";

// Pin LED indikator
const int ledPin = 2;

void setup() {
  // Inisialisasi Serial Monitor
  Serial.begin(115200);

  // Konfigurasi pin LED
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Mengatur mode WiFi menjadi Station
  WiFi.mode(WIFI_STA);

  // Menghubungkan ESP32 ke WiFi
  WiFi.begin(ssid, password);

  Serial.print("Menghubungkan ke WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(5000);
    Serial.print(".");
  }

  // Jika berhasil terhubung
  Serial.println("\nWiFi berhasil terhubung!");

  // Menampilkan informasi jaringan
  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());

  Serial.print("MAC Address : ");
  Serial.println(WiFi.macAddress());

  Serial.print("RSSI (dBm) : ");
  Serial.println(WiFi.RSSI());

  // Menyalakan LED sebagai indikator WiFi terhubung
  digitalWrite(ledPin, HIGH);
}

void loop() {
  // Mengecek status koneksi setiap 5 detik
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Status: Terhubung");
    digitalWrite(ledPin, HIGH);
  } else {
    Serial.println("Status: Terputus");
    digitalWrite(ledPin, LOW);
  }

  delay(5000);
}
