#include <Arduino.h>
#include <Wire.h>
#include <BH1750.h> 

BH1750 lightMeter;

unsigned long timerSensor = 0;
const long intervalBaca = 1000; 

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // --- UBAH ANGKA INI UNTUK EKSPERIMEN ---
  // Coba 100000 dulu, perhatikan durasinya di Serial Monitor.
  // Lalu ganti ke 400000, upload lagi, dan bandingkan durasinya!
  Wire.setClock(100000); 

  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x23)) {
    Serial.println("Sensor BH1750 Siap. Memulai pengujian kecepatan...");
  } else {
    Serial.println("Sensor tidak terdeteksi!");
  }
}

void loop() {
  if (millis() - timerSensor >= intervalBaca) {
    
    // 1. Catat waktu sebelum mulai komunikasi I2C (Stopwatch Start)
    unsigned long waktuMulai = micros();
    
    // 2. Lakukan komunikasi I2C (Baca data)
    float lux = lightMeter.readLightLevel();
    
    // 3. Catat waktu setelah komunikasi selesai (Stopwatch Stop)
    unsigned long waktuSelesai = micros();
    
    // 4. Hitung durasi (selisih waktu)
    unsigned long durasi = waktuSelesai - waktuMulai;
    
    // Menampilkan hasil ke Serial Monitor
    Serial.print("Lux: ");
    Serial.print(lux);
    Serial.print("  |  Waktu Komunikasi I2C: ");
    Serial.print(durasi);
    Serial.println(" mikrodetik");
    
    timerSensor = millis();
  }
}