#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp; // Objek untuk GY-91 (BMP280)

unsigned long timerSensor = 0;
const long intervalBaca = 1000; 

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // --- EKSPERIMEN KECEPATAN ---
  // Coba mulai dari 100000 (100 kHz), lalu naikkan ke 400000 (400 kHz), 
  // lalu coba angka ekstrem seperti 800000 (800 kHz) atau 1000000 (1 MHz)!
  Wire.setClock(100000); 

  // Memulai sensor di alamat 0x76
  if (bmp.begin(0x76)) {
    Serial.println("Sensor GY-91 (BMP280) Siap. Memulai pengujian kecepatan...");
  } else {
    Serial.println("GY-91 tidak terdeteksi! Cek kabel.");
  }
}

void loop() {
  if (millis() - timerSensor >= intervalBaca) {
    
    // 1. Stopwatch Start
    unsigned long waktuMulai = micros();
    
    // 2. Tarik dua data sekaligus (Suhu & Tekanan)
    float suhu = bmp.readTemperature();
    float tekanan = bmp.readPressure() / 100.0F;
    
    // 3. Stopwatch Stop
    unsigned long waktuSelesai = micros();
    
    // 4. Hitung durasi
    unsigned long durasi = waktuSelesai - waktuMulai;
    
    // Tampilkan hasil
    Serial.print("Suhu: "); Serial.print(suhu, 1); Serial.print(" C  |  ");
    Serial.print("Tekanan: "); Serial.print(tekanan, 1); Serial.print(" hPa  |  ");
    
    Serial.print("Waktu I2C: ");
    Serial.print(durasi);
    Serial.println(" mikrodetik");
    
    timerSensor = millis();
  }
}