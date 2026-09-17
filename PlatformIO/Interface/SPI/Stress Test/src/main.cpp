#include <Arduino.h>
#include <SPI.h>

const int CS_PIN = 10;
const byte CMD_READ_DATA = 0x03;

// --- VARIABEL NON-BLOCKING ---
unsigned long timerUji = 0;
const long intervalUji = 3000; // Lakukan pengujian setiap 3 detik
int tahapUji = 0;              // 0 = 1 MHz, 1 = 8 MHz

// --- FUNCTION PROTOTYPE (Wajib di C++ / PlatformIO) ---
void ujiKecepatan(long speed);

void setup() {
  Serial.begin(9600);
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);
  SPI.begin();

  Serial.println("\n--- Percobaan 4: Uji Kecepatan Ekstrem SPI (Non-Blocking) ---");
}

void loop() {
  // Mengeksekusi pengujian tanpa menghentikan CPU
  if (millis() - timerUji >= intervalUji) {
    
    if (tahapUji == 0) {
      ujiKecepatan(1000000); // Eksekusi 1 MHz
      tahapUji = 1;          
    } 
    else if (tahapUji == 1) {
      ujiKecepatan(8000000); // Eksekusi 8 MHz
      Serial.println("=========================================\n");
      tahapUji = 0;          
    }
    
    timerUji = millis();     // Reset timer
  }
}

void ujiKecepatan(long speed) {
  Serial.print("Menguji kecepatan : ");
  Serial.print(speed / 1000000);
  Serial.println(" MHz");

  SPI.beginTransaction(SPISettings(speed, MSBFIRST, SPI_MODE0));

  unsigned long waktuMulai = micros();

  digitalWrite(CS_PIN, LOW);
  SPI.transfer(CMD_READ_DATA);
  SPI.transfer(0x00); 
  SPI.transfer(0x00); 
  SPI.transfer(0x00); 

  // Menarik 1000 Byte beruntun
  for (int i = 0; i < 1000; i++) {
    SPI.transfer(0x00); 
  }

  digitalWrite(CS_PIN, HIGH);
  SPI.endTransaction();

  unsigned long waktuSelesai = micros();
  unsigned long durasi = waktuSelesai - waktuMulai;

  Serial.print("Waktu tarik 1000 Byte : ");
  Serial.print(durasi);
  Serial.println(" mikrodetik\n");
}