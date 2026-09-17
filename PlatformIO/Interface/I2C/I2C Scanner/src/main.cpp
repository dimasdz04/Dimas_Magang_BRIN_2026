#include <Arduino.h>
#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("\nMemulai pencarian alamat I2C...");
}

void loop() {
  byte error, address;
  int nDevices = 0;

  Serial.println("Mencari...");

  // I2C memiliki rentang alamat dari 1 hingga 127
  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("Modul I2C ditemukan di alamat: 0x");
      if (address < 16) 
        Serial.print("0");
      
      // Print alamat dalam format Hexadecimal
      Serial.println(address, HEX); 
      nDevices++;
    }  
  }
  
  if (nDevices == 0) {
    Serial.println("Tidak ada perangkat I2C yang terdeteksi.");
  } else {
    Serial.println("Pencarian selesai.\n");
  }

  // Jeda 5 detik sebelum mencari lagi
  delay(5000); 
}