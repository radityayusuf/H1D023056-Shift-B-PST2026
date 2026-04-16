1. Uraikan hasil tugas pada praktikum yang telah dilakukan pada setiap percobaan!

Hasil Percobaan 2A (Seven Segment): Praktikum berhasil dilakukan dengan merangkai Seven Segment pada breadboard dan menghubungkannya ke pin GPIO Arduino. Sistem berhasil menampilkan siklus karakter heksadesimal (0-F) secara berurutan. Karena perangkat keras yang digunakan bertipe Common Anode, penyesuaian dilakukan pada perangkat lunak dengan menyematkan operator logika negasi (!) pada fungsi pengirim sinyal agar matriks data biner terbaca dengan benar. Selain itu, program juga berhasil dimodifikasi untuk melakukan perhitungan mundur (dari F ke 0).

Hasil Percobaan 1B (Push Button): Praktikum ini berhasil mengintegrasikan masukan mekanis saklar Push Button dengan layar Seven Segment menjadi sebuah sistem counter terpadu. Dengan memanfaatkan fitur internal INPUT_PULLUP pada Arduino, sinyal saklar dapat dibaca dengan stabil. Sistem berhasil mendeteksi penekanan tombol (falling edge detection) untuk menambah (increment) angka pada layar tanpa terjadi loncatan angka liar berkat penambahan delay debounce. Modifikasi lanjutan dengan dua tombol (increment dan decrement) juga sukses dieksekusi lengkap dengan fitur siklus batas overflow/underflow (kembali ke 0 setelah 15, dan kembali ke 15 setelah 0).

2. Bagaimana prinsip kerja dari Seven Segment Display dalam menampilkan angka dan karakter?
   Prinsip dasar Seven Segment Display adalah mengendalikan 7 buah Light Emitting Diode (LED) individual (ditambah 1 LED dot) yang disusun membentuk pola menyerupai angka 8. Setiap segmen lampu (dilabeli a, b, c, d, e, f, dan g) terhubung secara paralel menuju pin digital mikrokontroler. Untuk membentuk sebuah angka atau karakter (misalnya angka '1'), mikrokontroler memancarkan sinyal digital secara bersamaan hanya pada pin tertentu (segmen 'b' dan 'c') agar menyala, sementara pin lainnya dimatikan. Konfigurasi nyala-mati LED yang saling berkolaborasi inilah yang secara visual membentuk rupa karakter tertentu. Pada tipe Common Anode seperti di percobaan ini, LED akan diaktifkan saat mikrokontroler mengirimkan logika tegangan rendah (LOW).

3. Jelaskan bagaimana sistem counter bekerja pada program tersebut!
   Sistem counter pada program ini bekerja dengan mengandalkan sebuah variabel penyimpan nilai (misal: currentDigit) dan instruksi pemantauan transisi kondisi tombol di dalam fungsi perulangan utama (loop).
   Prosesnya adalah sebagai berikut:

Mikrokontroler secara konstan membaca status tegangan pada pin Push Button menggunakan digitalRead(). Karena memakai pull-up, status normalnya adalah HIGH.

Ketika tombol ditekan, status tegangan seketika anjlok menjadi LOW. Program mendeteksi tepi turun (falling edge) ini dengan membandingkan status tombol saat ini dan status pada milidetik sebelumnya.

Jika syarat penekanan valid, mikrokontroler menjeda proses sesaat (delay) sebagai debounce untuk meredam guncangan mekanis agar tak terbaca ganda.

Variabel counter (currentDigit) kemudian ditambahkan (++) atau dikurangi (--) sesuai instruksi.

Program memiliki pengaman batas; jika nilai melewati ambang 15, ia di-reset ke 0, begitu pun sebaliknya. Terakhir, variabel yang baru diperbarui ini dipanggil ke fungsi tampilan untuk diterjemahkan menjadi wujud angka di Seven Segment.
