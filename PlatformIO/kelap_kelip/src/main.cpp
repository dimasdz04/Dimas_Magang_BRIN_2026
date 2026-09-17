#include <Arduino.h>

// --- Deklarasi Pin ---
const int ledPin1 = 8; // Hubungkan LED 1 ke pin 8
const int ledPin2 = 9; // Hubungkan LED 2 ke pin 9

// --- Variabel Penyimpan Status ---
int ledState1 = LOW;
int ledState2 = LOW;
int counter = 0; // Variabel untuk angka yang bertambah

// --- Variabel Penyimpan Waktu Terakhir (Stopwatch) ---
unsigned long previousMillisLed1 = 0;
unsigned long previousMillisLed2 = 0;
unsigned long previousMillisSerial = 0;

// --- Pengaturan Interval Kecepatan (dalam milidetik) ---
const long intervalLed1 = 500;   // LED 1 berkedip setiap 0.5 detik
const long intervalLed2 = 850;   // LED 2 berkedip setiap 0.85 detik
const long intervalSerial = 1000; // Serial bertambah setiap 1 detik

void setup() {
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  
  // Memulai komunikasi serial dengan baud rate 9600
  Serial.begin(9600);
}

void loop() {
  // Catat waktu saat ini
  unsigned long currentMillis = millis();

  // ---------------------------------------------------------
  // TUGAS 1: Mengontrol LED 1
  // ---------------------------------------------------------
  if (currentMillis - previousMillisLed1 >= intervalLed1) {
    previousMillisLed1 = currentMillis; // Reset stopwatch LED 1
    
    // Ubah state dari ON ke OFF, atau sebaliknya
    if (ledState1 == LOW) {
      ledState1 = HIGH;
    } else {
      ledState1 = LOW;
    }
    digitalWrite(ledPin1, ledState1);
  }

  // ---------------------------------------------------------
  // TUGAS 2: Mengontrol LED 2
  // ---------------------------------------------------------
  if (currentMillis - previousMillisLed2 >= intervalLed2) {
    previousMillisLed2 = currentMillis; // Reset stopwatch LED 2
    
    if (ledState2 == LOW) {
      ledState2 = HIGH;
    } else {
      ledState2 = LOW;
    }
    digitalWrite(ledPin2, ledState2);
  }

  // ---------------------------------------------------------
  // TUGAS 3: Mengirim Angka ke Serial Monitor
  // ---------------------------------------------------------
  if (currentMillis - previousMillisSerial >= intervalSerial) {
    previousMillisSerial = currentMillis; // Reset stopwatch Serial
    
    counter++; // Tambah angka
    Serial.print("Angka saat ini: ");
    Serial.println(counter);
  }
}