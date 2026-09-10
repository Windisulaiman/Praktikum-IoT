#include <ESP8266WiFi.h>              // Memanggil library WiFi untuk ESP8266

// ===============================
// KONFIGURASI WIFI RUMAH (STA)
// ===============================
const char* ssidSTA = "Authentic Nasgor Tuna Asap";       // Nama WiFi rumah yang akan dihubungkan
const char* passwordSTA = "12345678";     // Password WiFi rumah

// ===============================
// KONFIGURASI ACCESS POINT (AP)
// ===============================
const char* ssidAP = "ESP8266_TunaAsap";             // Nama WiFi yang dibuat oleh ESP8266
const char* passwordAP = "12345678";            // Password Access Point, minimal 8 karakter

void setup() {
  Serial.begin(115200);                         // Memulai komunikasi Serial dengan baud rate 115200
  delay(100);                                   // Memberikan waktu singkat agar Serial siap digunakan

  Serial.println();                             // Membuat baris kosong pada Serial Monitor
  Serial.println("Memulai ESP8266 AP + STA");   // Menampilkan informasi bahwa program dimulai

  // Mengatur ESP8266 agar bekerja dalam mode Station dan Access Point secara bersamaan
  WiFi.mode(WIFI_AP_STA);

  // ===============================
  // MENGHUBUNGKAN ESP8266 KE WIFI RUMAH
  // ===============================

  WiFi.begin(ssidSTA, passwordSTA);             // Memulai koneksi ESP8266 ke WiFi rumah

  Serial.print("Menghubungkan ke WiFi rumah");  // Menampilkan proses koneksi pada Serial Monitor

  // Selama ESP8266 belum mendapatkan koneksi WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);                                 // Menunggu selama 500 ms
    Serial.print(".");                          // Menampilkan titik sebagai indikator proses koneksi
  }

  // Jika koneksi berhasil
  Serial.println();
  Serial.println("WiFi rumah berhasil terhubung!");

  // Menampilkan alamat IP ESP8266 yang diberikan oleh router rumah
  Serial.print("IP STA: ");
  Serial.println(WiFi.localIP());

  // ===============================
  // MEMBUAT ACCESS POINT ESP8266
  // ===============================

  WiFi.softAP(ssidAP, passwordAP);              // Membuat Access Point dengan SSID dan password yang ditentukan

  Serial.println("Access Point berhasil dibuat!");

  // Menampilkan nama WiFi Access Point
  Serial.print("SSID AP: ");
  Serial.println(ssidAP);

  // Menampilkan alamat IP Access Point ESP8266
  Serial.print("IP AP: ");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  // Mengecek apakah koneksi ESP8266 ke WiFi rumah masih aktif
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Status STA: Terhubung");    // Menampilkan bahwa ESP masih terhubung ke WiFi rumah
  } else {
    Serial.println("Status STA: Terputus");     // Menampilkan bahwa ESP terputus dari WiFi rumah
  }

  // Menampilkan jumlah perangkat yang sedang terhubung ke Access Point ESP8266
  Serial.print("Jumlah perangkat AP: ");
  Serial.println(WiFi.softAPgetStationNum());

  delay(5000);                                  // Menunggu 5 detik sebelum melakukan pengecekan lagi
}
