#include <DHT.h>
#include <math.h>

#define DHTPIN 4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

const int N = 5;  
float buffer[N]; 
int indeks = 0;  
int jumlah = 0;  

float suhuMin = 1000.0;
float suhuMax = -1000.0;


float bacaSuhuMock() {
  static unsigned long jumlahBaca = 0;
  jumlahBaca++;

  // Suhu dasar berubah secara bertahap
  float suhuDasar = 28.0 + 2.0 * sin(jumlahBaca * 0.08);

  // Noise kecil untuk mensimulasikan fluktuasi sensor
  float noise = random(-5, 6) * 0.1;

  float suhu = suhuDasar + noise;

  // Lonjakan suhu setiap 25 pembacaan
  if (jumlahBaca % 25 == 0) {
    suhu += 8.0;
  }

  return suhu;
}

// Fungsi moving average
float hitungMovingAverage(float nilaiBaru) {
  buffer[indeks] = nilaiBaru;
  indeks = (indeks + 1) % N;

  if (jumlah < N) {
    jumlah++;
  }

  float total = 0;

  for (int i = 0; i < jumlah; i++) {
    total += buffer[i];
  }

  return total / jumlah;
}

void setup() {
  Serial.begin(115200);

  dht.begin();
  randomSeed(analogRead(A0));

  Serial.println("Simulasi Pembacaan Suhu");
  Serial.println("========================");
}

void loop() {
  // Menggunakan data suhu simulasi
  float suhu = bacaSuhuMock();

  if (isnan(suhu)) {
    Serial.println("Pembacaan gagal, data diabaikan");
  } else {
    float suhuFilter = hitungMovingAverage(suhu);

    if (suhu < suhuMin) {
      suhuMin = suhu;
    }

    if (suhu > suhuMax) {
      suhuMax = suhu;
    }

    Serial.print("Mentah: ");
    Serial.print(suhu, 2);
    Serial.print(" °C | Filter: ");
    Serial.print(suhuFilter, 2);
    Serial.print(" °C | Min: ");
    Serial.print(suhuMin, 2);
    Serial.print(" °C | Max: ");
    Serial.print(suhuMax, 2);
    Serial.println(" °C");
  }

  delay(2000);
}