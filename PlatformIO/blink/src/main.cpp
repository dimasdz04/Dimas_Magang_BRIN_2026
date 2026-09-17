#include <Arduino.h>
#include <SPI.h>

const int CS_PIN = 10;
unsigned long timerSPI = 0;

void setup() {
  Serial.begin(9600);
  
  // Mengatur pin CS sebagai Output
  pinMode(CS_PIN, OUTPUT);
  // STANDBY MODE: Pin CS harus HIGH saat SPI sedang tidak digunakan
  digitalWrite(CS_PIN, HIGH);

  // Memulai jalur komunikasi SPI
  SPI.begin();
  
  Serial.println("Memulai Eksperimen SPI W25Qxx...");
  Serial.println("Mencari JEDEC ID...");
  Serial.println("----------------------------------");
}

void loop() {
  if (millis() - timerSPI >= 3000) { // Baca setiap 3 detik
    
    // 1. AKTIFKAN KOMUNIKASI (Tarik CS ke LOW)
    // Berbeda dengan I2C yang kirim alamat, SPI cukup menarik pin CS menjadi 0V
    digitalWrite(CS_PIN, LOW);
    
    // 2. KIRIM PERINTAH (Command 0x9F = Read JEDEC ID)
    SPI.transfer(0x9F);
    
    // 3. TERIMA BALASAN (Kirim byte kosong/0x00 untuk memancing data keluar)
    byte manufacturerID = SPI.transfer(0x00); 
    byte memoryType     = SPI.transfer(0x00);
    byte capacity       = SPI.transfer(0x00);
    
    // 4. TUTUP KOMUNIKASI (Tarik CS kembali ke HIGH)
    digitalWrite(CS_PIN, HIGH);
    
    // 5. TAMPILKAN HASILNYA
    Serial.print("Pabrikan (Manufacturer ID) : 0x");
    Serial.println(manufacturerID, HEX);
    
    Serial.print("Tipe Memori                : 0x");
    Serial.println(memoryType, HEX);
    
    Serial.print("Kapasitas (Capacity)       : 0x");
    Serial.println(capacity, HEX);
    Serial.println("----------------------------------");
    
    timerSPI = millis();
  }
}