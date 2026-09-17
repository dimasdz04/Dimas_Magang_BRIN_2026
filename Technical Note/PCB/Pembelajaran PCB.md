PCB
---

Flow Pembelajaran Kicad [v7 or newer]

Tujuan pembelajaran
- Memahami konsep rangkain dan PCB serta implementasi menggunakan Kicad
- Mampu membuat rangkaian serta PCB menggunakan KiCad (sederhana)
- Mampu mengimplementasi rangkain kompleks yang menggunakan
    - memilih footprint yang sesuai,
    - menggunakan label, bus
    - mengembangan symbol maupun footprint untuk modul yang belum tersedia

0. Konsep dan Istilah dalam Rangkaian dan PCB
   Footprint : though hole, SMD, packaging (TO-3-To-92, SSOTxxx, pitch --> TN
, Label, Bus, Symbol, Pitch, packaging / konektor dlsb
- Memahami istilah PCB - Double Layer (F.Cu, B.Cu, Masking, Silk - Vias), etc
   [TN]
1. Simple Schematic
	R + C + IC + Power Supply + Konektor (KF2510)
(gunakan footprint through-hole)
	harus menggunakan Label : Gnd + Power Supply
Check integrity
    [exercise]
   TN
2. Simple PCB (dari hasil 1)
	Import Schematic
	Buat PCB :
- Teks (nama PCB, versi, tgl)
- Check Integrity
- [exercise]
- TN
3. Advanced Schematic
	rangkaiann : * Menggunakan Bus ()
	* Upload Custom Component dari eksternal (check SnapEDA) - Misalkan modul ESP-32 Wroom , modul ADS1115
    * [exercise]
- TN
4. Advanced PCB
	Rangkaian dgn Custom Footprint - misal : Pocket Beagle, Raspbery Pi Zero, Arduino Leonardo/Nano/Uno ← harus buat sendiri symbol + footprint
	* Power Plane (optional - Modif)
    *  [exercise]
- TN
  5. Miscellaneous
a. External --> Order PCB
    * BOM (Bill of Material)
    * Gerber File
  Configurasi
    * Track Width
AutoTrack
(exercise)
      (TN)

[NeXT - Simulasi rangkaian menggunakan ngspice]

Instumentasi
- Pengukuran Impendansi 
- Signal Generator (AD9833), VCCS (voltage to current), Low Noise OpAmp
- Precision timing dan Location + Sensor --> GPS –>
  (Konsep NMEA), GPS Repeater.

Instumentasi
- Remote Monitoring System
- Low Power Sensor System

Requirements
- Power Source, Power Management,
- Data Acquisition
- Data Transfer
- Data management
-  Analytics 
