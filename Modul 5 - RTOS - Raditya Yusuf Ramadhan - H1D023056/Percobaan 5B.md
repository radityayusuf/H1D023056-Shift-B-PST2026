# Percobaan 5B: Komunikasi Antar-Tugas (Inter-Task Communication) Menggunakan Message Queue dan Sensor DHT Fisik

Aplikasi ini merupakan pengembangan dari proyek _task queue_ statis pada Modul V Pemrograman Sistem Tertanam. Program ini mengimplementasikan konsep arsitektur **Producer-Consumer** yang dinamis dengan memanfaatkan objek primitif **Message Queue** milik FreeRTOS untuk menyalurkan data telemetri fisik berupa suhu dan kelembapan dari sensor **DHT** secara aman tanpa risiko korupsi memori.

## 👤 Identitas Praktikan

- **Nama Lengkap:** Raditya Yusuf Ramadhan
- **NIM:** H1D023056
- **Matakuliah:** Praktikum Pemrograman Sistem Tertanam (Modul V - RTOS)

---

## 💻 Kode Program Hasil Modifikasi (`modul6_taskqueue_modified.ino`)

```c
#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include <DHT.h>

#define DHTPIN 2       // Pin DATA DHT terhubung ke PIN Digital 2 Arduino
#define DHTTYPE DHT11  // Menyesuaikan tipe jenis sensor DHT11 atau DHT22

DHT dht(DHTPIN, DHTTYPE);

// Struktur data terstruktur untuk mengemas informasi sensor
struct readings {
  int temp;
  int h;
};

// Handle objek antrean data kernel
QueueHandle_t my_queue;

void setup() {
  // Inisialisasi komunikasi serial dan sensor DHT fisik
  Serial.begin(9600);
  dht.begin();

  // Membuat task queue dengan panjang penampung 1 elemen ukuran struct readings
  my_queue = xQueueCreate(1, sizeof(struct readings));

  // Inisialisasi tugas Producer dan Consumer dengan prioritas setara
  xTaskCreate(read_data, "read sensors", 128, NULL, 1, NULL);
  xTaskCreate(display, "display", 128, NULL, 1, NULL);
}

void loop() {
  // Dibiarkan kosong karena sistem dikendalikan penuh oleh scheduler RTOS
}

void read_data(void *pvParameters) {
  struct readings x;
  for(;;) {
    // Mengambil parameter telemetri lingkungan fisik riil dari sensor DHT
    x.temp = (int)dht.readTemperature();
    x.h = (int)dht.readHumidity();

    // Validasi penanganan pembacaan data sensor kosong (Not a Number)
    if (!isnan(x.temp) && !isnan(x.h)) {
      // Mengirim data ke antrean, menunggu tanpa batas waktu jika antrean penuh
      xQueueSend(my_queue, &x, portMAX_DELAY);
    }

    // DHT idealnya disampling per 2 detik (2000 ms) agar stabil
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void display(void *pvParameters) {
  struct readings x;
  for(;;) {
    // Memantau antrean, tugas akan langsung Blocked jika antrean kosong
    if(xQueueReceive(my_queue, &x, portMAX_DELAY) == pdPASS) {
      Serial.println("--- TELEMETRI DINAMIS DATA SENSOR DHT ---");
      Serial.print("Temperatur Suhu : ");
      Serial.print(x.temp);
      Serial.println(" *C");
      Serial.print("Kelembapan Udara: ");
      Serial.print(x.h);
      Serial.println(" %");
      Serial.println("----------------------------------------");
    }
  }
}
```

🔍 Jawaban Pertanyaan & Analisis Praktikum 5B

1. Mekanisme Eksekusi Aliran Data Tugas
   Kedua tugas beroperasi secara asinkron namun terikat secara sinkron di dalam pipa saluran penampung data (data pipeline). Mekanisme ini dikendalikan oleh perpindahan status kesiapan tugas (task states). Saat sistem diaktifkan, Task display (Consumer) akan langsung dimasukkan ke dalam status Blocked State oleh kernel karena antrean data masih kosong. Siklus CPU dialokasikan penuh untuk menjalankan Task read_data (Producer). Begitu tugas produsen selesai menginterogasi sensor DHT dan memasukkan data struktur ke antrean lewat perintah xQueueSend(), penjadwal kernel secara otomatis mendeteksi perubahan tersebut dan mengalihkan status consumer menjadi Ready untuk memproses ekstraksi pesan. Setelah isi pesan dicetak ke saluran serial, consumer otomatis tertidur kembali.

2. Analisis Potensi Terjadinya Race Condition
   Program komunikasi tugas ini dijamin tidak berpotensi mengalami kondisi balapan (race condition). Race condition hanya terjadi apabila terdapat dua tugas atau thread atau lebih yang mencoba untuk mengakses, memodifikasi, atau menulis ke satu alamat memori global secara bersamaan tanpa proteksi sinkronisasi. Pustaka FreeRTOS merancang objek QueueHandle_t secara bawaan aman dari thread (native thread-safe). Di dalam internal fungsi xQueueSend() dan xQueueReceive(), terdapat primitif penguncian kernel mutual eksklusif (internal critical sections) yang otomatis menonaktifkan interupsi penjadwalan lain selama pemindahan memori terjadi, sehingga integritas data terjamin penuh tanpa risiko data korup.

3. Hasil Pengamatan Modifikasi Sensor DHT Sesungguhnya
   Integrasi komponen sensor DHT fisik membuktikan bahwa pipa antrean pesan (message queue) mampu mengalirkan data parameter fisik lingkungan (suhu dan kelembapan) secara berkala, kontinu, dan akurat ke Serial Monitor. Struktur arsitektur ini sangat andal untuk memisahkan fungsi akuisisi data yang lambat (sensor DHT memerlukan waktu sampling sekitar 2 detik) dari fungsi visualisasi presentasi data, tanpa membebani performa stabilitas kerja mikrokontroler.

