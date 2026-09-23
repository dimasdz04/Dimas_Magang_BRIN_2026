#include <Arduino.h>

const int pinLM35 = A0;
const int jumlahSampel = 20; // Kita gunakan 20 sampel historis
int indexSampel = 0;
float totalSuhu = 0;
float arraySuhu[jumlahSampel]; // Array untuk menyimpan 20 data terakhir

void setup() {
  Serial.begin(9600);
  
  // Mengisi array awal dengan nilai 0 agar tidak ada data acak (garbage memory)
  for (int i = 0; i < jumlahSampel; i++) {
    arraySuhu[i] = 0;
  }
}

void loop() {
  // 1. Baca data keras dari ADC
  int nilaiADC = analogRead(pinLM35);
  float tegangan = (nilaiADC * 5.0) / 1024.0;
  float suhuKasar = tegangan * 100.0; // Suhu dengan Quantization Error

  // 2. Algoritma Moving Average
  // Kurangi nilai paling lama di dalam array dari total
  totalSuhu = totalSuhu - arraySuhu[indexSampel]; 
  
  // Masukkan nilai suhu yang baru dibaca ke dalam array
  arraySuhu[indexSampel] = suhuKasar; 
  
  // Tambahkan nilai baru tersebut ke dalam total
  totalSuhu = totalSuhu + arraySuhu[indexSampel]; 
  
  // Majukan indeks. Jika sudah di ujung array, kembali ke 0 (Circular)
  indexSampel = indexSampel + 1;
  if (indexSampel >= jumlahSampel) {
    indexSampel = 0; 
  }

  // 3. Kalkulasi hasil akhir
  float suhuHalus = totalSuhu / jumlahSampel;

  // 4. Visualisasikan perbandingannya
  Serial.print("Kasar (Hardware): ");
  Serial.print(suhuKasar, 2);
  Serial.print(" °C | Halus (Software): ");
  Serial.print(suhuHalus, 2);
  Serial.println(" °C");

  delay(50); // Jeda sampling
}