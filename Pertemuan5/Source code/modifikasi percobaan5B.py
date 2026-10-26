import time
import json
import math
import random
import paho.mqtt.client as mqtt
import pandas as pd
import matplotlib.pyplot as plt

BROKER = "broker.hivemq.com"
PORT = 1883
TOPIC = "unsoed/tk245004/kelompokAnda/data"  
JUMLAH_SAMPEL = 20  

WINDOW_MA = 5

client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
client.connect(BROKER, PORT)
client.loop_start()

data = []
print("Memulai pengiriman data dummy ke MQTT dan pengumpulan data...")

for i in range(1, JUMLAH_SAMPEL + 1):
    suhu_dasar = 28.0 + 2.0 * math.sin(i * 0.08)
    noise = random.randint(-5, 5) * 0.1
    suhu = round(suhu_dasar + noise, 2)

    if i % 25 == 0:
        suhu += 8.0  # Anomali

    payload = json.dumps({"suhu": suhu})
    client.publish(TOPIC, payload)

    waktu_str = time.strftime("%Y-%m-%d %H:%M:%S")
    data.append({"waktu": waktu_str, "suhu": suhu})
    print(f"[{i}/{JUMLAH_SAMPEL}] Mengirim suhu = {suhu} °C")

    time.sleep(1)  

client.loop_stop()
client.disconnect()

df = pd.DataFrame(data)
status_csv = "Berhasil"
df.to_csv("data_suhu.csv", index=False)
print("\nData berhasil disimpan di data_suhu.csv")

df["suhu_ma"] = df["suhu"].rolling(window=WINDOW_MA, min_periods=1).mean()

df["suhu_ma"] = df["suhu_ma"].round(2)

print(f"\nMoving Average (jendela {WINDOW_MA}) diterapkan → kolom 'suhu_ma'")


rata2 = df["suhu_ma"].mean() 
std   = df["suhu_ma"].std()  

if std == 0 or pd.isna(std):
    df["zscore"] = 0.0
else:
    df["zscore"] = (df["suhu_ma"] - rata2) / std

df["anomali"] = df["zscore"].abs() > 2

df["status_anomali"] = df["anomali"].apply(lambda x: "Anomali" if x else "Normal")
df["status_csv"] = status_csv

print("\n" + "=" * 95)
print(f"{'No':<4} | {'Waktu Penerimaan':<19} | {'Suhu (°C)':<9} | {'Suhu MA':>7} | {'Z-score':<8} | {'Status Anomali':<14} | {'Status CSV'}")
print("=" * 95)

for idx, row in df.iterrows():
    no = idx + 1
    print(
        f"{no:<4} | {row['waktu']:<19} | "
        f"{row['suhu']:<9.2f} | "
        f"{row['suhu_ma']:>5.2f} °C | "  # modifikasi tampilkan kolom suhu_ma
        f"{row['zscore']:<8.2f} | "
        f"{row['status_anomali']:<14} | "
        f"{row['status_csv']}"
    )

print("=" * 95)

print(f"\nStatistik (dari data Moving Average, jendela {WINDOW_MA}):")
print(f"  Rata-rata  : {rata2:.2f} °C")
print(f"  Std deviasi: {std:.2f}")
print(f"  Minimum    : {df['suhu_ma'].min():.2f} °C")
print(f"  Maksimum   : {df['suhu_ma'].max():.2f} °C")
print(f"  Jumlah anomali (|z-score| > 2): {df['anomali'].sum()}")

rata2_mentah = df["suhu"].mean()
std_mentah   = df["suhu"].std()
if std_mentah == 0 or pd.isna(std_mentah):
    df["zscore_mentah"] = 0.0
else:
    df["zscore_mentah"] = (df["suhu"] - rata2_mentah) / std_mentah
df["anomali_mentah"] = df["zscore_mentah"].abs() > 2

print(f"\nPerbandingan deteksi anomali:")
print(f"  Data MENTAH         : {df['anomali_mentah'].sum()} anomali")
print(f"  Data MOVING AVERAGE : {df['anomali'].sum()} anomali")

# modifikasi Garis suhu setelah Moving Average
plt.figure(figsize=(12, 5))

plt.plot(
    df.index + 1, df["suhu"],
    marker="o", markersize=4, linewidth=1,
    alpha=0.4, color="steelblue",
    label="Suhu Mentah"
)

# modifikasiGaris suhu setelah Moving Average
plt.plot(
    df.index + 1, df["suhu_ma"],
    marker="s", markersize=4, linewidth=2,
    color="green",
    label=f"Moving Average (window={WINDOW_MA})"
)

anom = df[df["anomali"]]
if not anom.empty:
    plt.scatter(
        anom.index + 1, anom["suhu_ma"],
        color="red", s=120, zorder=5,
        label="Anomali (dari MA)"
    )

plt.axhline(rata2, color="gray", linestyle="--", linewidth=1.5,
            label=f"Rata-rata MA = {rata2:.2f} °C")

plt.xlabel("Urutan data")
plt.ylabel("Suhu (°C)")
plt.title(f"Data Suhu Dummy + Moving Average (window={WINDOW_MA}) + Deteksi Anomali (Z-score)")
plt.legend()
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig("grafik_suhu.png", dpi=150)
print("\nGrafik berhasil disimpan sebagai grafik_suhu.png")
plt.show()