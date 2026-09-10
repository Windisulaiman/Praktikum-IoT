#include <ESP8266WiFi.h>

// Konfigurasi Access Point
const char* ap_ssid = "ESP32_AccessPoint";
const char* ap_password = "12345678";  // Minimal 8 karakter

void setup() {
  // Inisialisasi Serial Monitor
  Serial.begin(115200);

  // Mengatur mode WiFi menjadi Access Point
  WiFi.mode(WIFI_AP);

  // Membuat Access Point
  WiFi.softAP(ap_ssid, ap_password);

  // Mendapatkan alamat IP Access Point
  IPAddress apIP = WiFi.softAPIP();

  // Menampilkan informasi Access Point
  Serial.println("Access Point aktif!");

  Serial.print("SSID        : ");
  Serial.println(ap_ssid);

  Serial.print("IP Address  : ");
  Serial.println(apIP);
}

void loop() {
  // Mendapatkan jumlah perangkat yang terhubung
  int jumlahClient = WiFi.softAPgetStationNum();

  // Menampilkan jumlah perangkat setiap 5 detik
  Serial.print("Jumlah perangkat terhubung: ");
  Serial.println(jumlahClient);

  delay(5000);
}