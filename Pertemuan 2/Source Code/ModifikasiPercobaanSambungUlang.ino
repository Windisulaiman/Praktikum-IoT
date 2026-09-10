#include <ESP8266WiFi.h>

// Konfigurasi WiFi
const char* ssid = "S24";
const char* password = "11111111";

// LED indikator
#define LED_PIN 2  // GPIO 2 = D4 pada NodeMCU ESP8266

void setup() {
  // Memulai komunikasi Serial
  Serial.begin(115200);

  // Mengatur LED sebagai output
  pinMode(LED_PIN, OUTPUT);

  // LED mati pada awal program
  digitalWrite(LED_PIN, LOW);

  // Mengatur ESP8266 sebagai Station
  WiFi.mode(WIFI_STA);

  // Memulai koneksi WiFi
  WiFi.begin(ssid, password);

  Serial.println();
  Serial.print("Menghubungkan ke WiFi");

  // Menunggu sampai terhubung
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // WiFi berhasil terhubung
  Serial.println();
  Serial.println("WiFi berhasil terhubung!");

  // Menyalakan LED indikator
  digitalWrite(LED_PIN, HIGH);

  // Menampilkan informasi jaringan
  Serial.print("SSID        : ");
  Serial.println(WiFi.SSID());

  Serial.print("IP Address  : ");
  Serial.println(WiFi.localIP());

  Serial.print("MAC Address : ");
  Serial.println(WiFi.macAddress());

  Serial.print("RSSI        : ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
}

void loop() {

  // Mengecek apakah koneksi WiFi terputus
  if (WiFi.status() != WL_CONNECTED) {

    // Mematikan LED karena WiFi terputus
    digitalWrite(LED_PIN, LOW);

    Serial.println();
    Serial.println("WiFi terputus!");
    Serial.println("Mencoba reconnect...");

    // Mencoba menghubungkan kembali ke WiFi
    WiFi.disconnect();
    WiFi.begin(ssid, password);

    // Menunggu sampai berhasil terhubung kembali
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    // Koneksi berhasil kembali
    Serial.println();
    Serial.println("WiFi berhasil terhubung kembali!");

    // Menyalakan kembali LED
    digitalWrite(LED_PIN, HIGH);

    // Menampilkan IP setelah reconnect
    Serial.print("IP Address : ");
    Serial.println(WiFi.localIP());

    // Menampilkan RSSI setelah reconnect
    Serial.print("RSSI       : ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
  }

  // Mengecek koneksi setiap 1 detik
  delay(1000);
}
