#include <Arduino.h>

const int pinAntena = A1;
// 2 ms = 500 sampel per detik (Sangat cepat, memenuhi syarat Nyquist untuk 50 Hz)
const int intervalSampling = 18; 
unsigned long timerSampling = 0;

void setup() {
  Serial.begin(115200); // Naikkan baudrate agar kuat mengirim data cepat
}

void loop() {
  if (millis() - timerSampling >= intervalSampling) {
    int sinyalTubuh = analogRead(pinAntena);
    
    Serial.print("Gelombang_PLN:");
    Serial.println(sinyalTubuh);
    
    timerSampling = millis();
  }
}