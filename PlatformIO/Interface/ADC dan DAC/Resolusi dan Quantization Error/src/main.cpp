#include <Arduino.h>

const int pinPotensio = A0;
const int pinPWM = 9;

unsigned long timerSerial = 0;
const long intervalSerial = 500; // Update layar tiap setengah detik agar mudah dibaca

void setup() {
  Serial.begin(9600);
  pinMode(pinPWM, OUTPUT);
}

void loop() {
  // 1. Baca Analog Input (ADC 10-bit: 0 - 1023)
  int nilaiADC = analogRead(pinPotensio);
  
  // 2. Konversi ke Skala PWM 8-bit (0 - 255)
  // Bisa pakai map(nilaiADC, 0, 1023, 0, 255) atau bagi 4 secara langsung
  int nilaiPWM = nilaiADC / 4; 
  
  // 3. Keluarkan Sinyal PWM
  analogWrite(pinPWM, nilaiPWM);

  // 4. Observasi Serial Monitor (Non-blocking)
  if (millis() - timerSerial >= intervalSerial) {
    float estimasiTegangan = (nilaiPWM / 255.0) * 5.0; // Perkiraan tegangan matematis
    
    Serial.print("ADC: "); Serial.print(nilaiADC);
    Serial.print(" | PWM: "); Serial.print(nilaiPWM);
    Serial.print(" | Estimasi Voltase Vout: "); Serial.print(estimasiTegangan, 2);
    Serial.println(" V");
    
    timerSerial = millis();
  }
}