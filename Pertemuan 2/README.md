
# Modul 2: Konfigurasi Jaringan

---

### Identitas

Nama: Windi Sulaiman Ismansa   
NIM: H1H024005    
Mata Kuliah: Praktikum Sistem Internet of Things / TK24505   
Modul: 2 – Konfigurasi Jaringan   
Shift: A

---

## Tujuan Praktikum

1. Memahami cara kerja konfigurasi WiFi pada ESP8266 menggunakan pustaka `ESP8266WiFi.h`.
2. Mengimplementasikan ESP8266 sebagai **Station (STA)** untuk terhubung ke jaringan WiFi yang telah tersedia serta mengamati proses dan status koneksinya.
3. Mengimplementasikan ESP8266 sebagai **Access Point (AP)** untuk membuat jaringan WiFi yang dapat diakses oleh smartphone atau laptop secara langsung.
4. Mengidentifikasi dan menganalisis parameter jaringan pada ESP8266, meliputi IP address, MAC address, RSSI, serta jumlah perangkat yang terhubung.
5. Menganalisis perbedaan fungsi mode STA, AP, dan AP+STA berdasarkan proses konfigurasi dan peran ESP8266 dalam jaringan.
6. Menerapkan mekanisme koneksi ulang (**reconnect**) pada mode STA ketika koneksi WiFi terputus.
7. Menerapkan mode **AP+STA** agar ESP8266 dapat terhubung ke jaringan WiFi sekaligus menyediakan Access Point untuk kebutuhan konfigurasi atau komunikasi dengan perangkat lain.

---

## Library 

| Library | Kegunaan |
|---|---|
| `ESP8266WiFi.h` | Digunakan Untuk memanggil modul wifi yang ada pada board esp.
---

## Board yang Digunakan

**NodeMCU 1.0 (ESP8266 – ESP-12E Module)**

---

## Percobaan 2A – Mode Station (STA)
### Penjelasan Fungsi STA

| Fungsi / Kode | Kegunaan |
|---|---|
| `#include <ESP8266WiFi.h>` | Memanggil library WiFi untuk board ESP8266. |
| `Serial.begin(115200)` | Membuka komunikasi dengan Serial Monitor pada baud rate 115200 untuk menampilkan log dan informasi jaringan. |
| `WiFi.mode(WIFI_STA)` | Mengatur ESP8266 agar bekerja dalam mode **Station**, yaitu sebagai klien yang terhubung ke jaringan WiFi yang sudah ada. |
| `WiFi.begin(ssid, password)` | Memulai proses autentikasi dan koneksi ESP8266 ke jaringan WiFi menggunakan SSID dan password yang diberikan. |
| `WiFi.status()` | Mengembalikan status koneksi WiFi saat ini. Nilai `WL_CONNECTED` menandakan ESP8266 sudah berhasil terhubung. |
| `WiFi.localIP()` | Mengembalikan alamat IP yang diberikan oleh router (via DHCP) kepada ESP8266 setelah berhasil terhubung. |
| `WiFi.macAddress()` | Mengembalikan alamat MAC (Media Access Control) unik dari perangkat ESP8266 ini dalam format string. |
| `WiFi.RSSI()` | Mengembalikan nilai kekuatan sinyal WiFi (Received Signal Strength Indicator) dalam satuan dBm. Nilai mendekati 0 berarti sinyal lebih kuat. |
| `pinMode(ledPin, OUTPUT)` | Mengatur mode pin GPIO 2 (D4) sebagai output digital untuk mengendalikan LED indikator. |
| `digitalWrite(ledPin, HIGH/LOW)` | Mengatur kondisi output GPIO untuk menyalakan (`LOW`) atau mematikan (`HIGH`) LED indikator pada NodeMCU. |
| `delay(ms)` | Memberikan jeda waktu eksekusi program dalam satuan milidetik. |

---

### Penjelasan Percabangan STA

Pada Percobaan 2A terdapat dua percabangan utama:

#### 1. Percabangan Loop Menunggu Koneksi (`while`)

```cpp
while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
}
```

Ini adalah percabangan berulang (loop kondisi). Selama `WiFi.status()` belum bernilai `WL_CONNECTED` (kondisi TRUE), program akan terus menunggu — mencetak titik setiap 500ms sebagai indikator proses. Begitu ESP8266 berhasil terhubung, kondisi menjadi FALSE dan eksekusi keluar dari `while`, berlanjut ke blok berikutnya untuk menampilkan informasi jaringan.

#### 2. Percabangan Status Koneksi di `loop()` (`if-else`)

```cpp
if (WiFi.status() == WL_CONNECTED) {
  Serial.println("Status: Terhubung");
} else {
  Serial.println("Status: Terputus");
  digitalWrite(ledPin, HIGH); // LED mati
}
```

Setiap 5 detik, program mengecek apakah koneksi masih aktif. Jika kondisi bernilai TRUE (masih terhubung), program mencetak status terhubung dan LED tetap menyala. Jika FALSE atau koneksi putus, program mencetak status terputus dan mematikan LED sebagai indikator bahwa koneksi hilang.

---

**Penjelasan Modifikasi: Pertanyaan Praktikum 2A**
- Logika koneksi dipisahkan ke fungsi `connectWiFi()` agar bisa dipanggil berulang kali baik saat `setup()` maupun saat reconnect di `loop()`.
- Variabel `retryCount` melacak berapa kali ESP8266 sudah mencoba koneksi ulang.
- Variabel `maxRetry` membatasi jumlah percobaan agar program tidak terus-menerus mencoba tanpa akhir.
- `WiFi.disconnect()` dipanggil sebelum `WiFi.begin()` baru untuk membersihkan state koneksi lama dan mencegah konflik.
- LED dimatikan saat koneksi putus dan dinyalakan kembali saat reconnect berhasil.

---

## Percobaan 2B – Mode Access Point (AP)
### Penjelasan Fungsi AP

| Fungsi / Kode | Kegunaan |
|---|---|
| `#include <ESP8266WiFi.h>` | Library WiFi untuk ESP8266. Menyediakan semua fungsi WiFi termasuk SoftAP. |
| `Serial.begin(115200)` | Membuka komunikasi Serial Monitor untuk menampilkan informasi status AP dan jumlah klien. |
| `WiFi.mode(WIFI_AP)` | Mengatur ESP8266 agar bekerja dalam mode **Access Point**, yaitu sebagai penyedia jaringan WiFi mandiri tanpa memerlukan router eksternal. |
| `WiFi.softAP(ssid, password)` | Mengaktifkan fungsi Access Point pada ESP8266 dan membuatnya memancarkan sinyal WiFi dengan SSID dan password yang ditentukan. |
| `WiFi.softAPIP()` | Mengembalikan alamat IP dari Access Point yang sedang aktif. Nilai default adalah `192.168.4.1`. |
| `WiFi.softAPgetStationNum()` | Mengembalikan jumlah perangkat (klien) yang sedang aktif terhubung ke Access Point ESP8266. |
| `delay(5000)` | Memberikan jeda 5 detik antara setiap pengecekan dan penampilan jumlah klien. |

---

**Penjelasan Modifikasi: Pertanyaan Praktikum 2B**
- `WiFi.mode(WIFI_AP_STA)` adalah kunci utama yang mengaktifkan kedua mode secara bersamaan.
- AP diinisialisasi terlebih dahulu agar perangkat konfigurasi bisa terhubung meskipun koneksi ke router belum selesai.
- STA terhubung ke router secara terpisah; keduanya berjalan independen dan saling tidak mengganggu.
- Fungsi `loop()` memantau status kedua mode: koneksi ke router (STA) dan jumlah klien AP.
- **Skenario nyata:** Mode AP+STA sangat berguna saat melakukan provisioning perangkat IoT — pengguna terhubung ke AP ESP8266 untuk mengatur konfigurasi WiFi, sementara ESP8266 juga sudah terhubung ke internet melalui STA untuk mengirim data ke cloud.

---

## Keterangan Rangkaian

### Percobaan 2A – Mode Station (STA) dengan LED Indikator

```
Keterangan:
- GPIO 2 (D4) → LED Anoda (+)
- LED Katoda (-) → GND
- LED built-in NodeMCU (GPIO 2) aktif LOW:
    LOW  = LED Menyala (koneksi WiFi berhasil)
    HIGH = LED Mati    (tidak terhubung / awal program)
```

### Percobaan 2B – Mode Access Point (AP)

```
Koneksi Jaringan:
  [ESP8266 AP] ←──WiFi──→ [Smartphone / Laptop]
   192.168.4.1              192.168.4.x (DHCP)
```
---

## Detail Percobaan

### Percobaan 2A – Mode Station (STA)

Pada percobaan ini, ESP8266 dikonfigurasi sebagai Station yang terhubung ke jaringan WiFi smartphone. Program berhasil menampilkan IP Address, MAC Address, dan nilai RSSI melalui Serial Monitor. LED indikator pada GPIO 2 menyala ketika koneksi WiFi berhasil terbentuk. Nilai RSSI berada antara -39 dBm hingga -55 dBm hal ini menandakan sinyal dalam kondisi baik. IP address yang diperoleh (`10.30.129.194`) diberikan secara otomatis oleh server DHCP jaringan WiFi yang digunakan. Pengujian dengan kredensial salah (SSID/password keliru) menghasilkan status "WiFi connection failed" dan ESP8266 tidak memperoleh IP address sama sekali.

### Percobaan 2B – Mode Access Point (AP)

ESP8266 dikonfigurasi sebagai Access Point dengan SSID `ESP32_AccessPoint` dan password `12345678`. Access Point berhasil aktif dengan IP default `192.168.4.1`. Perangkat smartphone dan laptop berhasil mendeteksi SSID dan terhubung ke AP. Jumlah yang terhubung total ada 2 dengan diakhir yang 1 lose connet dan serial monitor menunjukkan jumlah klient akhir ialah 1. Hal tersebut menunjukkan bahwa fungsi `WiFi.softAPgetStationNum()` berhasil memantau jumlah klien secara real-time.

---

## Dokumentasi
(Sesudah Connect WiFi)["https://github.com/user-attachments/assets/bd85b984-3ad7-45fc-abce-2e446860c6f4" /]   
(Sebelum Connect WiFi)["https://github.com/user-attachments/assets/206f864f-7fb1-4b70-82d7-ee1ef3281148" /]   
(Serial Monitor)["https://github.com/user-attachments/assets/63b922e2-2556-428a-995a-3ff9b006976d" /]




---
