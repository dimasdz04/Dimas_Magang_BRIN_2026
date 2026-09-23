#include <Arduino.h>

const int pinPWM = 9;

void setup() {
  pinMode(pinPWM, OUTPUT);
  
  // Memerintahkan output 50% (Sintesis ~2.5V)
  analogWrite(pinPWM, 200); 
}

void loop() {
  // Biarkan kosong, PWM berjalan otomatis di latar belakang (Hardware Timer)
}