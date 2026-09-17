# Technical Note: Komponen Elektronik, PCB, dan Alur Desain dengan KiCad

*Disusun berdasarkan gabungan catatan teknis mandiri, materi "Pengenalan Software KiCad dan Pembuatan PCB", dan Technical Notes Magang BRIN — Dasar Desain PCB dan Terminologi KiCad.*

---

## Daftar Isi

1. Pendahuluan
2. Komponen Elektronik Dasar
3. Konsep Dasar PCB (Printed Circuit Board)
4. Mengenal KiCad
5. Cara Kerja KiCad (Arsitektur & Alur Data)
6. Referensi Cepat: Toolbar dan Shortcut KiCad
7. Studi Kasus: Membuat Rangkaian Sederhana (Piano NE555)
8. Membuat Simbol dan Footprint Kustom
9. Import Simbol/Footprint dari Sumber Eksternal
10. Bus Routing pada Skematik
11. Alur Lengkap Pembuatan PCB dengan KiCad (Step-by-Step)
12. Standarisasi & Best Practice Desain
13. Proses Fabrikasi & Assembly (PCBA)
14. Troubleshooting Umum
15. Glosarium Istilah

---

## 1. Pendahuluan

Dokumen ini adalah catatan teknis (technical note) yang membahas secara terpadu:

1. **Dasar-dasar komponen elektronik** — jenis, fungsi, simbol, packaging, dan parameter penting sebelum mendesain rangkaian.
2. **PCB (Printed Circuit Board)** — konsep fisik papan sirkuit, jenis layer, material, hingga proses fabrikasinya.
3. **KiCad** — software EDA (Electronic Design Automation) open-source untuk mendesain skematik dan layout PCB, lengkap dengan cara kerja internal, referensi toolbar/shortcut, studi kasus praktik langsung (rangkaian piano sederhana berbasis NE555), pembuatan simbol/footprint kustom, hingga standarisasi kerja ala tim teknis (disarikan dari pengalaman magang).

Dalam pengembangan sistem tertanam (*embedded systems*), kemampuan menerjemahkan skematik menjadi desain fisik board sangat krusial — KiCad menyediakan alur kerja komprehensif mulai dari pembuatan simbol hingga layout PCB siap fabrikasi.

Cocok digunakan sebagai referensi tugas kuliah, project embedded system/IoT, maupun dokumentasi internal tim.

---

## 2. Komponen Elektronik Dasar

Komponen elektronik dibagi menjadi dua kelompok besar: **komponen pasif** dan **komponen aktif**, ditambah komponen elektromekanik dan konektor.

### 2.1 Komponen Pasif

Komponen pasif tidak dapat menguatkan sinyal atau mengubah energi menjadi bentuk lain secara aktif; hanya menyimpan, membuang, atau membatasi energi.

| Komponen | Fungsi | Satuan | Parameter Penting |
|---|---|---|---|
| **Resistor** | Membatasi arus, membagi tegangan | Ohm (Ω) | Toleransi (±%), rating daya (W), koefisien suhu |
| **Kapasitor** | Menyimpan muatan, filter, decoupling, kopling AC | Farad (F) | Voltage rating, ESR, dielektrik (ceramic, tantalum, elektrolit) |
| **Induktor** | Menyimpan energi dalam medan magnet, filter, choke | Henry (H) | Arus saturasi, DCR (resistansi DC), frekuensi resonansi |
| **Transformator** | Mengubah level tegangan AC, isolasi galvanis | — | Rasio lilitan, daya, frekuensi kerja |
| **Kristal/Resonator** | Sumber clock presisi | Hz | Frekuensi, load capacitance, stabilitas (ppm) |

Catatan praktis:
- Resistor SMD umum memakai kode 3-4 digit (mis. "103" = 10 kΩ) atau kode warna untuk versi through-hole.
- Kapasitor ceramic MLCC rentan terhadap **voltage derating** — kapasitansi efektif turun saat tegangan mendekati rating.
- Induktor power (untuk regulator switching) harus dipilih berdasarkan arus saturasi, bukan hanya nilai induktansi.

### 2.2 Komponen Aktif

Komponen aktif dapat menguatkan sinyal atau mengontrol aliran arus menggunakan sumber energi eksternal.

| Komponen | Fungsi | Contoh Tipe |
|---|---|---|
| **Dioda** | Menyearahkan arus (satu arah), proteksi, referensi tegangan (Zener) | 1N4148, 1N5819 (Schottky), Zener |
| **Transistor BJT** | Saklar/penguat arus | NPN (2N2222, BC547), PNP (2N2907) |
| **MOSFET** | Saklar daya, penguat tegangan | N-channel, P-channel |
| **IC (Integrated Circuit)** | Fungsi kompleks dalam satu chip | Mikrokontroler, op-amp, regulator, timer (NE555), sensor |
| **Optocoupler** | Isolasi sinyal secara optik | PC817 |

Kategori IC yang sering dipakai di project embedded/IoT:
- **MCU (Microcontroller Unit)** — ESP32, STM32, ATmega, dsb.
- **Voltage Regulator** — Linear (LDO, mis. AMS1117) atau switching (buck/boost, mis. MP2307).
- **Sensor IC** — suhu (DS18B20, DHT22), akselerometer (MPU6050), dsb.
- **Communication IC** — level shifter, RS485 transceiver (MAX485), CAN transceiver.
- **IC Timer** — mis. NE555, sering dikonfigurasi sebagai *astable multivibrator* untuk menghasilkan sinyal gelombang kotak (frekuensi suara/clock sederhana).

### 2.3 Komponen Elektromekanik & Konektor

- **Switch/Push button** — input digital manual.
- **Relay** — saklar mekanik dikendalikan sinyal listrik, untuk beban daya besar.
- **Connector/Header** — pin header, JST, terminal block, USB, untuk interkoneksi antar board/kabel.
- **LED** — indikator visual, perhatikan **forward voltage** dan **arus maksimum** (perlu resistor pembatas).
- **Speaker** — mengubah sinyal listrik (mis. gelombang kotak dari IC timer) menjadi gelombang suara.

### 2.4 Metode Pemasangan: Through-Hole vs SMD

| Metode | Deskripsi | Contoh Komponen |
|---|---|---|
| **Through-Hole (THT)** | Kaki komponen dimasukkan menembus lubang bor pada PCB, disolder dari sisi sebaliknya | Resistor karbon lama, kapasitor elektrolit besar, konektor pin header |
| **SMD (Surface Mount Device)** | Komponen ditempel dan disolder langsung pada permukaan PCB (pad) tanpa menembus papan | Resistor/kapasitor chip 0603/0805, IC package modern |

SMD memungkinkan kepadatan komponen jauh lebih tinggi pada board dibanding THT, namun THT umumnya lebih mudah disolder manual dan lebih kuat secara mekanis untuk komponen yang menahan beban fisik (mis. konektor, elektrolit besar).

### 2.5 Packaging Komponen

**Packaging** adalah bentuk kemasan fisik standar suatu komponen yang menentukan luas area yang dibutuhkan di PCB serta footprint yang harus dipakai. Pemilihan packaging bergantung pada kebutuhan daya, ruang board, dan kemudahan penyolderan (manual vs otomatis).

| Packaging | Tipe | Kegunaan |
|---|---|---|
| **TO-3** | THT, logam besar | Aplikasi daya tinggi, umumnya butuh heatsink |
| **TO-92** | THT, plastik kecil, 3 kaki | Transistor daya rendah (mis. BC547) |
| **SOT/SSOT (Small Outline Transistor)** | SMD kecil | Transistor & regulator tegangan rendah (mis. SOT-23) |
| **Package IC SMD lain** (SOIC, QFN, TQFP, dsb.) | SMD | Beragam IC — pemilihan harus mempertimbangkan kesesuaian ukuran, spesifikasi tegangan, dan dimensi yang tersedia secara komersial |

**Pitch** adalah jarak antara pusat satu pin ke pusat pin berikutnya (jarak spasi standar antar kaki komponen/konektor). Pitch yang lebih kecil (*fine pitch*) membutuhkan presisi manufaktur dan penyolderan yang lebih tinggi — penting dipertimbangkan terutama jika prototype akan disolder manual.

### 2.6 Membaca Datasheet

Sebelum memilih komponen untuk desain PCB, hal yang wajib dicek di datasheet:
1. **Absolute maximum ratings** (tegangan, arus, suhu operasi).
2. **Pinout & package** (menentukan footprint yang dipakai di KiCad).
3. **Electrical characteristics** (parameter operasi normal).
4. **Recommended land pattern / footprint** (biasanya di bagian akhir datasheet, penting untuk desain PCB yang benar).
5. **Application circuit** (rangkaian referensi dari pabrikan, sangat membantu untuk desain awal).

---

## 3. Konsep Dasar PCB (Printed Circuit Board)

PCB adalah papan yang menghubungkan komponen elektronik secara mekanis dan elektris menggunakan jalur konduktif (**trace/track**) tanpa kabel.

### 3.1 Jenis PCB Berdasarkan Jumlah Layer

| Jenis | Jumlah Layer Copper | Kegunaan |
|---|---|---|
| Single-layer | 1 | Rangkaian sederhana, low-cost |
| Double-layer | 2 | Mayoritas project hobby/embedded (Arduino shield, dsb.) |
| Multi-layer | 4, 6, 8, dst. | Desain kompleks (high-speed digital, RF, high-density) |

### 3.2 Anatomi Layer PCB (Terminologi KiCad/Pcbnew)

PCB modern biasanya menggunakan desain multi-layer. Berikut terminologi layer yang umum ditemukan di KiCad:

| Nama Layer | Deskripsi Fungsi |
|---|---|
| **F.Cu (Front Copper)** | Lapisan tembaga utama bagian atas/depan tempat jalur sinyal dan pad SMD tercetak |
| **B.Cu (Back/Bottom Copper)** | Lapisan tembaga bagian bawah/belakang, sering untuk ground plane atau jalur tambahan |
| **F.Mask / B.Mask (Solder Mask)** | Lapisan isolator pelindung (umumnya hijau) yang menutupi jalur tembaga agar terhindar dari korsleting dan oksidasi, hanya menyisakan area yang akan disolder |
| **F.Silk / B.Silk (Silkscreen)** | Lapisan tinta (umumnya putih) untuk mencetak teks informasi, nama komponen (mis. R1, C10), indikator polaritas, dan logo |
| **Edge.Cuts** | Garis batas fisik/bentuk dari board PCB itu sendiri |

### 3.3 Konektivitas dan Fitur Fisik

- **Vias** — lubang berlapis tembaga yang ditanam pada PCB untuk menghubungkan jalur kelistrikan antara layer atas (F.Cu) dan layer bawah (B.Cu), termasuk *blind via* dan *buried via* pada desain multilayer.
- **Pad** — area tembaga terbuka tempat kaki komponen disolder.
- **Trace (jalur)** — "kabel" tembaga pada PCB yang menghubungkan antar komponen; lebar trace harus disesuaikan dengan besar arus yang mengalir.
- **Clearance** — jarak minimum yang aman antara dua elemen tembaga berbeda untuk mencegah hubungan pendek (*short circuit*).

### 3.4 Material PCB

- **FR4** — material paling umum, fiberglass + epoxy resin, tahan panas dan cukup kuat secara mekanis.
- **Aluminium (MCPCB)** — untuk aplikasi dengan disipasi panas tinggi, mis. PCB LED power.
- **Flex PCB (Polyimide)** — PCB fleksibel untuk aplikasi yang membutuhkan lekukan.
- **Rogers** — material khusus untuk RF/high-frequency dengan loss rendah.

### 3.5 Parameter Fabrikasi Penting

- **Copper weight** — ketebalan tembaga, umum 1 oz (35 µm) atau 2 oz untuk arus lebih besar.
- **Trace width & clearance** — dihitung berdasarkan arus yang lewat (lihat kalkulator IPC-2221) dan kemampuan fabrikasi (umumnya minimum 0.15–0.2 mm untuk fabrikasi standar/hobby).
- **Board thickness** — umum 1.6 mm, tapi bisa disesuaikan (0.8 mm, 1.0 mm, 2.0 mm).
- **Surface finish** — HASL, ENIG (emas), OSP — mempengaruhi solderability dan umur simpan board.

---

## 4. Mengenal KiCad

**KiCad** adalah software EDA open-source (gratis, cross-platform: Windows/Linux/macOS) untuk mendesain skematik elektronik dan layout PCB secara end-to-end, lengkap dengan simulasi dasar dan visualisasi 3D. Dalam ekosistem KiCad, terdapat pemisahan logis yang jelas antara fungsi komponen (**Symbol**, di modul Eeschema) dan bentuk fisiknya (**Footprint**, di modul Pcbnew).

### 4.1 Modul Utama KiCad

| Modul | Fungsi |
|---|---|
| **KiCad Project Manager** | Mengelola file-file project (schematic, PCB, library lokal) |
| **Schematic Editor (Eeschema)** | Menggambar diagram skematik rangkaian |
| **Symbol Editor** | Membuat/mengedit simbol komponen untuk skematik |
| **PCB Editor (Pcbnew)** | Layout fisik PCB — penempatan komponen dan routing trace |
| **Footprint Editor** | Membuat/mengedit footprint (land pattern) komponen |
| **Gerber Viewer** | Melihat dan verifikasi file Gerber sebelum kirim ke fabrikasi |
| **3D Viewer** | Visualisasi board dalam bentuk 3D dengan model komponen |
| **Plugin & Content Manager (PCM)** | Mengelola library tambahan dan plugin pihak ketiga |

### 4.2 Konsep Library di KiCad

KiCad memisahkan tiga jenis data yang saling terhubung:

1. **Symbol** — representasi visual/logis komponen di skematik (mis. simbol zigzag untuk resistor).
2. **Footprint** — representasi fisik pad komponen di PCB, ukurannya disesuaikan persis dengan kaki komponen fisik yang akan disolder.
3. **3D Model** — model visual (STEP/WRL) untuk keperluan 3D viewer, tidak mempengaruhi fungsi elektris.

Ketiganya dihubungkan melalui field pada simbol komponen. Satu symbol bisa dipasangkan ke beberapa pilihan footprint (mis. resistor bisa dipasang footprint 0603 atau 0805).

### 4.3 Terminologi Skematik Tambahan

- **Label** — penanda teks pada sebuah jalur kabel (*wire*) yang memungkinkan dua jalur terhubung secara logis tanpa perlu menarik garis panjang yang membuat skematik berantakan; jalur dengan nama label yang sama dianggap terhubung secara elektrik.
- **Bus** — jalur tebal yang merepresentasikan kumpulan beberapa jalur sinyal/kabel (mis. bus data 8-bit) yang disatukan agar skematik lebih rapi.
- **Netlist** — daftar koneksi yang menghubungkan semua komponen, diekspor dari skematik menuju layout PCB.

---

## 5. Cara Kerja KiCad (Arsitektur & Alur Data)

Secara konseptual, KiCad bekerja dengan alur data seperti berikut:

```
[Symbol Library] --> [Schematic Editor] --> [Netlist/Ratsnest]
                                                    |
                                                    v
[Footprint Library] --> [PCB Editor] <----- (import netlist)
                                |
                                v
                     [DRC + 3D Review]
                                |
                                v
                  [Gerber/Drill/Pick&Place files]
```

Penjelasan:

1. **Schematic Editor** merepresentasikan koneksi logis antar komponen (siapa terhubung ke siapa) — belum ada informasi posisi fisik.
2. Dari skematik, KiCad menghasilkan **netlist** — daftar koneksi antar pin yang menjadi "kontrak" yang harus dipenuhi di layout PCB.
3. **PCB Editor** membaca netlist ini dan menampilkannya sebagai **ratsnest** (garis lurus penghubung sementara) di antara pad komponen yang belum di-routing.
4. Saat trace digambar mengikuti ratsnest, KiCad menandai koneksi tersebut "selesai".
5. **Design Rule Check (DRC)** memverifikasi bahwa layout memenuhi aturan fisik (clearance, trace width minimum, dll) sekaligus memastikan tidak ada koneksi elektris yang hilang (unrouted nets) atau short circuit.
6. Setelah semua clear, KiCad melakukan **export/plot** ke format standar industri (Gerber RS-274X, Excellon drill file) yang dipakai oleh pabrik PCB.

Konsep kunci: **schematic dan PCB selalu tersinkronisasi lewat netlist** — perubahan di skematik (menambah/menghapus komponen atau koneksi) perlu di-*update* ke PCB melalui fitur "Update PCB from Schematic".

---

## 6. Referensi Cepat: Toolbar dan Shortcut KiCad

### 6.1 Toolbar Umum (Schematic Editor)

| Tool | Fungsi |
|---|---|
| Grid toggle | Mengaktifkan/menonaktifkan grid |
| Unit selector | Menentukan satuan dimensi (inci, mil, mm) |
| Crosshair mode | Mengubah bentuk crosshair menjadi full/kecil |
| Hidden pins toggle | Mengaktifkan/menonaktifkan pin yang disembunyikan secara default |
| Line mode | Menentukan garis bebas atau tegak lurus/45° sebagai kabel penghubung |
| Annotate | Menentukan anotasi terhadap simbol baru |
| Navigator panel | Membuka/menutup window navigator |

### 6.2 Toolbar Aksi Skematik (Eeschema)

| Tool (Shortcut) | Fungsi |
|---|---|
| Select Items (**S**) | Memilih item |
| Wire color | Memberikan warna pada kabel |
| Place Symbols (**A**) | Menambahkan komponen |
| Place Power Symbols (**P**) | Menambahkan ground/power |
| Place Wire | Menambahkan kabel |
| Place Bus | Menambahkan jalur bus |
| Place Wire to Bus Entry | Menambahkan kabel ke jalur bus |
| Place No Connect Flag (**Q**) | Menandai pin komponen yang sengaja tidak terhubung |
| Place Junction | Menambahkan simpangan (junction) |
| Place Net Label | Menambahkan net label |
| Place Label | Menambahkan label pada net yang belum berlabel |
| Place Global Label | Menambahkan label global yang dapat terhubung antar sheet berbeda |
| Hierarchical Label | Menerapkan label hirarki |
| Hierarchical Sheet | Menerapkan hirarki subsheet |
| Import Sheet Pin | Import hirarki pin dari subsheet |
| Add Text | Menambahkan teks |
| Add Text Box | Menambahkan kotak teks |

### 6.3 Toolbar File & Edit (Umum di Semua Editor)

| Tool | Fungsi |
|---|---|
| New / Open / Save | Membuat, membuka, menyimpan dokumen |
| Schematic Setup | Mengedit pengaturan skematik |
| Page Settings | Mengatur ukuran halaman |
| Print / Plot | Mencetak / plot ke file produksi |
| Undo / Redo | Memundurkan / memajukan editan terakhir |
| Zoom In/Out/Fit/Selection | Mengatur jarak pandang tampilan |
| Rotate CCW/CW | Memutar berlawanan/searah jarum jam |
| Mirror Horizontal/Vertical | Mencerminkan objek secara horizontal/vertikal |
| Symbol Editor | Membuat, menghapus, dan mengedit simbol |
| Symbol Library Search | Mencari library simbol |
| Footprint Editor | Membuat, menghapus, dan mengedit footprint |
| Annotate Schematic | Mengisi designator simbol skematik |
| Electrical Rules Checker (ERC) | Mengecek adanya error elektris |
| Simulator | Simulasi sirkuit |
| Assign Footprints | Menerapkan footprint pada simbol |
| Edit Symbol Fields | Mengedit seluruh simbol pada skematik sekaligus |
| Generate BOM | Membuat *material bill* (BOM) dari skematik saat ini |
| Switch to PCB Editor | Membuka PCB editor |
| Scripting Console | Menampilkan konsol script Python |

### 6.4 Toolbar PCB Editor (Pcbnew)

| Tool | Fungsi |
|---|---|
| Polar/Cartesian toggle | Mengatur tampilan koordinat polar atau kartesian |
| Show Tracks | Menampilkan jalur PCB |
| Show Curved Tracks | Menampilkan jalur PCB melengkung |
| Layer dim toggle | Menampilkan layer tidak aktif secara normal atau redup |
| Highlight Net | Menyorot (highlight) suatu net tertentu |
| Pad outline toggle | Menampilkan outline pad |
| Via outline toggle | Menampilkan outline via |
| Board Setup | Mengedit pengaturan board |
| Update PCB from Schematic (**F8**) | Membaca netlist dan memperbarui koneksi board |
| Switch Active Layer | Mengganti pasangan layer aktif untuk routing |
| Select | Memilih jalur PCB |
| Add Footprint | Menambahkan footprint |
| Route Single Track (**X**) | Menambahkan rute track |
| Tune Track Length | Mengatur panjang satu track |
| Add Via (Free) | Menambahkan via bebas |
| Add Filled Zone | Menambah zona untuk diisi (copper pour) |
| Add Rule Area | Menambah area larangan/keepout |
| Draw Line/Arc | Membuat garis lurus atau garis lengkung |
| Draw Rectangle | Menambahkan bentuk kotak |
| Draw Circle | Menambahkan bentuk lingkaran |
| Draw Polygon | Menambahkan bentuk bebas (poligon) |
| Add Image | Menambahkan gambar |

---

## 7. Studi Kasus: Membuat Rangkaian Sederhana (Piano NE555)

Sebagai contoh praktik langsung, berikut alur pembuatan rangkaian **piano sederhana** berbasis IC timer NE555 yang dikonfigurasi sebagai *astable multivibrator* untuk menghasilkan nada suara berbeda saat tombol ditekan.

### 7.1 Daftar Komponen

| Referensi | Nilai/Part | Kategori | Fungsi Utama |
|---|---|---|---|
| U2 | NE555P | IC Timer | "Otak" rangkaian, dikonfigurasi sebagai astable multivibrator penghasil gelombang kotak (frekuensi suara) |
| R1–R5 | 1k | Resistor | Membentuk resistor ladder (jaringan seri) untuk mengubah total resistansi & menghasilkan nada berbeda saat tombol ditekan |
| R6 | 4.7k | Resistor | Menentukan batas resistansi minimum jalur, mengatur nada tertinggi/batas dasar frekuensi bersama kapasitor |
| R7 | 1k | Resistor | Mengatur waktu pengisian (charging) kapasitor C1 — berperan sebagai resistor utama (Ra) di pin 7 IC 555 |
| SW1–SW6 | SW_Push | Push Button | Berfungsi seperti tuts piano; menekan tombol "memotong" (bypass) jalur resistor tertentu untuk mengubah frekuensi suara |
| C1 | 100nF | Kapasitor Keramik | Menentukan rentang frekuensi suara utama (bersama kombinasi resistor) |
| C2 | 10µF | Kapasitor Elektrolit | Kapasitor coupling (DC blocking) — memblokir arus DC dari IC 555 dan hanya meloloskan sinyal AC ke speaker agar koil speaker tidak panas/rusak |
| LS1 | Speaker | Output Audio | Mengubah sinyal listrik (gelombang kotak) menjadi gelombang suara |
| J1 | Conn_01x02_Pin | Konektor | Pin header 2-pin (VCC & GND) untuk sumber tegangan (baterai/adaptor 9V) |

### 7.2 Langkah Skematik

1. Buat project baru: **File → New Project** (Ctrl+N).
2. Buka file `.kicad_sch`, masuk mode **Place Symbols** (shortcut **A**) untuk menambahkan komponen sesuai tabel di atas.
3. Tambahkan simbol power (VCC, GND) dengan mode **Place Power Symbols** (shortcut **P**).
4. Lakukan wiring: masuk mode **Select Items** (shortcut **S**), lalu hubungkan pinout yang diinginkan dengan menarik wire.
5. Untuk pin yang sengaja tidak dipakai, pasangkan **No Connect Flag** (shortcut **Q**) agar tidak dianggap error oleh ERC.
6. Jalankan **Electrical Rules Checker (ERC)** — pastikan tidak ada error/warning yang signifikan sebelum lanjut.

### 7.3 Konversi Skematik ke PCB

1. Setelah ERC bersih, gunakan **Assign Footprints** untuk menyesuaikan footprint tiap komponen dengan bentuk fisik komponen sebenarnya.
2. Klik **Switch to PCB Editor**, lalu jalankan **Update PCB from Schematics** (shortcut **F8**) mengikuti konfigurasi yang muncul, kemudian klik **Update PCB**.
3. Lakukan routing: pilih layer **F.Cu**, gunakan tool **Route Single Track** (shortcut **X**) untuk menghubungkan ratsnest menjadi trace fisik.
4. Cek tampilan hasil desain secara 3D dengan shortcut **Alt+3**.
5. Jalankan **Design Rules Checker (DRC)** untuk memastikan tidak ada pelanggaran aturan desain sebelum board dianggap final.

---

## 8. Membuat Simbol dan Footprint Kustom

Ketika komponen yang dibutuhkan tidak tersedia di library bawaan KiCad, kita dapat membuat simbol dan footprint sendiri.

### 8.1 Tipe Pin pada Symbol Editor

Saat membuat simbol kustom (mis. modul layar ST7789), setiap pin perlu diberi tipe yang sesuai agar ERC dapat memvalidasi rangkaian dengan benar.

**Tipe Pin Sinyal & Data**
- **Input** — pin yang murni menerima sinyal; jika tidak tersambung ke sumber sinyal apa pun, KiCad akan memberi peringatan (contoh: pin SCL/SDA pada layar ST7789 yang menerima perintah dari ESP32).
- **Output** — pin yang mengirim sinyal ke komponen lain; KiCad akan memberi error jika dua pin Output disambungkan bersamaan karena berisiko tabrakan arus (contoh: pin TX mikrokontroler).
- **Bidirectional** — pin dua arah, bisa berganti fungsi Input/Output tergantung kode program (contoh: pin data pada protokol I2C).

**Tipe Pin Daya (Power)**
- **Power input** — pin yang membutuhkan suplai tegangan/ground agar komponen bisa hidup; jika dibiarkan menggantung, ERC akan memberi peringatan (contoh: pin VCC & GND pada layar ST7789).
- **Power output** — pin yang menyediakan/mengeluarkan tegangan untuk komponen lain (contoh: pin 3.3V keluaran IC regulator/LDO).

**Tipe Pin Lainnya**
- **Passive** — untuk komponen pasif dasar yang arusnya bisa mengalir bolak-balik tanpa memanipulasi sinyal secara aktif (contoh: pin resistor, kapasitor, induktor).
- **Tri-state** — pin output logika dengan tiga keadaan: High, Low, dan High-Impedance (mengambang); umum dipakai pada bus memori paralel.
- **Open collector / Open drain** — pin output khusus yang tidak bisa mengeluarkan tegangan High sendiri, sehingga wajib ditambahkan resistor *pull-up* eksternal.
- **Unspecified / Free** — dipakai jika fungsi pin sangat fleksibel atau belum yakin fungsinya; KiCad tidak melakukan pengecekan ketat pada pin ini.
- **No connect** — pin fisik pada IC yang memang dirancang pabrik untuk dibiarkan kosong dan tidak boleh disambungkan ke jalur mana pun.

### 8.2 Langkah Membuat Symbol Kustom

1. Buka **Symbol Editor**, klik **File → New Library**, beri nama library.
2. Buat **New Symbol**, gambar bentuk yang diinginkan, lalu tambahkan pin (di panel kanan atas) sesuai tipe pin pada bagian 8.1.
3. Setelah semua pin sesuai kebutuhan, simbol kustom siap dipakai di skematik.

### 8.3 Langkah Membuat Footprint Kustom

1. **Buat Library Footprint** — buka **Footprint Editor**, klik **File → New Library...**, pilih **Project**, beri nama library (mis. `Footprint_Kustom`).
2. **Buat Footprint** — klik kanan pada library tersebut → **New Footprint**, beri nama (mis. `Modul_ST7789`), pilih tipe (mis. **Through hole**), klik OK.
3. **Gambar batas fisik komponen** (penting!):
   - Ubah ukuran grid menjadi **1.0 mm** agar mudah menggambar.
   - Pilih layer **F.Silkscreen**.
   - Gunakan tool **Draw a rectangle**, gambar kotak sesuai ukuran fisik komponen (mis. layar 41.8 mm × 43.7 mm).
4. **Buat zona larangan (keepout)** agar KiCad tidak meletakkan komponen lain di bawah komponen ini:
   - Klik kanan pada garis kotak yang baru dibuat → **Create from Selection → Create Rule Area from Selection**.
   - Hapus centang pada *Tracks* dan *Vias*, biarkan **Keep out footprints** tetap tercentang, lalu klik OK.
5. **Pasang pad solder**:
   - Ubah grid menjadi **2.54 mm** (jarak standar internasional untuk kaki komponen/pin header).
   - Gunakan tool **Add a pad** (shortcut **P**), letakkan pad berjejer sesuai jumlah kaki komponen — dengan grid 2.54 mm, jarak antar lubang otomatis presisi.
6. **Menandai Pin 1 & label** (standar industri):
   - Klik kanan pada pad nomor 1 → **Properties**, ubah *Pad shape* menjadi **Rectangular** untuk menandai posisi Pin 1.
   - Ubah grid menjadi **0.5 mm**, gunakan **Add text** (shortcut **T**) untuk memberi nama pin (VCC, GND, SCL, dst.) di atas masing-masing lubang pada layer F.Silkscreen.

---

## 9. Import Simbol/Footprint dari Sumber Eksternal

Terkadang komponen yang dibutuhkan tidak tersedia di library bawaan KiCad. Selain membuat simbol/footprint kustom sendiri (lihat Bab 8), kita juga bisa mencari komponen di website eksternal seperti **SnapEDA**.

Langkah umum:
1. Cari komponen yang ingin dipakai (mis. **ESP32-WROOM**) di situs penyedia, lalu pilih opsi **download symbol and footprint**.
2. Pilih format file sesuai software yang dipakai — dalam kasus ini pilih format **KiCad**.
3. File akan terunduh dalam format **.ZIP** — ekstrak untuk mendapatkan file library (biasanya berakhiran `_sym` untuk simbol, beserta file footprint terkait).
4. Kembali ke KiCad, buka tab **Preferences → Manage Symbol Libraries**, klik tombol **+**, lalu arahkan ke file yang sudah diekstrak untuk menambahkannya sebagai library baru.
5. Lakukan hal serupa pada **Manage Footprint Libraries** jika file footprint terpisah dari file simbol.

---

## 10. Bus Routing pada Skematik

**Bus** berguna untuk menyederhanakan tampilan skematik yang memiliki banyak jalur sinyal terkait (mis. bus data 8-bit), mirip seperti kabel LAN — dari luar terlihat sebagai satu kabel tebal, padahal di dalamnya terdapat banyak jalur kecil dengan fungsi berbeda-beda (jalur kirim data, terima data, dsb). Selain merapikan tampilan, bus route juga membuat skematik terlihat lebih profesional.

Langkah membuat bus route:
1. Siapkan rangkaian dengan beberapa jalur sinyal yang ingin dikelompokkan.
2. Gunakan tool **Place Wire to Bus Entry** ("place wires for bus entries") dan pasangkan ke masing-masing pin yang akan dimasukkan ke dalam bus.
3. Gunakan tool **Place Bus** ("Draw Buses") dan tarik garis (biasanya digambarkan berwarna biru tebal) dari titik kumpulan jalur ke arah tujuan.
4. Setelah bus terbentuk, beri **label** pada tiap jalur yang masuk ke bus (mis. `D0`, `D1`, ... `D7`) agar jelas kabel dari pin mana saja yang tergabung di dalam bus tersebut — tanpa label ini, KiCad tidak akan tahu bagaimana memisahkan kembali sinyal-sinyal di dalam bus saat sampai ke tujuan.

---

## 11. Alur Lengkap Pembuatan PCB dengan KiCad (Step-by-Step)

Bagian ini merangkum alur kerja umum dari nol hingga file siap fabrikasi — kombinasi antara best practice umum dan langkah praktis di KiCad.

### Tahap 0 — Perencanaan (Sebelum Membuka KiCad)

- Tentukan requirement: fungsi board, mikrokontroler yang dipakai, interface (I2C, SPI, UART, dsb.), catu daya, dan enclosure/dimensi board jika ada batasan fisik.
- Buat **block diagram** sederhana (power supply, MCU, sensor, komunikasi, output) untuk memetakan sub-sistem.
- Kumpulkan datasheet semua komponen utama.

### Tahap 1 — Membuat Project Baru

1. Buka KiCad → **New Project**, beri nama project.
2. KiCad otomatis membuat file `.kicad_pro` (project), `.kicad_sch` (schematic), dan `.kicad_pcb` (PCB layout).

### Tahap 2 — Desain Skematik (Schematic Capture)

1. Buka **Schematic Editor**.
2. Tambahkan simbol komponen (`Add Symbol`/mode **A**) dari library — cari berdasarkan nama part.
3. Susun simbol sesuai block diagram, lalu hubungkan pin-pin yang berelasi menggunakan **wire**.
4. Gunakan **label** dan **global label** untuk koneksi yang sama tanpa perlu digambar garis fisik (mis. label `GND`, `3V3`, `SDA`, `SCL`).
5. Tambahkan **power symbol** (VCC, GND, mode **P**) sesuai kebutuhan.
6. Beri **annotasi** referensi komponen (`Tools > Annotate Schematic`) sehingga tiap komponen punya reference designator unik (R1, R2, C1, U1, dst).
7. Assign **footprint** ke tiap simbol lewat **Assign Footprints**.
8. Jalankan **Electrical Rule Check (ERC)** untuk mendeteksi kesalahan seperti pin yang tidak terhubung, konflik output-output, atau net yang floating. Perbaiki semua warning/error signifikan.

### Tahap 3 — Generate Netlist / Update PCB

1. Gunakan menu **Update PCB from Schematic** (shortcut **F8**).
2. KiCad akan mentransfer seluruh komponen beserta footprint dan informasi koneksinya ke PCB Editor dalam bentuk ratsnest.

### Tahap 4 — Setup PCB (Sebelum Layout)

1. Buka **PCB Editor**.
2. Tentukan **board outline** di layer `Edge.Cuts`, sesuaikan dengan dimensi/enclosure yang direncanakan.
3. Atur **layer count** dan stackup sesuai kebutuhan (2-layer untuk kebanyakan project hobby/embedded sederhana).
4. Atur **design rules** (`Board Setup`): minimum trace width, clearance, via size, sesuai kemampuan fabrikasi vendor yang dituju.

### Tahap 5 — Placement (Penempatan Komponen)

1. Susun posisi komponen berdasarkan alur sinyal logis, kedekatan komponen terkait (mis. kapasitor decoupling sedekat mungkin dengan pin power IC), posisi konektor di tepi board, serta menjauhkan komponen panas dari komponen sensitif suhu.
2. Gunakan **3D Viewer** (Alt+3) secara berkala untuk mengecek potensi bentrok fisik antar komponen atau dengan enclosure.

### Tahap 6 — Routing (Menggambar Trace)

1. Ikuti garis **ratsnest** untuk menggambar trace pada layer **F.Cu**/**B.Cu** dengan tool **Route Single Track** (shortcut **X**).
2. Tentukan **trace width** berbeda untuk jalur signal biasa vs jalur power/arus besar.
3. Gunakan **via** untuk memindahkan trace antar layer bila diperlukan.
4. Buat **ground plane/pour** (**Add Filled Zone**) di salah satu layer untuk mengurangi resistansi/induktansi ground dan membantu shielding noise.
5. Perhatikan jalur sinyal sensitif (clock, analog, high-speed) — hindari rute yang terlalu panjang atau berdekatan dengan sumber noise.

### Tahap 7 — Design Rule Check (DRC)

1. Jalankan **DRC** untuk mendeteksi clearance violation, net yang belum ter-routing, atau pelanggaran aturan lain sesuai board setup.
2. Perbaiki semua error sebelum lanjut.

### Tahap 8 — Finishing Layout

1. Tambahkan **silkscreen** informatif (label pin penting, nama project, versi board, polaritas komponen) — pastikan silkscreen tidak menimpa pad.
2. Cek **copper pour** sudah ter-refill dengan benar.
3. Review keseluruhan board di **3D Viewer**.

### Tahap 9 — Generate File Produksi (Gerber)

1. Klik **File → Plot**, sesuaikan konfigurasi layer yang ingin di-export (Cu layers, solder mask, silkscreen, edge cuts) sesuai kebutuhan fabrikasi.
2. Klik **Plot**, tunggu proses selesai, lalu cek direktori output untuk memastikan seluruh file Gerber berhasil dibuat.
3. **Generate Drill File** (Excellon format) untuk lubang via dan through-hole.
4. **Generate Pick and Place file** (posisi & orientasi komponen SMD) jika PCB akan dirakit otomatis oleh pabrik (PCBA).
5. **Generate BOM (Bill of Materials)** — daftar komponen lengkap dengan reference designator, value, footprint, dan part number.
6. Kompres semua file Gerber + drill menjadi satu ZIP untuk diunggah ke vendor fabrikasi.

### Tahap 10 — Verifikasi Akhir

1. Buka hasil Gerber di **Gerber Viewer** untuk memastikan semua layer benar sebelum submit.
2. Bandingkan sekali lagi dengan skematik untuk memastikan tidak ada yang tertinggal.

---

## 12. Standarisasi & Best Practice Desain

Prinsip umum sekaligus standarisasi kerja tim teknis (disarikan dari pengalaman praktik magang):

- **Pemilihan komponen**: pastikan **pitch** komponen yang dipilih sesuai dengan kemampuan pabrikasi atau kemudahan penyolderan manual jika membutuhkan prototype.
- **Penamaan label**: gunakan label yang deskriptif pada skematik untuk mempermudah proses debugging pada sistem tertanam yang kompleks.
- **Manajemen layer**: gunakan layer tembaga (Cu) secara efisien, dan pastikan silkscreen tidak menimpa pad agar informasi pada board tetap terbaca setelah perakitan.
- **Decoupling capacitor**: tempatkan sedekat mungkin dengan pin VCC setiap IC (umumnya 100nF ceramic, ditambah kapasitor bulk lebih besar per rail power).
- **Trace width untuk arus**: gunakan referensi IPC-2221 atau kalkulator trace width online; jangan hanya menebak.
- **Ground plane solid**: hindari banyak "cut" di ground plane karena bisa menimbulkan loop arus balik yang tidak diinginkan, terutama untuk sinyal high-speed.
- **Jarak antar via/pad**: sesuaikan dengan kapabilitas fabrikasi vendor yang dituju.
- **Test point**: sediakan titik pengujian untuk sinyal-sinyal kritikal (power rail, komunikasi) guna mempermudah debugging setelah board jadi.
- **Orientasi konektor seragam**: memudahkan proses assembly dan mengurangi human error saat pemasangan kabel.
- **Revisi & versioning**: selalu simpan nomor versi board di silkscreen (mis. "REV A", "REV B") untuk melacak iterasi desain.

---

## 13. Proses Fabrikasi & Assembly (PCBA)

Setelah file Gerber, drill file, BOM, dan pick-and-place selesai dibuat di KiCad, alur menuju board fisik umumnya:

1. **Upload file ke vendor fabrikasi** (mis. JLCPCB, PCBWay, Seeed Fusion) → vendor melakukan review otomatis/manual (DFM check).
2. **Fabrikasi bare PCB** — proses etching tembaga, laminasi, drilling, solder mask, silkscreen, surface finish.
3. **Quality control** oleh vendor (electrical test, visual inspection).
4. **Assembly (PCBA)** — jika menggunakan jasa assembly:
   - Vendor menempatkan komponen SMD sesuai file pick-and-place, lalu melewati **reflow oven**.
   - Komponen through-hole (jika ada) biasanya disolder manual atau via wave soldering.
5. **Functional testing** — pengujian board hasil rakitan sesuai spesifikasi yang direncanakan di tahap awal.

Jika assembly dilakukan mandiri (manual soldering), pastikan urutan solder: komponen dengan profil suhu tertinggi/paling sulit diakses terlebih dahulu, baru komponen yang lebih mudah (umumnya SMD kecil dulu, lalu komponen besar/through-hole).

---

## 14. Troubleshooting Umum

| Masalah | Kemungkinan Penyebab | Solusi |
|---|---|---|
| Board tidak menyala | Short pada power rail, polaritas komponen terbalik | Cek dengan multimeter (continuity test) sebelum power-on |
| MCU tidak terdeteksi programmer | Jalur SWD/UART salah wiring, crystal tidak berosilasi | Cek skematik vs datasheet pinout MCU |
| Noise pada sinyal analog | Ground plane terpotong, trace analog dekat trace digital berkecepatan tinggi | Pisahkan analog & digital ground, re-route |
| DRC error clearance | Trace/pad terlalu berdekatan | Perbesar clearance di board setup atau reposisi komponen |
| Komponen tidak muat secara fisik | Footprint salah/berbeda dari komponen asli yang dibeli | Verifikasi footprint dengan datasheet sebelum order |
| ERC memberi peringatan pin Input floating | Pin input tidak terhubung ke sumber sinyal apa pun | Sambungkan ke sumber sinyal, atau pasang No Connect Flag bila memang sengaja tidak dipakai |
| Silkscreen tidak terbaca setelah board jadi | Teks silkscreen menimpa pad/area solder | Reposisi teks silkscreen agar tidak tertutup solder mask/pad |

---

## 15. Glosarium Istilah

- **Symbol** — representasi visual/logis komponen elektronik (mis. resistor, kapasitor, IC) di lembar kerja skematik.
- **Label** — penanda teks pada wire agar dua jalur terhubung secara logis tanpa garis panjang.
- **Bus** — jalur tebal representasi kumpulan beberapa jalur sinyal/kabel.
- **Netlist** — daftar koneksi antar pin komponen hasil dari skematik, diekspor ke PCB.
- **Ratsnest** — garis penghubung sementara di PCB Editor yang menunjukkan koneksi belum di-routing.
- **Pitch** — jarak spasi standar antar pin/kaki komponen atau konektor.
- **Packaging/Konektor** — bentuk kemasan fisik standar komponen (mis. TO-92, SOT-23) yang menentukan luas area di PCB.
- **Footprint/Land Pattern** — pola pad fisik tempat kaki komponen disolder.
- **Through-Hole (THT)** — metode pemasangan komponen menembus lubang PCB, disolder dari sisi sebaliknya.
- **SMD (Surface Mount Device)** — metode pemasangan komponen ditempel langsung pada permukaan PCB.
- **F.Cu / B.Cu** — lapisan tembaga depan (Front Copper) dan belakang (Back/Bottom Copper).
- **Masking (Solder Mask)** — lapisan isolator pelindung (umumnya hijau) yang menutupi jalur tembaga.
- **Silk (Silkscreen)** — lapisan tinta (umumnya putih) untuk teks informasi, nama komponen, dan indikator polaritas.
- **Vias** — lubang berlapis tembaga penghubung antar layer PCB (F.Cu ↔ B.Cu).
- **Pad** — area tembaga terbuka tempat kaki komponen disolder.
- **Trace** — jalur tembaga penghubung antar komponen di PCB.
- **Clearance** — jarak minimum aman antar elemen tembaga berbeda untuk mencegah short circuit.
- **ERC (Electrical Rule Check)** — pengecekan aturan elektris pada skematik.
- **DRC (Design Rule Check)** — pengecekan aturan fisik pada layout PCB.
- **Gerber File** — format file standar industri untuk fabrikasi PCB.
- **BOM (Bill of Materials)** — daftar lengkap komponen suatu board.
- **Pick and Place File** — file berisi posisi & orientasi komponen untuk mesin assembly otomatis.
- **Copper Pour/Zone** — area tembaga solid yang biasanya digunakan sebagai ground/power plane.
- **Reflow Soldering** — proses solder komponen SMD menggunakan panas oven.

---

*Technical note ini dapat dikembangkan lebih lanjut sesuai kebutuhan project spesifik (mis. penambahan bagian simulasi SPICE di KiCad, high-speed design rules, atau checklist DFM per vendor fabrikasi).*
