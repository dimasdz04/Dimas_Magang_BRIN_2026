#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <BH1750.h>
#include <Adafruit_BMP280.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
BH1750 sensorCahaya1;
BH1750 sensorCahaya2;
Adafruit_BMP280 bmp;

// --- DUA TIMER INDEPENDEN ---
unsigned long timerLayar = 0;   // Timer untuk ganti halaman (3 detik)
unsigned long timerSensor = 0;  // Timer untuk baca sensor & update LCD (200 ms)

int halamanLayar = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Memulai Sistem..");

  sensorCahaya1.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x23);
  sensorCahaya2.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x5C);
  bmp.begin(0x76); 

  delay(2000); // Delay di setup() tidak masalah karena hanya jalan sekali
  lcd.clear();
}

void loop() {
  // 1. LOGIKA GANTI HALAMAN (3 Detik) - NON-BLOCKING
  if (millis() - timerLayar > 3000) {
    halamanLayar++;
    if (halamanLayar > 1) {
      halamanLayar = 0;
    }
    timerLayar = millis();
    lcd.clear(); // Bersihkan layar saat ganti halaman
  }

  // 2. LOGIKA BACA SENSOR & UPDATE TAMPILAN (200 ms) - NON-BLOCKING
  if (millis() - timerSensor > 200) {
    
    // Tarik data dari I2C Bus hanya setiap 200ms
    float lux1 = sensorCahaya1.readLightLevel();
    float lux2 = sensorCahaya2.readLightLevel();
    float suhu = bmp.readTemperature();
    float tekanan = bmp.readPressure() / 100.0F;

    // Tulis ke LCD berdasarkan halaman aktif
    if (halamanLayar == 0) {
      lcd.setCursor(0, 0);
      lcd.print("CHY1: "); lcd.print(lux1, 1); lcd.print(" lx");
      
      lcd.setCursor(0, 1);
      lcd.print("CHY2: "); lcd.print(lux2, 1); lcd.print(" lx");
    } 
    else if (halamanLayar == 1) {
      lcd.setCursor(0, 0);
      lcd.print("Suhu: "); lcd.print(suhu, 1); lcd.print(" C");
      
      lcd.setCursor(0, 1);
      lcd.print("Pres: "); lcd.print(tekanan, 1); lcd.print(" hPa");
    }

    timerSensor = millis(); // Reset timer pembacaan sensor
  }
}