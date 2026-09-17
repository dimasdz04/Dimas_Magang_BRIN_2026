Introduction to Interface
-------------------------
- Operating condition
  - Operating voltage (12v / 5v / 3.3v)
  - Microcontroller / Interface voltage
  - 5v tolerant 
  - Level Translator
  - Exercise : measure digital pin output voltage
- Digital Input Output
  - Exercise :
    1 push button to turn on/off LED
    2 counter to measure how many times the button is pushed
      - apakah perlu de-bouncing, how to?
- Serial Port
    - Concepts : 
    - port vs bus
    - hardware signal (RTS, etc)
    - software level - data format (bit level) : start/stop/bit rate/etc
    - Difference between RS232 (12v, DB-9 pin) and TTL (5v or other voltage, pin)
- Exercise : 
    - kirim data ke PC
    - coba software serial
- ADC / DAC
    - Concept : 
    - Resolution (sampling speed, quantization)

Interface - Intermediate (Digital I/O)
------------------------
- I2C
  - Concepts : bus, speed, pins, configuration : hw/sw (pullup etc) 
  - Limitation
  - Exercise : LCD display
- OneWire
  - Concept
  - Limitation
  - Exercise : read DS18b20
- SPI
  - Concept
  - Exercise : read / write W25Q31
- RS485 bus
  - Concept : bus, serial with differential
  - Limitation
  - Exercise : 
    - ubah komunikasi serial port dengan RS485
    - multiple device ?
  
Interface : Advanced
--------------------

- Modbus
  - Concept : protocol with RS485 bus


Communication
- Modem
- BLE
- LoRa

Measurements
- 
