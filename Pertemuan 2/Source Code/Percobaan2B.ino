#include <ESP8266WiFi.h>

const char* ap_ssid = "ESPTunaAsap";
const char* ap_password = "12345678"; 

void setup() {
  Serial.begin(115200);

  // Set mode WiFi menjadi Access Point
  WiFi.mode(WIFI_AP);

  // Membuat Access Point
  WiFi.softAP(ap_ssid, ap_password);

  // Mendapatkan IP Address Access Point
  IPAddress apIP = WiFi.softAPIP();

  Serial.println("Access Point aktif!");

  Serial.print("SSID        : ");
  Serial.println(ap_ssid);

  Serial.print("IP Address  : ");
  Serial.println(apIP);
}

void loop() {
  // Menampilkan jumlah perangkat yang terhubung setiap 5 detik
  int jumlahClient = WiFi.softAPgetStationNum();

  Serial.print("Jumlah perangkat terhubung: ");
  Serial.println(jumlahClient);

  delay(5000);
}
