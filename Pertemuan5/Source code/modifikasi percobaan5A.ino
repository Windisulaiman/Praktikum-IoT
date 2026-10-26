#include <DHT.h>
#include <math.h>  

#define DHTPIN  4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

const int N  = 5;      
float buffer[N];      
int   indeks = 0;       
int   jumlah = 0;       

float suhuMin =  1000.0;
float suhuMax = -1000.0;

float bacaSuhuMock() {
  static unsigned long jumlahBaca = 0;
  jumlahBaca++;

  float suhuDasar = 28.0 + 2.0 * sin(jumlahBaca * 0.08);

  float noise = random(-5, 6) * 0.1;

  float suhu = suhuDasar + noise;

  if (jumlahBaca % 25 == 0) {
    suhu += 8.0;
  }

  return suhu;
}


float hitungMovingAverage(float nilaiBaru) {
  buffer[indeks] = nilaiBaru;           
  indeks = (indeks + 1) % N;           
  if (jumlah < N) jumlah++;            

  float total = 0;
  for (int i = 0; i < jumlah; i++) {
    total += buffer[i];                
  }
  return total / jumlah;               
}


float hitungStdDev() {
  if (jumlah < 2) return 0.0;

  float total = 0;
  for (int i = 0; i < jumlah; i++) {
    total += buffer[i];
  }
  float rata2 = total / jumlah;        

  float jumlahKuadrat = 0;
  for (int i = 0; i < jumlah; i++) {
    float selisih = buffer[i] - rata2;           
    jumlahKuadrat += selisih * selisih;         
  }

  return sqrt(jumlahKuadrat / jumlah);        
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  randomSeed(analogRead(A0));          

  Serial.println("Simulasi Pembacaan Suhu + Simpangan Baku");
  Serial.println("==========================================");
}

void loop() {
  
  float suhu = bacaSuhuMock();

  if (isnan(suhu)) {
    Serial.println("Pembacaan gagal, data diabaikan");
  } else {
    
    float suhuFilter = hitungMovingAverage(suhu);  /
    float stdDev     = hitungStdDev();             //  hitung std dev buffer

    if (suhu < suhuMin) suhuMin = suhu;            
    if (suhu > suhuMax) suhuMax = suhu;           

    // Tampilkan semua nilai ke Serial Monitor
    Serial.print("Mentah: ");
    Serial.print(suhu, 2);
    Serial.print(" °C | Filter: ");
    Serial.print(suhuFilter, 2);
    Serial.print(" °C | StdDev: ");
    Serial.print(stdDev, 4);           //tampilkan simpangan baku buffer
    Serial.print(" | Min: ");
    Serial.print(suhuMin, 2);
    Serial.print(" °C | Max: ");
    Serial.print(suhuMax, 2);
    Serial.println(" °C");
  }

  delay(2000);                         
}
