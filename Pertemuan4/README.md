<img width="1600" height="794" alt="image" src="https://github.com/user-attachments/assets/824150e1-ac22-445c-be82-7bb76fb5f1b0" /># Modul 4: Komunikasi dan Pertukaran Data

---

### Identitas

Nama: Windi Sulaiman Ismansa  
NIM: H1H024005  
Mata Kuliah: Praktikum Sistem Internet of Things / TK24505  
Modul: 4 – Komunikasi dan Pertukaran Data  
Shift: A

---

## Tujuan Praktikum

1. Memahami konsep pertukaran data dua arah (*bidirectional*) pada sistem IoT.
2. Memahami mekanisme *subscribe* dan proses deserialisasi data JSON pada ESP8266.
3. Mengimplementasikan penerimaan perintah kendali melalui MQTT untuk menggerakkan aktuator secara *real-time*.
4. Mengimplementasikan sistem IoT yang dapat mempublikasikan data sensor dan menerima perintah kendali secara bersamaan (*full duplex*).
5. Mampu menganalisis mekanisme pertukaran data IoT secara menyeluruh pada sistem yang saling terhubung.

---

## Library

| **Library** | **Kegunaan** |
|---|---|
| `ESP8266WiFi.h` | Menghubungkan ESP8266 ke jaringan WiFi. |
| `PubSubClient.h` | Menangani komunikasi MQTT (publish, subscribe, dan koneksi ke broker). |
| `ArduinoJson.h` | Melakukan serialisasi (objek ke JSON string) dan deserialisasi (JSON string ke objek) data format JSON. |
| `DHT.h` | Membaca data suhu dan kelembaban dari sensor DHT11. |

---

## Board yang Digunakan

NodeMCU 1.0 (ESP8266 – ESP-12E Module)

---

## Percobaan 4A – Subscribe dan Deserialisasi Data JSON untuk Kendali Aktuator

### Penjelasan Fungsi

| **Fungsi / Kode** | **Kegunaan** |
|---|---|
| `WiFi.begin(ssid, password)` | Memulai proses autentikasi dan koneksi ESP8266 ke jaringan WiFi. |
| `WiFi.status()` | Mengecek status koneksi WiFi saat ini. Mengembalikan `WL_CONNECTED` jika sudah terhubung. |
| `PubSubClient client(espClient)` | Membuat objek client MQTT yang menggunakan koneksi TCP/IP WiFi sebagai transport layer. |
| `client.setServer(server, port)` | Mendaftarkan alamat broker MQTT dan nomor port yang akan dihubungi. |
| `client.setCallback(callback)` | Mendaftarkan fungsi callback yang akan dipanggil otomatis setiap kali ada pesan masuk dari topic yang di-subscribe. |
| `void callback(topic, payload, length)` | Fungsi yang menangani setiap pesan MQTT yang masuk mengonversi payload, mem-parsing JSON, dan mengendalikan LED. |
| `client.connect(clientId)` | Mengirimkan permintaan koneksi ke broker MQTT menggunakan Client ID unik. |
| `client.subscribe(topic)` | Mendaftarkan ESP8266 untuk menerima setiap pesan yang dipublikasikan ke topic tertentu. |
| `client.connected()` | Mengecek apakah koneksi ke broker MQTT masih aktif. |
| `client.loop()` | Menjaga koneksi ke broker tetap aktif (keep-alive) dan memproses pesan masuk secara berkelanjutan. Wajib dipanggil di setiap iterasi `loop()`. |
| `deserializeJson(doc, pesan)` | Mengonversi string JSON yang diterima dari broker menjadi objek `JsonDocument` yang dapat diakses field-nya. |
| `doc["perintah"]` | Mengakses nilai dari field bernama `"perintah"` di dalam objek JSON hasil parsing. |
| `digitalWrite(ledPin, HIGH/LOW)` | Menyalakan (`HIGH`) atau mematikan (`LOW`) LED aktuator pada pin GPIO. |

---

### Penjelasan Percabangan

#### 1. Loop menunggu koneksi WiFi (`while`)

```cpp
while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
}
```

Percangan ini digunakan untuk menunggu ESP8266 berhasil terhubung ke WiFi.
#### 2. Loop reconnect ke broker MQTT (`while`)

```cpp
while (!client.connected()) {
  // mencoba koneksi
  if (client.connect(clientId.c_str())) {
    client.subscribe(topicPerintah);
  } else {
    delay(2000);
  }
}
```

Program terus mencoba Selama koneksi ke broker belum berhasil. Subscribe melakukan pendaftaran ulang ke topic setiap kali terjadi reconnect, karena sesi broker baru tidak mewarisi subscription dari sesi sebelumnya.

#### 3. Cek dan reconnect MQTT di `loop()` (`if`)

```cpp
if (!client.connected()) {
  hubungkanMQTT();
}
```

Setiap iterasi `loop()`, program mengecek apakah koneksi MQTT masih aktif. Jika terputus, fungsi `hubungkanMQTT()` dipanggil untuk menyambung ulang dan mendaftarkan ulang subscription.

#### 4. Validasi format JSON (`if`)

```cpp
if (error) {
  Serial.print("Gagal parsing JSON: ");
  Serial.println(error.c_str());
  return;
}
```

Jika pesan yang diterima bukan format JSON yang valid, `deserializeJson()` mengembalikan error. Program mencetak pesan kesalahan ke Serial Monitor lalu mengeksekusi `return` untuk menghentikan fungsi callback sehingga status LED tidak berubah akibat pesan yang tidak valid.

#### 5. Kendali aktuator berdasarkan nilai perintah (`if-else if`)

```cpp
if (String(perintah) == "ON") {
  digitalWrite(ledPin, HIGH);
} else if (String(perintah) == "OFF") {
  digitalWrite(ledPin, LOW);
}
```

Setelah JSON berhasil di-parsing, nilai field `"perintah"` dibandingkan. Jika bernilai `"ON"` maka LED dinyalakan, jika `"OFF"` maka LED dimatikan.

---

### Broker, Topic, dan Format Data

```text
Broker : broker.hivemq.com
Port   : 1883
Topic  : unsoed/tk245004/AB3/perintah
```

```json
{ "perintah": "ON" }
{ "perintah": "OFF" }
```

---
### Jawaban Pertanyaan Praktikum 4A

### Modifikasi – Kendali Kecerahan LED via PWM percobaan 4A

Modifikasi program agar data JSON yang diterima memuat nilai `intensitas` untuk mengatur kecerahan LED menggunakan `analogWrite()` (PWM).
```cpp
if (String(perintah) == "ON") {
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
  ``

**Contoh pesan JSON yang dikirim dari MQTT client:**
```json
{ "perintah": "ON", "intensitas": 1023 }
{ "perintah": "ON", "intensitas": 800 }
{ "perintah": "OFF" }
```

**Penjelasan perubahan utama:**   

- `constrain(intensitas, 0, 1023)` — membatasi nilai intensitas dalam range valid PWM ESP8266 (0–1023) untuk mencegah nilai di luar batas.
- `analogWrite(ledPin, intensitas)` — menulis nilai PWM ke pin LED untuk mengatur kecerahan secara proporsional. Nilai `0` = mati, `1023` = kecerahan penuh.
- Jika perintah `"ON"` diterima tanpa field `intensitas`, LED tetap menyala dengan kecerahan penuh (`1023`) sebagai fallback.

---

## Percobaan 4B – Pertukaran Data Dua Arah (Publish dan Subscribe Bersamaan)

---

### Penjelasan Fungsi

| **Fungsi / Kode** | **Kegunaan** |
|---|---|
| `WiFi.begin(ssid, password)` | Memulai proses koneksi ESP8266 ke jaringan WiFi. |
| `DHT dht(DHTPIN, DHTTYPE)` | Membuat objek sensor DHT11 dengan pin data dan tipe sensor yang ditentukan. |
| `dht.begin()` | Menginisialisasi komunikasi antara ESP8266 dengan sensor DHT11. |
| `dht.readTemperature()` | Membaca nilai suhu terkini dari sensor DHT11 dalam satuan Celsius. |
| `isnan(suhu)` | Memeriksa apakah nilai suhu yang dibaca valid. Mengembalikan `true` jika sensor gagal dibaca (Not a Number). |
| `PubSubClient client(espClient)` | Membuat objek client MQTT menggunakan koneksi TCP/IP WiFi. |
| `client.setServer(server, port)` | Mendaftarkan alamat dan port broker MQTT. |
| `client.setCallback(callback)` | Menghubungkan fungsi callback untuk memproses pesan masuk. |
| `client.connect(clientId)` | Mengirimkan permintaan koneksi ke broker dengan Client ID unik. |
| `client.subscribe(topic)` | Mendaftarkan ESP8266 untuk menerima pesan dari topic perintah. |
| `client.connected()` | Mengecek apakah koneksi ke broker masih aktif. |
| `client.loop()` | Menjaga koneksi aktif dan memproses semua pesan masuk — harus dipanggil di setiap iterasi `loop()`. |
| `millis()` | Mengembalikan waktu (milidetik) sejak ESP8266 mulai berjalan. Digunakan untuk timing non-blocking. |
| `serializeJson(doc, buffer)` | Mengonversi objek `JsonDocument` menjadi string JSON untuk dikirim via MQTT. |
| `client.publish(topic, buffer)` | Mempublikasikan data sensor dalam format JSON ke topic MQTT yang ditentukan. |
| `deserializeJson(doc, pesan)` | Mengonversi string JSON yang diterima menjadi objek yang dapat diakses field-nya. |

---

### Penjelasan Percabangan

#### 1. Reconnect MQTT di `loop()` (`if`)

```cpp
if (!client.connected()) hubungkanMQTT();
```

Setiap iterasi `loop()` mengecek status koneksi broker. Jika terputus, ESP8266 langsung mencoba menyambung dan mendaftarkan ulang subscription sebelum lanjut memproses hal lain.

#### 2. Mekanisme non-blocking publish dengan `millis()` (`if`)

```cpp
if (millis() - waktuTerakhirPublish > intervalPublish) {
  waktuTerakhirPublish = millis();
  // publish data sensor
}
```

Program mengecek selisih waktu antara sekarang (`millis()`) dan waktu publish terakhir. Jika selisihnya sudah melebihi 5 detik, maka data sensor dipublish dan timestamp diperbarui. Selama menunggu interval, CPU tetap bebas mengeksekusi `client.loop()` sehingga perintah subscribe tetap dapat diproses secara instan — berbeda dengan `delay()` yang memblokir seluruh CPU.

#### 3. Validasi data sensor (`if`)

```cpp
if (!isnan(suhu)) {
  // publish data
}
```

Data dari sensor DHT11 hanya dipublish jika nilainya valid. Jika sensor gagal dibaca dan mengembalikan `NaN`, blok publish dilewati untuk mencegah pengiriman data rusak ke broker.

#### 4. Kendali LED dengan operator ternary (`? :`)

```cpp
digitalWrite(ledPin, String(perintah) == "ON" ? HIGH : LOW);
```

Bentuk ringkas dari `if-else`: jika nilai perintah adalah `"ON"` maka LED dinyalakan (`HIGH`), jika bukan maka LED dimatikan (`LOW`). Keduanya dieksekusi dalam satu baris tanpa mengurangi kejelasan logika.

---

### Broker, Topic, dan Format Data

```text
Broker        : broker.hivemq.com
Port          : 1883
Topic Data    : unsoed/tk245004/AB3/data       (publish suhu dari ESP8266)
Topic Perintah: unsoed/tk245004/AB3/perintah   (subscribe perintah ke ESP8266)
```

**Data yang dipublish ESP8266 → broker (setiap 5 detik):**
```json
{ "suhu": 28.5 }
```

**Perintah dari broker → ESP8266 (dikirim manual via MQTT client):**
```json
{ "perintah": "ON" }
{ "perintah": "OFF" }
```

---
### Modifikasi – Dua Aktuator dengan Topic Berbeda Percobaan 4B

Modifikasi program dengan menambahkan topic baru untuk mengendalikan aktuator kedua (buzzer), di mana fungsi callback membedakan pesan berdasarkan topic asal.   


```cpp
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

```
**Perintah MQTT untuk mengendalikan dua aktuator:**
```json
// Topic: unsoed/tk245004/AB3/perintah → mengendalikan LED
{ "perintah": "ON" }
{ "perintah": "OFF" }

// Topic: unsoed/tk245004/AB3/buzzer → mengendalikan Buzzer
{ "perintah": "ON" }
{ "perintah": "OFF" }
```

**Penjelasan perubahan utama:**
- `topicPerintahBuzzer` — topic baru yang di *subscribe* khusus untuk menerima perintah kendali buzzer.
- `pinMode(buzzerPin, OUTPUT)` — inisialisasi pin buzzer sebagai output digital di `setup()`.
- `client.subscribe(topicPerintahBuzzer)` — pendaftaran topic buzzer dilakukan bersamaan dengan topic LED di dalam `hubungkanMQTT()` agar keduanya selalu terdaftar ulang setiap *reconnect*.
- `if (String(topic) == topicPerintahLED) / else if (String(topic) == topicPerintahBuzzer)` — satu fungsi *callback* yang sama menangani kedua topic dengan cara membandingkan parameter `topic` (berisi nama topic asal pesan) sebelum menentukan aktuator mana yang dikendalikan.

---

## Skematik Rangkaian

### Percobaan 4A
(Percobaan 4B)["https://github.com/user-attachments/assets/69076fd6-efdf-4d0c-b610-cad88f0baf3a"]

### Percobaan 4B
(Skematik percobaan 4B)["https://github.com/user-attachments/assets/b6a8ff10-3785-4ed8-9a5b-ba4c36f2ccc3"]

## Detail Percobaan

### Percobaan 4A – Subscribe dan Deserialisasi Data JSON

Pada percobaan ini, ESP8266 dikonfigurasi sebagai subscriber MQTT yang menerima perintah kendali LED melalui topic `unsoed/tk245004/AB3/perintah`. Setelah ESP8266 berhasil terhubung ke broker HiveMQ dan melakukan subscribe, perintah dikirim secara manual dari aplikasi MQTT Explorer. Pesan JSON `{"perintah":"ON"}` berhasil diterima, di-parsing, dan LED menyala sesuai perintah. Begitu pula pesan `{"perintah":"OFF"}` yang berhasil mematikan LED. Hasil percobaan menunjukkan seluruh  pengujian berjalan sesuai spesifikasi dan Serial Monitor menampilkan pesan yang diterima beserta hasil parsing, dan LED merespons secara real-time setiap kali perintah baru masuk.

### Percobaan 4B – Pertukaran Data Dua Arah
---

## Dokumentasi
(Perintah OFF)["https://github.com/user-attachments/assets/18c18466-5289-48fe-a3fd-cac44b8bd8a3"]   
["https://github.com/user-attachments/assets/19650cf3-e481-4c68-a1d3-7401b4fa3d25"]

(Perintah ON)["https://github.com/user-attachments/assets/e0d09295-b20b-42c8-ae8b-c55131cc7af5"]   
(Hasil percobaan)["https://github.com/user-attachments/assets/f0dcd03f-0663-43cf-bd90-50416e04bc8e"]



