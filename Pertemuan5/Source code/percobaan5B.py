import time
import json
import math
import random
import paho.mqtt.client as mqtt
import pandas as pd
import matplotlib.pyplot as plt

# Konfigurasi MQTT
BROKER = "broker.hivemq.com"
PORT = 1883
TOPIC = "unsoed/tk245004/kelompokAnda/data"  # Sesuaikan topik Anda[cite: 5]
JUMLAH_SAMPEL = 20  # Menyesuaikan dengan jumlah baris pada Tabel 5.2 Dapeng

client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
client.connect(BROKER, PORT)
client.loop_start()

data = []
print("Memulai pengiriman data dummy ke MQTT dan pengumpulan data...")

for i in range(1, JUMLAH_SAMPEL + 1):
    # Simulasi suhu menggunakan fungsi matematika & noise
    suhu_dasar = 28.0 + 2.0 * math.sin(i * 0.08)
    noise = random.randint(-5, 5) * 0.1
    suhu = round(suhu_dasar + noise, 2)
    
    if i % 25 == 0:
        suhu += 8.0  # Anomali

    # Format JSON yang dikirim ke MQTT
    payload = json.dumps({"suhu": suhu})
    client.publish(TOPIC, payload)
    
    waktu_str = time.strftime("%Y-%m-%d %H:%M:%S")
    data.append({"waktu": waktu_str, "suhu": suhu})
    print(f"[{i}/{JUMLAH_SAMPEL}] Mengirim suhu = {suhu} °C")
    
    time.sleep(1) # Jeda 1 detik per data

client.loop_stop()
client.disconnect()

# -- Tahap 2: Simpan ke CSV[cite: 7]
df = pd.DataFrame(data)
status_csv = "Berhasil"
df.to_csv("data_suhu.csv", index=False)
print("\nData berhasil disimpan di data_suhu.csv")

# -- Tahap 3: Analisis statistik dan deteksi anomali (z-score)[cite: 2, 7]
rata2 = df["suhu"].mean()
std = df["suhu"].std()

if std == 0 or pd.isna(std):
    df["zscore"] = 0.0
else:
    df["zscore"] = (df["suhu"] - rata2) / std

df["anomali"] = df["zscore"].abs() > 2

# Menambahkan kolom status teks untuk memudahkan pengisian tabel laporan
df["status_anomali"] = df["anomali"].apply(lambda x: "Anomali" if x else "Normal")
df["status_csv"] = status_csv

print("\n" + "="*80)
print(f"{'No':<4} | {'Waktu Penerimaan':<19} | {'Suhu (°C)':<9} | {'Z-score':<8} | {'Status Anomali':<14} | {'Status CSV'}")
print("="*80)

for idx, row in df.iterrows():
    no = idx + 1
    print(f"{no:<4} | {row['waktu']:<19} | {row['suhu']:<9.2f} | {row['zscore']:<8.2f} | {row['status_anomali']:<14} | {row['status_csv']}")

print("="*80)
print(f"\nRata-rata : {rata2:.2f} °C")
print(f"Std deviasi: {std:.2f}")
print(f"Minimum   : {df['suhu'].min():.2f} °C")
print(f"Maksimum  : {df['suhu'].max():.2f} °C")
print(f"Jumlah anomali (z-score > 2): {df['anomali'].sum()}")

# -- Tahap 4: Visualisasi[cite: 7]
plt.figure(figsize=(10, 4))
plt.plot(df.index + 1, df["suhu"], marker="o", label="Suhu")

anom = df[df["anomali"]]
if not anom.empty:
    plt.scatter(anom.index + 1, anom["suhu"], color="red", s=100, label="Anomali", zorder=3)

plt.axhline(rata2, color="gray", linestyle="--", label="Rata-rata")
plt.xlabel("Urutan data")
plt.ylabel("Suhu (°C)")
plt.title("Data Suhu Dummy dan Deteksi Anomali (Z-score)")
plt.legend()
plt.tight_layout()
plt.savefig("grafik_suhu.png")
print("\nGrafik berhasil disimpan sebagai grafik_suhu.png")
plt.show()