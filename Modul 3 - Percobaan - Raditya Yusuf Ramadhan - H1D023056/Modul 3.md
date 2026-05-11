# Jawaban Pertanyaan Praktikum Modul 3: Protokol Komunikasi

## Bagian 3.5.4: Komunikasi Serial (UART)

**1. Jelaskan proses dari input keyboard hingga LED menyala/mati!**

- **Input:** Pengguna mengetikkan karakter ('1' atau '0') di _keyboard_ dan menekan _Enter_ pada antarmuka Serial Monitor di komputer.
- **Transmisi:** Komputer mengubah karakter tersebut menjadi kode ASCII dan mengirimkannya dalam bentuk sinyal digital melalui kabel USB (protokol UART) ke pin RX (Receive) pada Arduino.
- **Pemrosesan:** Arduino menerima data tersebut dan menyimpannya di _serial buffer_. Perintah `Serial.read()` mengambil karakter tersebut dari _buffer_.
- **Eksekusi:** Struktur logika `if-else` mengevaluasi karakter. Jika data adalah '1', Arduino mengirimkan tegangan 5V (HIGH) ke pin 12 sehingga LED menyala. Jika '0', tegangan diturunkan ke 0V (LOW) sehingga LED mati.

**2. Mengapa digunakan Serial.available() sebelum membaca data? Apa yang terjadi jika baris tersebut dihilangkan?**
Fungsi `Serial.available()` digunakan untuk mengecek apakah ada byte data yang sudah masuk dan siap dibaca di dalam _buffer_ penerima. Jika baris ini dihilangkan, Arduino akan terus-menerus mengeksekusi `Serial.read()` pada setiap siklus `loop()` meskipun komputer tidak mengirimkan data apa pun. Hal ini akan mengembalikan nilai `-1` (atau 255/karakter kosong), yang menyebabkan program memproses "sampah" secara terus-menerus dan membuang siklus kerja CPU secara percuma.

**3. Modifikasi program agar LED berkedip (blink) ketika menerima input '2'**

_Source Code Modifikasi (`modul3A_modifikasi.ino`):_

```cpp
const int PIN_LED = 12;
char currentMode = '0';
unsigned long previousMillis = 0;
const long interval = 500;
bool ledState = LOW;

void setup() {
  Serial.begin(9600);
  Serial.println("Ketik '1'(ON), '0'(OFF), atau '2'(BLINK)");
  pinMode(PIN_LED, OUTPUT);
}

void loop() {
  // Mengecek apakah ada instruksi masuk dari Serial Monitor
  if (Serial.available() > 0) {
    char data = Serial.read();

    if (data == '1' || data == '0' || data == '2') {
      currentMode = data; // Perbarui mode sesuai input

      if (currentMode == '1') {
        digitalWrite(PIN_LED, HIGH);
        Serial.println("LED ON");
      }
      else if (currentMode == '0') {
        digitalWrite(PIN_LED, LOW);
        Serial.println("LED OFF");
      }
      else if (currentMode == '2') {
        Serial.println("LED BLINKING (Mode 2 Aktif)");
      }
    }
    else if (data != '\n' && data != '\r') {
      Serial.println("Perintah tidak dikenal");
    }
  }

  // Logika BLINK tanpa memblokir pembacaan serial
  if (currentMode == '2') {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      ledState = !ledState;
      digitalWrite(PIN_LED, ledState);
    }
  }
}
```

Penjelasan Baris Kode Modifikasi:

char currentMode = '0'; : Menyimpan status mode aktif saat ini (diawali dengan mode OFF).

unsigned long previousMillis = 0; : Variabel untuk menyimpan catatan waktu terakhir kali LED berubah status (menggunakan tipe unsigned long untuk mencegah overflow memori waktu).

const long interval = 500; : Menetapkan jeda kedipan LED sebesar 500 milidetik.

bool ledState = LOW; : Mengingat kondisi LED saat ini (mati atau menyala).

void loop() { ... } : Jika ada data serial (Serial.available() > 0), program akan membacanya dan memperbarui nilai currentMode.

if (currentMode == '2') { : Mengeksekusi blok ini jika pengguna sudah mengirimkan angka '2'.

unsigned long currentMillis = millis(); : Mengambil waktu (dalam milidetik) sejak Arduino pertama kali dihidupkan.

if (currentMillis - previousMillis >= interval) : Mengevaluasi apakah selisih waktu saat ini dengan waktu terakhir LED berubah sudah mencapai 500ms. Jika iya, maka status ledState akan dibalik (!ledState) dan dikirimkan ke pin LED.

4. Tentukan apakah menggunakan delay() atau millis()! Jelaskan pengaruhnya terhadap sistem
   Untuk memodifikasi LED agar berkedip (Mode '2') sembari tetap bisa menerima input mode lain ('0' atau '1') tanpa delay, kita wajib menggunakan millis().
   Pengaruhnya sangat besar: fungsi delay() bersifat blocking (menghentikan seluruh aktivitas mikrokontroler selama waktu jeda), sehingga Arduino akan "tuli" dan tidak bisa membaca input Serial baru saat LED sedang dalam fase jeda. Sebaliknya, millis() bersifat non-blocking, memungkinkan mikrokontroler mengeksekusi instruksi pembacaan Serial secara paralel sambil terus melacak waktu kedipan LED di latar belakang.

   Bagian 3.6.4: Inter-Integrated Circuit (I2C)

1.Jelaskan bagaimana cara kerja komunikasi I2C antara Arduino dan LCD pada rangkaian tersebut!
Komunikasi I2C menggunakan arsitektur Master-Slave. Arduino bertindak sebagai Master dan LCD sebagai Slave. Komunikasi ini hanya menggunakan dua kabel: SDA (Serial Data pada pin A4) untuk transfer data dua arah, dan SCL (Serial Clock pada pin A5) untuk sinkronisasi waktu transmisi.
Arduino pertama-tama mengirimkan sinyal clock dan memanggil alamat unik LCD (misalnya 0x27). Setelah LCD mengonfirmasi alamat tersebut, Arduino mulai mengirimkan paket data karakter teks atau instruksi posisi kursor melalui jalur SDA. LCD menerima data tersebut dan menampilkannya di layar.

2.Apakah pin potensiometer harus seperti itu? Jelaskan yang terjadi apabila pin kiri dan pin kanan tertukar!
Pin tengah potensiometer wajib terhubung ke pin Analog (A0) sebagai output pembacaan. Namun, pin kiri dan kanan (VCC dan GND) bebas ditukar.
Potensiometer bekerja sebagai pembagi tegangan (voltage divider). Apabila pin VCC 5V dan GND tertukar, yang terjadi hanyalah arah logikanya yang terbalik. Misalnya, jika awalnya diputar ke kanan penuh bernilai 1023 dan ke kiri penuh bernilai 0, maka jika kabel VCC dan GND ditukar, putar ke kanan penuh akan menjadi 0 dan putar ke kiri penuh akan menjadi 1023. Rangkaian tidak akan rusak atau mengalami short circuit.

3.Modifikasi program dengan menggabungkan antara UART dan I2C

Source Code Modifikasi (modul3B_modifikasi.ino):
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
const int pinPot = A0;

void setup() {
Serial.begin(9600);
lcd.init();
lcd.backlight();
}

void loop() {
int nilai = analogRead(pinPot);

// Proses perhitungan matematika
int panjangBar = map(nilai, 0, 1023, 0, 16);
int persen = map(nilai, 0, 1023, 0, 100);
float volt = (nilai / 1023.0) \* 5.0;

// 1. Output ke Serial Monitor (UART)
Serial.print("ADC: ");
Serial.print(nilai);
Serial.print("\tVolt: ");
Serial.print(volt);
Serial.print(" V\tPersen: ");
Serial.print(persen);
Serial.println("%");

// 2. Output ke LCD (I2C)
lcd.setCursor(0, 0);
lcd.print("ADC:");
lcd.print(nilai);
lcd.print(" ");
lcd.print(persen);
lcd.print("% "); // Spasi ekstra untuk membersihkan sisa karakter lama

lcd.setCursor(0, 1);
for (int i = 0; i < 16; i++) {
if (i < panjangBar) {
lcd.print((char)255); // Cetak blok hitam padat
} else {
lcd.print(" "); // Cetak spasi kosong
}
}

delay(500); // Penahan refresh rate agar mudah dibaca
}

Penjelasan Baris Kode Modifikasi:

int panjangBar = map(...); : Memetakan nilai ADC (0-1023) ke jumlah kolom maksimal pada LCD (0-16) untuk menggambar grafik bar.

int persen = map(nilai, 0, 1023, 0, 100); : Mengubah rentang 0-1023 menjadi skala persentase 0-100%.

float volt = (nilai / 1023.0) \* 5.0; : Menghitung tegangan murni. Karena 1023 mewakili 5V, nilai ADC dibagi 1023 lalu dikali 5 (menggunakan tipe data float agar presisi desimalnya tidak hilang).

Blok Serial.print(...) : Arduino mengirimkan data teks secara serial (asynchronous) ke komputer menggunakan modul UART.

lcd.setCursor(0, 0); : Memerintahkan modul I2C untuk meletakkan kursor pada kolom ke-0 (paling kiri) di baris ke-0 (paling atas).

lcd.print("% "); : Tiga buah spasi ekstra di akhir perintah print berfungsi sangat vital untuk menimpa/menghapus karakter dari putaran sebelumnya.

Blok for (int i = 0; i < 16; i++) { ... } : Mencetak balok solid (char)255 sebanyak jumlah panjangBar. Jika iterasi kolom i sudah melampaui panjangBar, sisa layar LCD di baris tersebut akan ditimpa dengan spasi agar kembali bersih.

4.Lengkapi tabel berikut berdasarkan pengamatan pada Serial Monitor
ADC,Volt (V),Persen (%)
1,0.00 V,0 %
21,0.10 V,2 %
49,0.24 V,4 %
74,0.36 V,7 %
96,0.47 V,9 %
