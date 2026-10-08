#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>

// ==============================================================================
// KONSTANTA & KONFIGURASI (Bebas Magic Number)
// ==============================================================================

// Konfigurasi Pin LoRa (Merujuk pada skematik Versi 2)
const int PIN_LORA_CS    = 7;  // Pin LORA_CS
const int PIN_LORA_RST   = -1; // Tidak terhubung ke pin digital, abaikan (-1)
const int PIN_LORA_DIO0  = 8;  // Pin Interupsi (IRQ)

// Konfigurasi Komunikasi
const long SERIAL_BAUD_RATE = 9600;
const long LORA_FREQUENCY   = 433E6; // Frekuensi modul LoRa: 433 MHz

// Konfigurasi Waktu (dalam milidetik) untuk Non-Blocking millis()
const unsigned long INTERVAL_RETRY_MS  = 3000; // Coba ulang setiap 3 detik jika gagal
const unsigned long INTERVAL_STATUS_MS = 5000; // Cetak status setiap 5 detik jika standby

// ==============================================================================
// VARIABEL GLOBAL
// ==============================================================================
bool isLoraReady = false;          
unsigned long previousMillis = 0;  

// ==============================================================================
// SETUP 
// ==============================================================================
void setup() {
  Serial.begin(SERIAL_BAUD_RATE);

  Serial.println("--- Memulai Tes Inisialisasi LoRa (Mode Non-Blocking) ---");

  // Alokasikan pin ke pustaka LoRa
  LoRa.setPins(PIN_LORA_CS, PIN_LORA_RST, PIN_LORA_DIO0);

  // Percobaan inisialisasi pertama
  isLoraReady = LoRa.begin(LORA_FREQUENCY);

  if (isLoraReady) {
    Serial.println("SUKSES: LoRa merespons pada percobaan pertama!");
  } else {
    Serial.println("GAGAL: LoRa tidak terdeteksi. Sistem akan mencoba ulang berkala...");
  }
}

// ==============================================================================
// LOOP UTAMA 
// ==============================================================================
void loop() {
  unsigned long currentMillis = millis();

  // SKENARIO A: Jika LoRa belum siap, lakukan percobaan ulang secara berkala
  if (!isLoraReady) {
    if (currentMillis - previousMillis >= INTERVAL_RETRY_MS) {
      previousMillis = currentMillis; 
      
      Serial.println("[Retry] Mencoba menghubungkan ulang ke modul LoRa...");
      isLoraReady = LoRa.begin(LORA_FREQUENCY);
      
      if (isLoraReady) {
        Serial.println("SUKSES: LoRa akhirnya merespons! Hardware aman.");
      } else {
        Serial.println("GAGAL: Masih tidak terdeteksi. Cek tegangan 3.3V dan jalur kabel SPI.");
      }
    }
  } 
  // SKENARIO B: Jika LoRa sudah siap, jalankan operasi normal
  else {
    if (currentMillis - previousMillis >= INTERVAL_STATUS_MS) {
      previousMillis = currentMillis; 
      
      Serial.println("Status: LoRa aktif dan standby mendengarkan...");
    }
  }
}