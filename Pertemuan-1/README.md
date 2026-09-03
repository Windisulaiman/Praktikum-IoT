## Pertemuan 1   
___  
### Identitas  
Nama: Windi Sulaiman Ismansa   
NIM: H1H024005
Shift : A   
___


Modifikasi program pertanyaan praktikum soal no 4
```ino
#include <DHT.h>

#define DHTPIN 4       // Pin data DHT22 terhubung ke GPIO 4
#define DHTTYPE DHT11  // Tipe sensor DHT22
#define RELAYPIN 5     // Pin kendali relay/LED indikator terhubung ke GPIO 5

DHT dht(DHTPIN, DHTTYPE);

// Dua ambang batas (Histeresis)
const float suhuAtas = 30.0;   // Ambang batas atas (°C) - Aktuator ON
const float suhuBawah = 28.0;  // Ambang batas bawah (°C) - Aktuator OFF

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(RELAYPIN, OUTPUT);
  digitalWrite(RELAYPIN, LOW); // Pastikan aktuator mati di awal
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
      digitalWrite(RELAYPIN, HIGH); // Aktifkan relay/LED jika suhu > 30°C
      Serial.println("Status Aktuator: ON (Suhu > 30°C)");
    } else if (suhu < suhuBawah) {
      digitalWrite(RELAYPIN, LOW);  // Matikan relay/LED jika suhu < 28°C
      Serial.println("Status Aktuator: OFF (Suhu < 28°C)");
    } else {
      // Jika suhu di antara 28°C dan 30°C, pertahankan status sebelumnya
      Serial.println("Status Aktuator: Mempertahankan kondisi sebelumnya ");
    }
  }

  delay(2000); // Jeda pembacaan sensor (2 detik)
}
```
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





