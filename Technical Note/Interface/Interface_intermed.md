Interface - Intermediate

-----------------------
- I2C
- Concepts : bus, speed, pins, configuration : hw/sw (pullup etc)
- Limitation?
- Exercise : LCD display with PF85xx

- SPI

- Concept
- Exercise : read / write W25Q31


**Daftar Percobaan I²C (Sensor BH1750 & GY-91)**


  - **Pemindaian Jalur (I2C Scanner):** Menjalankan program pemindai alamat untuk mendeteksi
sensor BH1750 dan GY-91 yang dirangkai secara paralel pada satu _bus_ I²C (pin SDA dan SCL), dan

berhasil melacak alamat heksadesimal keduanya beroperasi bersamaan tanpa saling

mengganggu.


  - **Resolusi Konflik Alamat:** Melakukan manipulasi _hardware_ dengan mengubah logika tegangan

pada pin ADDR di modul sensor (menariknya ke jalur HIGH atau LOW). Percobaan ini

membuktikan bahwa dua sensor yang bentuknya identik bisa diubah alamat _software_ -nya agar

tidak bertabrakan di satu _bus_ .


  - **Uji Kecepatan Optimal (BH1750):** Mengubah parameter kecepatan _clock_ I²C dan berhasil

mencapai titik operasional tinggi di 88.000 Hz, yang menghasilkan durasi penarikan data sensor

pencahayaan dalam waktu 120 mikrodetik.


  - **Uji Ekstrem** _**Underflow**_ **(GY-91):** Memasukkan parameter kecepatan di luar spesifikasi fisik

Arduino Uno, yaitu 10 MHz. Percobaan ini membuktikan terjadinya limitasi ukuran _register_

( _integer underflow_ ), di mana waktu pembacaan GY-91 justru membengkak menjadi 2524

mikrodetik.


**Daftar Percobaan SPI (Modul W25Q32)**


  - **Validasi Perangkat Keras (JEDEC ID):** Mengirimkan instruksi _raw_ 0x9F ke cip memori untuk
memvalidasi sambungan 4 kabel (CS, MOSI, MISO, SCK). Percobaan sukses mendeteksi identitas

pabrikan Winbond (0xEF), tipe memori (0x40), dan kapasitas 4 Megabyte (0x16).


  - **Siklus Baca-Tulis Memori Flash:** Mengeksekusi urutan hukum fisika memori _Flash_ secara murni
tanpa _library_ pihak ketiga. Urutan yang dilakukan meliputi pembukaan akses ( _Write Enable_ ),

penghapusan sektor blok silikon ( _Sector Erase_ ), penulisan teks "SPI" ( _Page Program_ ), dan

pembacaan kembali untuk memverifikasi keberhasilan penyimpanan.


  - **Uji Kecepatan** _**Continuous Read**_ **(Non-Blocking):** Membuat program _State Machine_ 100% _non-_

_blocking_ untuk menarik 1000 _byte_ data kosong secara beruntun dari alamat awal. Percobaan

secara langsung membandingkan waktu tempuh _clock_ 1 MHz (9296 mikrodetik) dengan

kecepatan maksimal Arduino Uno 8 MHz (2232 mikrodetik).




---

**Kesimpulan Eksperimen I²C**


  - **Eksperimen** _**Bus**_ **&** _**I2C Scanner**_ **:** Kita telah berhasil merangkai 4 komponen I2C berbeda ( **BH1750**
x2 dan **GY-91** ) dan LCD di atas satu jalur _bus_ I²C (SDA & SCL) yang sama. Program _I2C Scanner_

juga berhasil mendeteksi alamat heksadesimal kedua perangkat tersebut di dalam jaringan

secara akurat tanpa saling bertabrakan.


  - **Resolusi Konflik Alamat I²C:** Berdasarkan percobaan, konflik alamat (saat menggunakan dua

sensor yang identik) dapat diselesaikan dengan memanipulasi perangkat keras. Kita harus

mengubah status **Pin ADDR** pada modul (menyambungkannya ke HIGH atau LOW) untuk
mengubah alamat perangkat lunaknya (misalnya pada BH1750 dari 0x23 menjadi 0x5C),

sehingga keduanya bisa hidup berdampingan di satu _bus_ .


  - **Limitasi** _**Hardware**_ **& Uji Kecepatan:** Pada pengujian sensor BH1750, kita berhasil
mengoptimalkan kecepatan hingga **88.000 Hz (88 kHz)** dengan rekor waktu penarikan data yang

sangat singkat, yaitu sekitar **120 mikrodetik** .


  - **Bug Siluman (** _**Integer Underflow**_ **):** Saat menguji sensor kelas industri GY-91 dengan menginput

kecepatan ekstrem 10 MHz, Arduino tidak _crash_ . Namun, limitasi ukuran _register hardware_

memicu _underflow_, yang justru menyebabkan kecepatan anjlok parah menjadi sekitar 31 kHz

(waktu baca membengkak menjadi ~2524 mikrodetik).


**Kesimpulan Eksperimen SPI**


  - **Kecepatan Brutal & Batas Arduino:** Komunikasi perangkat keras SPI pada Arduino Uno memiliki
batas maksimal **8 MHz** . Dengan fitur _Auto-Increment_ dan arsitektur _Full-Duplex_ (jalur ganda:

MOSI & MISO), kita berhasil menyedot 1000 _byte_ data secara beruntun dari cip **W25Q32** hanya

dalam waktu **~2,2 milidetik** (2232 mikrodetik).


  - **Bottleneck pada CPU:** Pada kecepatan maksimal 8 MHz, transmisi data tidak lagi dihambat oleh

jalur SPI, melainkan oleh _Software Overhead_ (waktu yang dihabiskan oleh CPU Arduino untuk

berputar mengeksekusi instruksi kodingan).


  - **Hukum Fisika** _**Flash Memory**_ **:** Memori _Flash_ tidak mengizinkan penimpaan data secara instan.

Blok silikon wajib dikosongkan terlebih dahulu menjadi 0xFF (bit 1) melalui perintah _Sector_

_Erase_, sebelum data baru bisa disuntikkan (bit 0) menggunakan perintah _Page Program_ .


  - **Sistem Pengalamatan:** SPI tidak menggunakan sistem alamat _software_ . Proses pemilihan target

jauh lebih instan karena murni menggunakan kontrol perangkat keras, yaitu dengan menarik **Pin**

**Fisik CS (** _**Chip Select**_ **)** ke LOW.


