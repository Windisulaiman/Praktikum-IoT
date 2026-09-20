# Modul 3: Protokol Komunikasi IoT

---

### Identitas

Nama: Windi Sulaiman Ismansa  
NIM: H1H024005  
Mata Kuliah: Praktikum Sistem Internet of Things / TK24505  
Modul: 3 – Protokol Komunikasi IoT  
Shift: A

---

## Tujuan Praktikum

1. Memahami konsep dasar protokol komunikasi pada sistem IoT.
2. Memahami karakteristik dan perbedaan protokol HTTP dan MQTT dalam konteks IoT.
3. Mengimplementasikan pengiriman data dari ESP32 ke server menggunakan protokol HTTP (metode POST).
4. Melakukan pertukaran data dari ESP32 ke broker MQTT menggunakan pola publish-subscribe dengan format JSON.
5. Menganalisis kelebihan dan kekurangan masing-masing protokol untuk berbagai skenario aplikasi IoT.

---

## Library

| **Library** | **Kegunaan** |
|---|---|
| `ESP8266WiFi.h` | Menghubungkan ESP8266 ke jaringan WiFi. |
| `ESP8266HTTPClient.h` | Membuat komunikasi HTTP pada ESP8266. |
| `PubSubClient.h` | Menangani komunikasi MQTT. |
| `ArduinoJson.h` | Membuat dan mengubah data menjadi format JSON. |

---

## Board yang Digunakan   
* NodeMCU 1.0 (ESP8266 – ESP-12E Module)
---

## Percobaan 3A – HTTP POST

### Penjelasan Fungsi

| **Fungsi / Kode** | **Kegunaan** |
|---|---|
| `WiFi.begin(ssid, password)` | Menghubungkan ESP8266 ke WiFi. |
| `WiFi.status()` | Mengecek status koneksi WiFi. |
| `HTTPClient http` | Membuat objek untuk komunikasi HTTP. |
| `http.begin(wifiClient, serverUrl)` | Menentukan server tujuan HTTP. |
| `http.addHeader("Content-Type", "application/json")` | Menentukan tipe data yang dikirim, yaitu JSON. |
| `JsonDocument doc` | Menyimpan data yang akan dikirim. |
| `serializeJson(doc, requestBody)` | Mengubah data menjadi string JSON. |
| `http.POST(requestBody)` | Mengirim data menggunakan metode HTTP POST. |
| `http.getString()` | Membaca response dari server. |
| `http.end()` | Mengakhiri koneksi HTTP. |

### Penjelasan Percabangan

```cpp
while (WiFi.status() != WL_CONNECTED) {
  delay(500);
}
```

Percangan ini digunakan untuk menunggu ESP8266 berhasil terhubung ke WiFi.

```cpp
if (WiFi.status() == WL_CONNECTED) {
  // mengirim data
}
```

Percangan untuk memastikan data dapat dikirim apabila koneksi WiFi masih aktif.

```cpp
if (httpResponseCode > 0) {
  // response berhasil diterima
} else {
  // pengiriman gagal
}
```

Program untuk membedakan kondisi request berhasil dan gagal berdasarkan kode response HTTP.

### Data JSON

```json
{
  "suhu": 28.5,
  "kelembaban": 65.0
}
```

---

## Percobaan 3B – MQTT

### Penjelasan Fungsi

| **Fungsi / Kode** | **Kegunaan** |
|---|---|
| `WiFi.begin(ssid, password)` | Menghubungkan ESP8266 ke WiFi. |
| `PubSubClient client(espClient)` | Membuat client MQTT. |
| `client.setServer(mqttServer, mqttPort)` | Menentukan broker dan port MQTT. |
| `client.connect(clientId.c_str())` | Menghubungkan perangkat ke broker MQTT. |
| `client.connected()` | Mengecek status koneksi MQTT. |
| `client.loop()` | Menjaga komunikasi MQTT tetap berjalan. |
| `client.publish(mqttTopic, buffer)` | Mengirim data ke topic MQTT. |
| `serializeJson(doc, buffer)` | Mengubah data sensor menjadi JSON. |

### Broker dan Topic

```text
Broker : broker.hivemq.com
Port   : 1883
Topic  : unsoed/tk245004/nim10/sensor
```

### Penjelasan Percabangan

```cpp
while (!client.connected()) {
  // mencoba koneksi MQTT
}
```

Function agar program terus mencoba menghubungkan perangkat ke broker sampai berhasil.

```cpp
if (!client.connected()) {
  hubungkanMQTT();
}
```

Jika koneksi MQTT terputus, perangkat akan mencoba melakukan koneksi ulang.

### Data yang Dikirim

```json
{
  "suhu": 28.5,
  "kelembaban": 65.0
}
```

Data dikirim ke topic menggunakan:

```cpp
client.publish(mqttTopic, buffer);
```

Pengiriman dilakukan setiap 5 detik.

---   

### Modifikasi Percobaan 3A - Pertanyaan praktikum 3A nomor 4

Modifikasi pada percobaan  dilakukan dengan menambahkan data `waktu` menggunakan fungsi `millis()`. Dengan penambahan tersebut, data yang dikirim tidak hanya berisi nilai suhu dan kelembaban, tetapi juga informasi waktu relatif perangkat. Letak perubahan pada program terletak pada bagian berikut: 

```cpp
{
  doc["suhu"] = 28.5;               
  doc["kelembaban"] = 65.0;        
  doc["waktu"] = millis();  
}
```

Waktu berasal dari `millis()` sehingga dapat digunakan untuk mengetahui waktu relatif sejak ESP8266 mulai berjalan.

## Detail Percobaan

### Percobaan 3A – HTTP POST

Percobaan dilakukan dengan menghubungkan ESP8266 ke hotspot WiFi terlebih dahulu, kemudian membuat data suhu dan kelembaban dalam format JSON. Setelah data dibuat kemudian dikirim menggunakan HTTP POST menuju `httpbin.org/post`. Step selanjutnya server memberikan response yang kemudian ditampilkan pada Serial Monitor dengan program yang akan mengirim data kembali setiap 10 detik.


### Percobaan 3B – MQTT

Pada percobaan 3B ini, ESP8266 terlebih dahulu dihubungkan ke jaringan WiFi, kemudian dikoneksikan ke broker MQTT HiveMQ. Setelah koneksi berhasil data kelembaban dan suhu yang sebelumnya dibuat pada program dikirim menggunakan metode publish ke topic unsoed/tk245004/nim10/sensor. Kemudian hasil dapat dilihat di MQTT Exproler dengan memasukkan topic yang sudah dibuat. Durasi waktu pengiriman data dilakukan secara berkala setiap 5 detik. Program juga mengecek status koneksi MQTT, sehingga apabila koneksi terputus, ESP8266 akan mencoba terhubung kembali ke broker secara otomatis.

---

## Skematik Rangkaian
[Rangkaian Percobaan](https://github.com/user-attachments/assets/0a176e11-9808-4409-843a-8adf1ac4da14)

---

## Dokumentasi

### Percobaan 3A – HTTP POST
[Hasil Percobaan pada serial monitor](https://github.com/user-attachments/assets/49999583-969a-40c3-bbcf-19394908d522)   



### Percobaan 3B – MQTT
[Serial monitor Percobaan](https://github.com/user-attachments/assets/0f4250d9-28da-4568-b660-4fbba4dcefc3)   

[Tampilan data yang dikirim pada MQTT Exproler 1](https://github.com/user-attachments/assets/9bef625a-4f14-43a9-874d-dddb9e2d5507)   
[Tampilan data yang dikirim pada MQTT Exproler 2 ](https://github.com/user-attachments/assets/67693c99-3edc-4dd2-b84c-60e1cb9cd142)

