# Modul 5 – Pengolahan dan Analisis Data IoT

## Identitas
Nama: Windi Sulaiman Ismansa
NIM : H1H024005   
Mata Kuliah : Praktikum Sistem Internet of Things / TK24505  
Modul : 5 – Pengolahan dan Visualisasi Data  
Shift : A
---

## Tujuan Praktikum

1. Memahami tahapan pengolahan dan analisis data pada sistem IoT.
2. Memahami perbedaan pengolahan data di sisi perangkat (edge) dan di sisi server/komputer.
3. Mengimplementasikan filter moving average dan perhitungan statistik sederhana pada ESP32/ESP8266.
4. Mengimplementasikan pengambilan data IoT melalui MQTT menggunakan Python dan menyimpannya ke file CSV.
5. Mampu menganalisis dan memvisualisasikan data sensor serta mendeteksi anomali menggunakan metode z-score.

---

## Library yang Digunakan

### Arduino / ESP8266

| Library | Kegunaan |
|---------|----------|
| `DHT.h` | Membaca data suhu dan kelembapan dari sensor DHT22. |
| `math.h` | Menyediakan fungsi matematika seperti `sin()` dan `sqrt()`. |

### Python

| Library | Kegunaan |
|---------|----------|
| `paho.mqtt.client` | Komunikasi MQTT — publish dan subscribe ke broker. |
| `pandas` | Pengolahan data tabular, analisis statistik, dan ekspor CSV. |
| `matplotlib.pyplot` | Membuat grafik dan visualisasi data. |
| `json` | Serialisasi dan deserialisasi data format JSON. |
| `math`, `random`, `time` | Simulasi data suhu, noise acak, dan timestamp. |

---

## Board yang Digunakan

NodeMCU 1.0 (ESP8266 – ESP-12E Module)
---

## Percobaan 5A – Pengolahan Data Sensor pada ESP8266 (Moving Average dan Statistik)

### Tujuan
Memahami dan mengimplementasikan pengolahan data sensor pada sisi perangkat (edge processing) menggunakan filter moving average serta pencatatan nilai minimum dan maksimum.

### Penjelasan Fungsi Source Code

| Kode / Fungsi | Kegunaan |
|---------------|----------|
| `#include <DHT.h>` | Memanggil library untuk membaca data suhu dari sensor DHT22. |
| `#include <math.h>` | Memanggil library operasi matematika, digunakan untuk fungsi `sin()` dan `sqrt()`. |
| `DHT dht(DHTPIN, DHTTYPE)` | Menginisialisasi objek sensor DHT dengan pin data dan tipe sensor. |
| `float buffer[N]` | Array circular buffer untuk menyimpan N pembacaan suhu terakhir. |
| `int indeks` | Penunjuk posisi penulisan berikutnya pada buffer (berputar melingkar). |
| `int jumlah` | Jumlah sampel yang sudah terisi di dalam buffer (0 hingga N). |
| `bacaSuhuMock()` | Fungsi simulasi pembacaan suhu menggunakan gelombang sinus, noise acak, dan lonjakan (spike) setiap 25 pembacaan. |
| `hitungMovingAverage(nilaiBaru)` | Menghitung rata-rata bergerak dari N pembacaan terakhir menggunakan teknik circular buffer. |
| `randomSeed(analogRead(A0))` | Mengatur nilai awal pembangkit angka acak berdasarkan pembacaan analog pin A0 agar noise bersifat acak tiap sesi. |
| `isnan(suhu)` | Memeriksa apakah nilai suhu tidak valid (Not a Number / NaN). |
| `indeks = (indeks + 1) % N` | Mengimplementasikan circular buffer: indeks berputar kembali ke 0 setelah mencapai N-1. |

### Penjelasan Percabangan

**1. Cek validitas pembacaan sensor**
```cpp
if (isnan(suhu)) {
    Serial.println("Pembacaan gagal, data diabaikan");
} else {
    // proses data
}
```
Jika sensor gagal membaca (mengembalikan NaN), data diabaikan dan tidak masuk ke buffer maupun perhitungan statistik.

**2. Perbarui nilai minimum dan maksimum**
```cpp
if (suhu < suhuMin) suhuMin = suhu;
if (suhu > suhuMax) suhuMax = suhu;
```
Setiap pembacaan valid dibandingkan dengan nilai minimum dan maksimum yang tersimpan, lalu diperbarui jika ditemukan nilai yang lebih kecil atau lebih besar.

**3. Kondisi pengisian buffer awal**
```cpp
if (jumlah < N) jumlah++;
```
Pada awal program, buffer belum penuh. Kondisi ini memastikan rata-rata hanya dihitung dari data yang sudah ada, bukan dari slot buffer yang masih bernilai 0.

---

## Percobaan 5B – Akuisisi, Analisis, dan Visualisasi Data

### Tujuan
Memahami dan mengimplementasikan pengolahan data pada sisi komputer, mulai dari pengambilan data melalui MQTT, penyimpanan ke file CSV, analisis statistik, deteksi anomali dengan z-score, hingga visualisasi data.

### Penjelasan Fungsi Source Code

| Kode / Fungsi | Kegunaan |
|---------------|----------|
| `mqtt.Client(CallbackAPIVersion.VERSION2)` | Membuat objek MQTT client menggunakan API versi 2 dari library paho-mqtt. |
| `client.connect(BROKER, PORT)` | Menghubungkan client ke broker MQTT pada alamat dan port yang ditentukan. |
| `client.loop_start()` | Menjalankan loop MQTT di background thread agar publish tidak memblokir program. |
| `client.publish(TOPIC, payload)` | Mempublikasikan data suhu dalam format JSON ke topic MQTT yang ditentukan. |
| `json.dumps({"suhu": suhu})` | Mengonversi dictionary Python menjadi string JSON untuk dikirim sebagai payload MQTT. |
| `math.sin(i * 0.08)` | Menghasilkan tren sinusoidal pada simulasi suhu agar data bervariasi secara periodik. |
| `random.randint(-5, 5) * 0.1` | Menambahkan noise acak ±0.5 °C pada data simulasi untuk meniru fluktuasi sensor nyata. |
| `pd.DataFrame(data)` | Mengonversi list dictionary menjadi DataFrame pandas untuk pengolahan tabular. |
| `df.to_csv("data_suhu.csv")` | Menyimpan seluruh data ke file CSV secara permanen. |
| `df["suhu"].mean()` | Menghitung nilai rata-rata (mean) seluruh data suhu. |
| `df["suhu"].std()` | Menghitung simpangan baku (standard deviation) seluruh data suhu. |
| `(df["suhu"] - rata2) / std` | Menghitung z-score tiap data: jarak terhadap mean dalam satuan simpangan baku. |
| `df["zscore"].abs() > 2` | Mendeteksi anomali: data dengan \|z\| > 2 dianggap menyimpang signifikan dari distribusi normal. |
| `plt.scatter(anom.index, anom["suhu"])` | Menandai titik anomali pada grafik dengan warna merah agar mudah diidentifikasi. |
| `plt.axhline(rata2)` | Menggambar garis horizontal pada nilai rata-rata sebagai referensi visual. |
| `plt.savefig("grafik_suhu.png")` | Menyimpan grafik ke file PNG secara otomatis. |
---

## Pertanyaan Praktikum
---

## Modifikasi Percobaan 5A – Tambah Perhitungan Simpangan Baku

### Deskripsi Modifikasi

Program dimodifikasi dengan menambahkan fungsi `hitungStdDev()` yang menghitung simpangan baku (standard deviation) dari data yang ada di dalam buffer saat ini, serta menampilkan nilainya di Serial Monitor bersama data lainnya.

### Baris Kode yang Ditambahkan

**1. Include library math (sudah ada, digunakan untuk `sqrt()`)**
```cpp
#include <math.h>
```
Diperlukan untuk fungsi `sqrt()` dalam perhitungan akar kuadrat pada rumus simpangan baku.

---

**2. Fungsi `hitungStdDev()` — inti modifikasi**
```cpp
float hitungStdDev() {
```
Deklarasi fungsi baru yang mengembalikan nilai simpangan baku (tipe `float`) dari data yang ada di buffer saat ini.

```cpp
  if (jumlah < 2) return 0.0;
```
Simpangan baku membutuhkan minimal 2 data agar bermakna secara statistik. Jika data masih kurang dari 2, fungsi langsung mengembalikan 0.

```cpp
  float total = 0;
  for (int i = 0; i < jumlah; i++) {
    total += buffer[i];
  }
  float rata2 = total / jumlah;
```
Langkah 1: menghitung rata-rata (mean) dari semua data yang ada di buffer. Ini adalah acuan untuk mengukur penyimpangan tiap data.

```cpp
  float jumlahKuadrat = 0;
  for (int i = 0; i < jumlah; i++) {
    float selisih = buffer[i] - rata2;
    jumlahKuadrat += selisih * selisih;
  }
```
Langkah 2 & 3: untuk setiap data di buffer, dihitung selisihnya terhadap mean, kemudian dikuadratkan dan diakumulasikan. Pengkuadratan menghilangkan tanda negatif dan memberikan bobot lebih pada penyimpangan besar.

```cpp
  return sqrt(jumlahKuadrat / jumlah);
}
```
Langkah 4: membagi jumlah kuadrat selisih dengan banyak data (simpangan baku populasi), lalu diambil akar kuadratnya untuk mengembalikan ke satuan asli (°C).

---

**3. Pemanggilan fungsi di `loop()`**
```cpp
float stdDev = hitungStdDev();
```
Memanggil fungsi `hitungStdDev()` setiap iterasi untuk mendapatkan nilai simpangan baku terkini dari buffer.

```cpp
Serial.print(" °C | StdDev: ");
Serial.print(stdDev, 4);
```
Menampilkan nilai simpangan baku ke Serial Monitor dengan presisi 4 desimal di samping data mentah, filter, min, dan max.

---

## Modifikasi Percobaan 5B – Moving Average Sebelum Z-score

### Deskripsi Modifikasi

Program dimodifikasi agar menerapkan moving average dengan jendela 5 data pada kolom suhu hasil CSV sebelum dihitung z-score-nya, kemudian membandingkan jumlah anomali yang terdeteksi antara data mentah dan data hasil moving average.

### Baris Kode yang Ditambahkan

**1. Konstanta ukuran jendela Moving Average**
```python
WINDOW_MA = 5
```
Mendefinisikan ukuran jendela (window) moving average. Nilai 5 berarti setiap titik data dihitung sebagai rata-rata dari 5 data terakhir. Mudah diubah untuk eksperimen ukuran jendela yang berbeda.

---

**2. Penerapan Moving Average pada DataFrame**
```python
df["suhu_ma"] = df["suhu"].rolling(window=WINDOW_MA, min_periods=1).mean()
```
- `.rolling(window=5)` — membuat "jendela geser" sepanjang 5 baris pada kolom `suhu`.
- `.mean()` — menghitung rata-rata dari data dalam jendela tersebut untuk tiap baris.
- `min_periods=1` — memastikan baris awal yang datanya kurang dari 5 tetap dihitung dari data yang ada (tidak menghasilkan NaN).
- Hasilnya disimpan ke kolom baru `suhu_ma`.

```python
df["suhu_ma"] = df["suhu_ma"].round(2)
```
Membulatkan hasil moving average ke 2 desimal agar konsisten dengan format kolom suhu mentah.

---

**3. Hitung statistik dan z-score dari data MA (bukan data mentah)**
```python
rata2 = df["suhu_ma"].mean()
std   = df["suhu_ma"].std()
```
Rata-rata dan simpangan baku dihitung dari kolom `suhu_ma` (data hasil MA), bukan dari `suhu` mentah. Tujuannya agar z-score yang dihasilkan lebih stabil karena noise sesaat sudah diredam.

```python
df["zscore"] = (df["suhu_ma"] - rata2) / std
```
Z-score dihitung dari kolom `suhu_ma`: seberapa jauh tiap nilai MA menyimpang dari mean MA dalam satuan simpangan baku.

---

**4. Perbandingan jumlah anomali data mentah vs data MA**
```python
rata2_mentah = df["suhu"].mean()
std_mentah   = df["suhu"].std()
df["zscore_mentah"] = (df["suhu"] - rata2_mentah) / std_mentah
df["anomali_mentah"] = df["zscore_mentah"].abs() > 2
```
Menghitung z-score dari data mentah secara terpisah khusus untuk perbandingan, tanpa mengubah hasil deteksi utama yang menggunakan data MA.

```python
print(f"  Data MENTAH         : {df['anomali_mentah'].sum()} anomali")
print(f"  Data MOVING AVERAGE : {df['anomali'].sum()} anomali")
```
Menampilkan perbandingan jumlah anomali yang terdeteksi dari kedua metode, sehingga efek penerapan MA terhadap sensitivitas deteksi dapat dianalisis secara langsung.

---

**5. Grafik dengan dua garis (mentah + MA)**
```python
plt.plot(df.index + 1, df["suhu"], ..., alpha=0.4, color="steelblue", label="Suhu Mentah")
```
Menggambar garis suhu mentah dengan transparansi 40% sebagai referensi di latar belakang.

```python
plt.plot(df.index + 1, df["suhu_ma"], ..., color="green", label=f"Moving Average (window={WINDOW_MA})")
```
Menggambar garis suhu hasil Moving Average dengan warna hijau dan ketebalan lebih tinggi agar mudah dibedakan dari data mentah.

```python
plt.scatter(anom.index + 1, anom["suhu_ma"], color="red", s=120, label="Anomali (dari MA)")
```
Menandai titik anomali yang terdeteksi dari data MA (bukan data mentah) dengan titik merah berukuran lebih besar.

### Perbandingan Hasil Deteksi Anomali

| Metode | Jumlah Anomali Terdeteksi | Keterangan |
|--------|---------------------------|------------|
| Z-score dari data **mentah** | Lebih banyak | Noise sesaat ikut terdeteksi sebagai anomali (false positive lebih tinggi) |
| Z-score dari data **Moving Average** | Lebih sedikit | Hanya lonjakan signifikan yang terdeteksi, lebih akurat |

> **Kesimpulan:** Penerapan Moving Average sebelum z-score meningkatkan akurasi deteksi anomali dengan meredam noise acak, sehingga hanya penyimpangan yang benar-benar signifikan yang terdeteksi sebagai anomali.
---

## Catatan

Pengujian pengolahan data sensor menggunakan filter moving average (Percobaan 5A) dan akuisisi data via MQTT menggunakan Python (Percobaan 5B) berjalan dengan baik. Data mentah yang disimulasikan menggunakan mock function berhasil dihaluskan oleh algoritma moving average, serta deteksi anomali berbasis z-score pada skrip Python mampu mengidentifikasi lonjakan suhu abnormal (z > 2) secara presisi dan menyimpannya ke format CSV. Modifikasi penambahan simpangan baku pada Percobaan 5A memberikan informasi tambahan tentang sebaran data dalam buffer, sedangkan modifikasi moving average sebelum z-score pada Percobaan 5B terbukti mengurangi false positive dalam deteksi anomali.

