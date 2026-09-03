## Pertemuan 1   
___  
### Identitas  
Nama: Windi Sulaiman Ismansa   
NIM: H1H024005
Shift : A   
___
### Penjelasan Code   
```ino
#include <DHT.h> //berfungsi untuk mengimpor library DHT22

#define DHTPIN 4 // Deklarasi pin DHT11 yang berapa di GPIO 4 ESP32
#define DHTTYPE DHT11 // Menentukan jenis sensor yang digunakan
#define RELAYPIN 5 // Deklarasi pin Relay di GPIO 5 ESP32

DHT dht(DHTPIN, DHTTYPE); 

const float suhuThreshold = 30.0; // Menentukan batas ambang suhu sebesar 30.0 °C sebagai pemicu Relay atau aktuator

void setup() { 
  Serial.begin(115200); // Komunikasi Serial Monitor dengan kecepatan transfer data 115200 baud rate
  pinMode(DHTPIN, INPUT_PULLUP); // Mengatur GPIO 4 sebagai input dengan mengaktifkan resistor pull-up internal
  dht.begin(); // Menginisialisasi komunikasi dengan sensor DHT
  pinMode(RELAYPIN, OUTPUT); // Mengatur GPIO 5 (pin relay) sebagai output digital
  digitalWrite(RELAYPIN, LOW); // Memastikan relay berada dalam kondisi mati (LOW) saat perangkat baru dinyalakan
} // Penutup fungsi setup()

void loop() { 
  delay(2000); // Memberikan jeda waktu 2 detik sebelum membaca sensor

  float suhu = dht.readTemperature(); // Membaca data suhu dari sensor dalam satuan Celsius dan menyimpannya ke variabel 'suhu'

  if (isnan(suhu)) { // Memeriksa apakah nilai variabel 'suhu' bernilai not a number
    Serial.println("Gagal membaca data sensor!"); // Menampilkan pesan kegagalan ke Serial Monitor jika sensor gagal dibaca oleh mikrokontroler
    delay(1000); // Memberikan jeda tambahan 1 detik jika terjadi error pembacaan
  } else { 
    Serial.print("Suhu: "); // Mencetak teks "Suhu: " ke Serial Monitor tanpa baris baru
    Serial.print(suhu); // Mencetak nilai angka suhu saat ini ke Serial Monitor
    Serial.print(" °C -> "); // Mencetak teks " °C -> " ke Serial Monitor

    if (suhu > suhuThreshold) { // Mengecek apakah suhu saat ini lebih tinggi dari ambang batas (30.0 °C)
      digitalWrite(RELAYPIN, HIGH); // Mengirim sinyal HIGH ke GPIO 5 untuk menyalakan relay jika suhu diatas 30°C
      Serial.println("Aktuator: ON"); // Mencetak teks "Aktuator: ON" ke Serial Monitor
    } else { 
      digitalWrite(RELAYPIN, LOW); // Mengirim sinyal LOW ke GPIO 5 untuk mematikan relayjika suhu <= 30°C
      Serial.println("Aktuator: OFF"); // Mencetak teks "Aktuator: OFF" ke Serial Monitor
    } 
  }
} 
```
___

### Function
* **setup()**: Fungsi inisialisasi yang hanya dieksekusi satu kali saat perangkat pertama kali dinyalakan untuk mengatur konfigurasi dasar pin dan komunikasi.
* **loop()**: Fungsi utama yang dijalankan secara berulang-ulang tanpa henti selama perangkat dialiri listrik untuk membaca data dan mengontrol aktuator secara real-time.
* **Serial.begin(115200)**: Mengaktifkan komunikasi serial antara ESP32 dengan komputer pada kecepatan 115200 baud rate untuk monitoring.
* **Serial.print() & Serial.println()**: Menampilkan data ke Serial Monitor. **print()** mencetak teks di baris yang sama, sedangkan **println()** mencetak teks lalu membuat baris baru.
* **pinMode()**: Menentukan mode kerja pin GPIO, apakah sebagai masukan sinyal (**INPUT_PULLUP**) atau keluaran sinyal (**OUTPUT**).
* **digitalWrite()**: Mengirim sinyal digital tinggi (**HIGH** / 3.3V) untuk menyalakan alat atau sinyal rendah **LOW** / 0V) untuk mematikan alat pada pin **OUTPUT**.
* **dht.begin()**: Memulai dan mengomunikasikan mikrokontroler dengan modul sensor DHT.
* **dht.readTemperature()**: Membaca nilai suhu lingkungan dari sensor dalam satuan derajat Celsius.
* **isnan()**: Mengecek apakah hasil pembacaan sensor berupa nilai yang tidak valid (*Not a Number* / gagal baca).
* **delay()**: Memberikan jeda waktu penundaan eksekusi program dalam satuan milidetik.
___ 

### Percabangan  
### Penjelasan Logika Percabangan (Conditional Statement)

Pada percobaan ini terdapat dua tahapan percabangan utama yang dijalankan oleh mikrokontroler:

#### 1. Percabangan Validasi Data Sensor  
```ino
if (isnan(suhu)) {
  Serial.println("Gagal membaca data sensor!");
} else
```

Sebelum memproses kontrol aktuator, program terlebih dahulu melakukan pengecekan kualitas data menggunakan fungsi `if (isnan(suhu))`. Fungsi `isnan()` bertugas mengecek apakah data suhu yang dibaca dari sensor bernilai valid atau gagal. Jika kondisi bernilai **TRUE** artinya data pembacaan berupa `NaN` atau rusak, program akan mengeksekusi perintah di dalam blok `if` untuk menampilkan pesan kesalahan *"Gagal membaca data sensor!"* ke Serial Monitor dan menghentikan proses eksekusi aktuator demi keamanan. Sebaliknya, jika kondisi bernilai **FALSE** artinya data suhu yang dibaca berupa angka yang valid, program akan berpindah ke blok `else` untuk melanjutkan ke proses pengolahan logika berikutnya.

#### 2. Percabangan Ambang Batas Tunggal (Pemicu Relay atau Aktuator)
```ino
if (suhu > suhuThreshold) {
  digitalWrite(RELAYPIN, HIGH); // Aktuator ON
} else {
  digitalWrite(RELAYPIN, LOW);  // Aktuator OFF
}
```
Di dalam blok `else` yang valid, program menjalankan percabangan kedua untuk menentukan status relay atau aktuator melalui perintah `if (suhu > suhuThreshold)`. Pada kondisi ini, nilai suhu yang baru dibaca akan dibandingkan langsung dengan nilai batas (*threshold*) yang telah ditentukan, yaitu **30.0 °C**. Apabila suhu saat ini bernilai lebih dari 30.0 °C (**TRUE**), program akan mengeksekusi perintah `digitalWrite(RELAYPIN, HIGH)` untuk mengaktifkan relay (aktuator **ON**). Namun, jika suhu saat ini bernilai kurang dari atau sama dengan 30.0 °C (**FALSE**), kontrol akan dialihkan ke blok `else` yang mengeksekusi perintah `digitalWrite(RELAYPIN, LOW)` untuk mematikan relay (aktuator **OFF**).
___
### Library yang digunakan 
DHT11 sensor Library
### Board yang digunakan   
NodeMCU 1.0 (ESP12E Module)  
___
### Modifikasi program pertanyaan praktikum soal no 4
```ino
#include <DHT.h>

#define DHTPIN 4       
#define DHTTYPE DHT11  
#define RELAYPIN 5     

DHT dht(DHTPIN, DHTTYPE);

// Dua ambang batas (Histeresis)
const float suhuAtas = 30.0;   // Ambang batas atas (°C) - Aktuator ON
const float suhuBawah = 28.0;  // Ambang batas bawah (°C) - Aktuator OFF

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(RELAYPIN, OUTPUT);
  digitalWrite(RELAYPIN, LOW); 
}

void loop() {
  float suhu = dht.readTemperature();

  if (isnan(suhu)) {
    Serial.println("Gagal membaca data sensor!");
  } else {
    Serial.print("Suhu: ");
    Serial.print(suhu);
    Serial.println(" °C");

    // Kendali aktuator berdasarkan logika Histeresis
    if (suhu > suhuAtas) {
      digitalWrite(RELAYPIN, HIGH); // Aktifkan relay jika suhu > 30°C
      Serial.println("Status Aktuator: ON (Suhu > 30°C)");
    } else if (suhu < suhuBawah) {
      digitalWrite(RELAYPIN, LOW);  // Matikan relay jika suhu < 28°C
      Serial.println("Status Aktuator: OFF (Suhu < 28°C)");
    } else {
      // Jika suhu di antara 28°C dan 30°C, pertahankan status sebelumnya
      Serial.println("Status Aktuator: Mempertahankan kondisi sebelumnya ");
    }
  }

  delay(2000); // Jeda pembacaan sensor (2 detik)
}
```
___
### Detail Percobaan   
Pada percobaan 1A dan 2A secara keseluruhan penggunaan sensor dht22/11 dapat berjalan dengan baik, dimana sensor dapat membaca suhu dan kelembapan yang dapat memicu relay apabila melewati ambang batas yang telah ditentukan, meskipun terdapat beberapa kendala ketika proses percobaan dilakukan mulai dari mikrokontroler yang tidak dapat berfungsi, keterbatasan komponen seperti sensor dht22 dan relay yang jumlahnya terbatas. Tetapi dari semua kendala tersebut percobaan tetap berjalan dengan baik.  
### Scematik
[Scematik 1A]("https://github.com/user-attachments/assets/fb025a07-de43-47d2-b55a-627c674b6742" /)   
[Scematik 2A]("https://github.com/user-attachments/assets/29019c69-0cd8-4cf9-b98d-1bf98c4e1eea" /)   

### Dokumentasi   

[Proses perakitan]   
("https://github.com/user-attachments/assets/708d8a5b-0adc-4e38-aad8-c739b3bb3818" /)   
("https://github.com/user-attachments/assets/b62290cd-cfec-48e5-b7cf-4d2c11c5f433" /)   
("https://github.com/user-attachments/assets/18cb176a-1f86-471c-8014-a9ca8f475f70" /)   
[Hasil pengujian]   
("https://github.com/user-attachments/assets/806d2940-2bec-4c1f-8cc0-098a66eb334b" /)   





