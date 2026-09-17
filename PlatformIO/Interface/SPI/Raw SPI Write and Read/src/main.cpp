#include <Arduino.h>
#include <SPI.h>

const int CS_PIN = 10;

// Kumpulan Perintah (Instruction Set) Resmi W25Qxx
const byte CMD_WRITE_ENABLE = 0x06;
const byte CMD_SECTOR_ERASE = 0x20; // Hapus 4 Kilobyte data
const byte CMD_PAGE_PROGRAM = 0x02; // Tulis data
const byte CMD_READ_DATA    = 0x03; // Baca data
const byte CMD_READ_STATUS  = 0x05; // Cek status apakah cip sedang sibuk

// Fungsi untuk mengecek apakah cip sedang sibuk (proses hapus memakan waktu)
bool isBusy() {
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(CMD_READ_STATUS);
  byte status = SPI.transfer(0x00);
  digitalWrite(CS_PIN, HIGH);
  
  // Memeriksa Bit ke-0 (WIP - Write In Progress)
  return (status & 1); 
}

void setup() {
  Serial.begin(9600);
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);
  SPI.begin();
  
  delay(2000); // Tunggu Serial Monitor siap
  Serial.println("\n--- Eksperimen Tulis/Baca W25Q32 ---");

  // Kita akan menggunakan alamat memori pertama (Alamat 0)
  // W25Q32 menggunakan alamat 24-bit (3 byte: 0x00, 0x00, 0x00)
  byte addr2 = 0x00, addr1 = 0x00, addr0 = 0x00;

  // ==========================================
  // 1. MENGHAPUS SEKTOR (Wajib sebelum menulis)
  // ==========================================
  Serial.println("1. Menghapus Sektor 0...");
  
  // Buka gembok (Write Enable)
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(CMD_WRITE_ENABLE);
  digitalWrite(CS_PIN, HIGH);
  
  // Kirim Perintah Hapus Sektor 0
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(CMD_SECTOR_ERASE);
  SPI.transfer(addr2);
  SPI.transfer(addr1);
  SPI.transfer(addr0);
  digitalWrite(CS_PIN, HIGH);

  // CPU Arduino harus menunggu sampai cip selesai menghapus secara fisik
  while(isBusy()) { 
    delay(10); 
  }
  Serial.println("   Sektor berhasil dihapus.");

  // ==========================================
  // 2. MENULIS DATA (Page Program)
  // ==========================================
  Serial.println("2. Menulis kata 'SPI' ke memori...");
  
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(CMD_WRITE_ENABLE);
  digitalWrite(CS_PIN, HIGH);
  
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(CMD_PAGE_PROGRAM);
  SPI.transfer(addr2);
  SPI.transfer(addr1);
  SPI.transfer(addr0);
  
  // Data yang ditulis (3 Byte/Huruf)
  SPI.transfer('S');
  SPI.transfer('P');
  SPI.transfer('I');
  digitalWrite(CS_PIN, HIGH);
  
  while(isBusy()) { delay(10); } // Tunggu proses simpan selesai
  Serial.println("   Penulisan selesai.");

  // ==========================================
  // 3. MEMBACA DATA
  // ==========================================
  Serial.println("3. Membaca kembali data dari memori:");
  
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(CMD_READ_DATA);
  SPI.transfer(addr2);
  SPI.transfer(addr1);
  SPI.transfer(addr0);
  
  // Tarik 3 byte data (kirim byte kosong untuk memancing data keluar)
  char char1 = SPI.transfer(0x00);
  char char2 = SPI.transfer(0x00);
  char char3 = SPI.transfer(0x00);
  digitalWrite(CS_PIN, HIGH);

  Serial.print("   Hasil Baca dari Cip: ");
  Serial.print(char1);
  Serial.print(char2);
  Serial.println(char3);
  Serial.println("------------------------------------");
}

void loop() {
  // Kosong, dibiarkan agar tidak terus-menerus menulis ke cip (Flash memiliki batas umur hapus/tulis).
}