# Jawaban Pertanyaan Praktikum Modul 4: ADC dan DAC

## Bagian 4.5.4: Analog to Digital Converter (ADC)

**1. Apa fungsi perintah analogRead() pada rangkaian praktikum ini?**
[cite_start]Fungsi `analogRead()` digunakan untuk membaca besaran tegangan analog murni yang dikirimkan oleh potensiometer ke pin A0, kemudian mengonversinya (kuantisasi) menjadi nilai digital diskret yang dapat dipahami oleh mikrokontroler Arduino [cite: 450-453, 609]. [cite_start]Rentang nilai digital yang dihasilkan adalah dari 0 (mewakili 0 Volt) hingga 1023 (mewakili 5 Volt) karena Arduino Uno menggunakan ADC beresolusi 10-bit[cite: 534, 1075].

**2. Mengapa diperlukan fungsi map() dalam program tersebut?**
[cite_start]Fungsi `map()` sangat diperlukan untuk men-skala ulang (konversi proporsional) rentang data input agar sesuai dengan batas rentang aktuator output [cite: 538-542]. [cite_start]Pada kasus ini, pembacaan ADC dari potensiometer memiliki rentang 0-1023, sedangkan motor servo SG90 secara fisik hanya bisa berputar dari sudut 0 hingga 180 derajat [cite: 538-541, 991]. Fungsi `map()` secara otomatis menerjemahkan nilai ADC ke sudut yang proporsional sehingga motor tidak menerima perintah pergerakan yang melampaui batas mekanisnya.

**3. Modifikasi Program Servo (Rentang 30° - 150°)**

_Source Code `modul4_adc_modifikasi.ino`:_

```cpp
#include <Servo.h>

Servo myservo;

const int potensioPin = A0;
const int servoPin = 9;

int pos = 0;
int val = 0;

void setup() {
  myservo.attach(servoPin);
  Serial.begin(9600);
}

void loop() {
  val = analogRead(potensioPin);

  // Modifikasi rentang pemetaan sudut di sini
  pos = map(val, 0, 1023, 30, 150);

  myservo.write(pos);

  Serial.print("ADC Potensio: ");
  Serial.print(val);
  Serial.print(" | Sudut Servo: ");
  Serial.println(pos);

  delay(100);
}
```

Penjelasan Baris per Baris Program:
#include <Servo.h> : Memasukkan library bawaan Arduino untuk mengontrol motor servo.
Servo myservo; : Membuat sebuah objek dari library Servo dengan nama myservo.
const int potensioPin = A0; : Menetapkan pin A0 sebagai jalur masuk data analog dari potensiometer. const int servoPin = 9; : Menetapkan pin digital 9 (yang mendukung PWM) sebagai jalur sinyal kendali untuk motor servo.
int pos = 0; : Mendeklarasikan variabel pos untuk menyimpan sudut servo (nilai awal 0).
int val = 0; : Mendeklarasikan variabel val untuk menyimpan hasil pembacaan ADC potensiometer (nilai awal 0).
void setup() { : Fungsi pengaturan awal yang dijalankan sekali oleh mikrokontroler.
myservo.attach(servoPin); : Menghubungkan objek myservo ke pin 9 agar sinyal PWM dapat dikirim ke aktuator.
Serial.begin(9600); : Mengaktifkan komunikasi serial ke komputer dengan kecepatan 9600 bps untuk proses monitoring.
void loop() { : Fungsi utama yang akan terus diulang (siklus program).
val = analogRead(potensioPin); : Membaca nilai dari pin A0 (0 hingga 1023) dan menyimpannya di variabel val .
pos = map(val, 0, 1023, 30, 150); : (Baris Modifikasi) Menerjemahkan nilai val (0-1023) menjadi nilai sudut secara linier dengan batas minimum 30 derajat dan maksimum 150 derajat .
myservo.write(pos); : Memerintahkan servo berputar menuju sudut yang telah dihitung dan disimpan dalam pos.
Serial.print("ADC Potensio: "); hingga Serial.
println(pos); : Mengirim teks dan nilai pemrosesan data (ADC dan Sudut) untuk ditampilkan di layar Serial Monitor secara real-time .
delay(100); : Memberikan waktu jeda 100 milidetik pada sistem untuk memberikan kesempatan bagi komponen mekanis servo mencapai posisi targetnya secara stabil .

Bagian 4.6.4: Digital to Analog Converter (PWM)

1.Jelaskan mengapa LED dapat diatur kecerahannya menggunakan fungsi analogWrite()!
Fungsi analogWrite() menggunakan teknik Pulse Width Modulation (PWM) yang secara periodik menghidupkan dan mematikan sinyal digital dalam kecepatan tinggi . Dengan mengubah durasi keadaan HIGH dan LOW (duty cycle), fungsi ini memanipulasi tegangan rata-rata yang diterima oleh komponen . Pada LED, perubahan tegangan rata-rata ini ditangkap oleh indera penglihatan manusia sebagai redup atau terangnya cahaya.

2.Apa hubungan antara nilai ADC (0-1023) dan nilai PWM (0-255)?
Nilai ADC (0-1023) merupakan hasil pembacaan input dengan resolusi 10-bit, sedangkan nilai PWM (0-255) adalah resolusi sinyal output 8-bit yang dihasilkan oleh Arduino Uno. Hubungannya adalah skalabilitas; agar data input dapat mengendalikan kecerahan lampu secara akurat dan linier, nilai ADC tersebut harus dikonversi (misalnya dengan dibagi 4 atau diproses menggunakan fungsi map()) sehingga sesuai dengan rentang maksimal output PWM .

3.Modifikasi Program Kecerahan LED (Rentang PWM 50 - 200)Source Code modul4_pwm_modifikasi.ino:

#include <Arduino.h>

const int potPin = A0;  
const int ledPin = 9;

int nilaiADC = 0;  
int pwm = 0;

void setup() {
pinMode(ledPin, OUTPUT);
Serial.begin(9600);
}

void loop() {
nilaiADC = analogRead(potPin);

// Modifikasi batas PWM menjadi 50 hingga 200 di sini
pwm = map(nilaiADC, 0, 1023, 50, 200);

analogWrite(ledPin, pwm);

Serial.print("ADC: ");
Serial.print(nilaiADC);
Serial.print(" | PWM: ");
Serial.println(pwm);

delay(50);
}
Penjelasan Baris per Baris Program:
#include <Arduino.h> : Memanggil kerangka library dasar milik Arduino.
const int potPin = A0; : Menetapkan pin analog A0 sebagai input dari potensiometer .
const int ledPin = 9; : Menetapkan pin digital 9 sebagai output PWM untuk LED .
int nilaiADC = 0; : Mendeklarasikan variabel nilaiADC dengan nilai awal 0 untuk menampung data sensor.
int pwm = 0; : Mendeklarasikan variabel pwm dengan nilai awal 0 untuk menyimpan data konversi kecerahan .
void setup() { : Fungsi penyiapan konfigurasi dasar saat alat pertama kali berjalan.
pinMode(ledPin, OUTPUT); : Mengonfigurasi fungsi ledPin (pin 9) agar bekerja sebagai jalur OUTPUT sinyal listrik ke LED.
Serial.begin(9600); : Menginisialisasi modul komunikasi serial komputer pada kecepatan baud rate 9600.
void loop() { : Fungsi program utama yang akan terus dieksekusi berulang-ulang tanpa henti.
nilaiADC = analogRead(potPin); : Memerintahkan mikrokontroler membaca sinyal analog di pin A0 dan menyimpannya sebagai angka (0-1023).
pwm = map(nilaiADC, 0, 1023, 50, 200); : (Baris Modifikasi) Mengubah skala nilai nilaiADC (0-1023) menjadi batas nilai PWM kecerahan "sedang" secara proporsional.Batas redup terendah diatur di angka 50 dan paling terang dibatasi pada angka 200 .
analogWrite(ledPin, pwm); : Memerintahkan pengiriman sinyal PWM (pulsa duty cycle) ke pin 9 sebesar nilai variabel pwm yang sudah dimodifikasi.
Serial.print("ADC: "); hingga Serial.println(pwm); : Rangkaian instruksi untuk mencetak nilai input (ADC) dan output kecerahan (PWM) ke layar monitor komputer .
delay(50); : Menghentikan program sejenak selama 50 milidetik pada akhir iterasi, bertujuan menjaga stabilitas pembacaan sensor dan kecepatan tampilan data agar tidak terlalu cepat di layar .
