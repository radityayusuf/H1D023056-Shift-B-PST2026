#include <Arduino_FreeRTOS.h>
#include <queue.h>

// Struktur data terstruktur untuk menampung data sensor
struct readings
{
    int temp;
    int h;
};

// Deklarasi handle objek antrean data
QueueHandle_t my_queue;

void setup()
{
    Serial.begin(9600);

    // Membuat task queue dengan panjang 1 elemen ukuran objek struct readings
    my_queue = xQueueCreate(1, sizeof(struct readings));

    // Inisialisasi tugas Producer dan Consumer
    xTaskCreate(read_data, "read sensors", 128, NULL, 0, NULL);
    xTaskCreate(display, "display", 128, NULL, 0, NULL);
}

void loop() {}

void read_data(void *pvParameters)
{
    struct readings x;
    for (;;)
    {
        // Mengisi data tiruan secara konstan (statis)
        x.temp = 54;
        x.h = 30;

        // Mengirimkan salinan objek struktur ke antrean data
        xQueueSend(my_queue, &x, portMAX_DELAY);
        vTaskDelay(100);
    }
}

void display(void *pvParameters)
{
    struct readings x;
    for (;;)
    {
        // Memantau dan menarik data dari antrean begitu data tersedia
        if (xQueueReceive(my_queue, &x, portMAX_DELAY) == pdPASS)
        {
            Serial.print("temp = ");
            Serial.println(x.temp);
            Serial.print("humidity = ");
            Serial.println(x.h);
        }
    }
}