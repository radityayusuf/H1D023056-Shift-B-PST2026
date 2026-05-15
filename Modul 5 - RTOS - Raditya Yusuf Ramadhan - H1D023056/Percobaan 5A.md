# Percobaan 5A: Multitasking Terjadwal dengan Kontrol Input Potensiometer

Aplikasi ini merupakan modifikasi dari proyek _multitasking baseline_ pada Modul V Pemrograman Sistem Tertanam. Program ini mendemonstrasikan kemampuan kernel **FreeRTOS** dalam mengelola tiga tugas (_tasks_) secara konkuren pada arsitektur prosesor tunggal (_single-core_ ATmega328P) dengan mengintegrasikan komponen input analog berupa potensiometer untuk mengendalikan parameter waktu secara dinamis.

## 👤 Identitas Praktikan

- **Nama Lengkap:** Raditya Yusuf Ramadhan
- **NIM:** H1D023056
- **Matakuliah:** Praktikum Pemrograman Sistem Tertanam (Modul V - RTOS)

---

## 💻 Kode Program Hasil Modifikasi (`modul5_multitasking_modified.ino`)

```c
#include <Arduino_FreeRTOS.h>

// Deklarasi prototip fungsi task
void TaskBlink1( void *pvParameters );
void TaskBlink2( void *pvParameters );
void Taskprint( void *pvParameters );

// Variabel global untuk menyimpan delay dinamis dari potensiometer
int dynamicDelay = 200;

void setup() {
  // Inisialisasi komunikasi serial pada kecepatan 9600 bps
  Serial.begin(9600);

  // Registrasi pembuatan task ke scheduler FreeRTOS
  xTaskCreate(TaskBlink1, "task1", 128, NULL, 1, NULL);
  xTaskCreate(TaskBlink2, "task2", 128, NULL, 1, NULL);
  xTaskCreate(Taskprint, "task3", 128, NULL, 1, NULL);

  // Memulai Scheduler FreeRTOS
  vTaskStartScheduler();
}

void loop() {
  // Dibiarkan kosong karena kendali dialihkan sepenuhnya ke kernel RTOS
}

void TaskBlink1(void *pvParameters) {
  // Troubleshooting Mismatch Pin: Menggunakan Pin D10 untuk LED Merah sesuai hardware
  pinMode(10, OUTPUT);

  while(1) {
    // Membaca nilai potensiometer pada PIN Analog A0
    int potValue = analogRead(A0);

    // Memetakan nilai ADC (0-1023) menjadi durasi delay dinamis (100ms - 1000ms)
    dynamicDelay = map(potValue, 0, 1023, 100, 1000);

    Serial.print("Task1 - Delay Dinamis: ");
    Serial.print(dynamicDelay);
    Serial.println(" ms");

    digitalWrite(10, HIGH);
    vTaskDelay( dynamicDelay / portTICK_PERIOD_MS );
    digitalWrite(10, LOW);
    vTaskDelay( dynamicDelay / portTICK_PERIOD_MS );
  }
}

void TaskBlink2(void *pvParameters) {
  // Troubleshooting Mismatch Pin: Menggunakan Pin D8 untuk LED Kuning sesuai hardware
  pinMode(8, OUTPUT);

  while(1) {
    Serial.println("Task2 - LED Kuning Berkedip Konstan");
    digitalWrite(8, HIGH);
    vTaskDelay( 300 / portTICK_PERIOD_MS );
    digitalWrite(8, LOW);
    vTaskDelay( 300 / portTICK_PERIOD_MS );
  }
}

void Taskprint(void *pvParameters) {
  int counter = 0;
  while(1) {
    counter++;
    Serial.print("Task3 - Nilai Counter Serial: ");
    Serial.println(counter);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}
```

🔍 Jawaban Pertanyaan & Analisis Praktikum 5A

1. Mekanisme Berjalannya Tugas (Simultan vs Bergantian)
   Secara arsitektur fisik perangkat keras, ketiga task berjalan secara bergantian (interleaved), bukan simultan sejati. Hal ini dikarenakan mikrokontroler ATmega328P pada Arduino Uno hanya memiliki inti prosesor tunggal (single-core) yang secara fisik hanya mampu mengeksekusi satu instruksi mesin pada satu waktu detak clock. Namun, karena kernel FreeRTOS menerapkan teknik penjadwalan pembagian waktu (time-slicing scheduler) berbasis interupsi timer internal (kernel tick), CPU melakukan perpindahan konteks tugas (context switching) antar-tugas dalam skala milidetik. Kecepatan pemindahan yang sangat tinggi ini memberikan kesan bagi manusia bahwa ketiga tugas tersebut berjalan secara bersamaan (concurrent multitasking).

2. Prosedur Menambahkan Task Keempat
   Untuk menambahkan tugas keempat, terdapat 3 langkah sintaksis prosedural yang wajib dipenuhi:

Deklarasi Prototip Fungsi: Menambahkan deklarasi di bagian atas file kode sebelum blok inisialisasi awal. Contoh: void TaskSensor4(void \*pvParameters);.

Instansiasi Tugas pada setup(): Memanggil fungsi pembentukan objek kernel xTaskCreate() di dalam fungsi setup(). Contoh: xTaskCreate(TaskSensor4, "task4", 128, NULL, 1, NULL);.

Implementasi Badan Fungsi: Menuliskan isi instruksi kerja tugas keempat dengan menggunakan struktur perulangan tak terbatas (infinite loop) serta instruksi penundaan non-blocking vTaskDelay() agar tidak memonopoli CPU secara sepihak.

3. Hasil Pengamatan Modifikasi Potensiometer
   Hasil modifikasi menunjukkan bahwa ketika tuas potensiometer diputar mendekati hambatan minimum (A0 menuju 0), durasi pemetaan delay mengecil mendekati 100 ms sehingga LED Merah berkedip sangat cepat. Sebaliknya, saat potensiometer diputar penuh ke kanan (A0 menuju 1023), jeda waktu memanjang hingga 1000 ms (1 detik) sehingga kedipan melambat.

Analisis ini membuktikan sifat non-blocking dari fungsi vTaskDelay(). Ketika TaskBlink1 memasuki kondisi tertunda, tugas tersebut dipindahkan ke status Blocked State oleh kernel penjadwal, sehingga siklus pemrosesan CPU segera dialokasikan untuk mengerjakan tugas lain yang berstatus Ready State. Efeknya, fluktuasi perubahan kecepatan pada LED Merah sama sekali tidak mengganggu atau memperlambat ketepatan waktu kedipan konstan LED Kuning (TaskBlink2) maupun kelancaran pembacaan hitungan pada tugas Taskprint.

⚠️ Catatan Analisis & Troubleshooting Pin Mismatch: > Terjadi ketidaksesuaian (mismatch) teknis antara pemetaan pin pada file program bawaan modul (menggunakan pin 8 dan 7) dengan Tabel 4.1 petunjuk rangkaian fisik sikit (LED Merah ke pin D10 dan LED Kuning ke pin D8). Kode modifikasi di atas telah diperbaiki secara analitis dengan menyelaraskan instruksi output digital langsung ke Pin D10 dan Pin D8 agar fungsionalitas sirkuit hardware berjalan sinkron dengan program.
