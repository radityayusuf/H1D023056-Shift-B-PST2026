#include <Arduino_FreeRTOS.h>

// Deklarasi prototip fungsi task
void TaskBlink1(void *pvParameters);
void TaskBlink2(void *pvParameters);
void Taskprint(void *pvParameters);

void setup()
{
    // Inisialisasi komunikasi serial pada kecepatan 9600 bps
    Serial.begin(9600);

    // Pembuatan Task 1 untuk LED Merah
    xTaskCreate(
        TaskBlink1, "task1", 128, NULL, 1, NULL);

    // Pembuatan Task 2 untuk LED Kuning
    xTaskCreate(
        TaskBlink2, "task2", 128, NULL, 1, NULL);

    // Pembuatan Task 3 untuk Counter Serial
    xTaskCreate(
        Taskprint, "task3", 128, NULL, 1, NULL);

    // Memulai Scheduler FreeRTOS
    vTaskStartScheduler();
}

void loop()
{
    // Dibiarkan kosong karena kendali dialihkan ke scheduler kernel RTOS
}

void TaskBlink1(void *pvParameters)
{
    pinMode(8, OUTPUT); // Mengontrol pin digital 8
    while (1)
    {
        Serial.println("Task1");
        digitalWrite(8, HIGH);
        vTaskDelay(200 / portTICK_PERIOD_MS);
        digitalWrite(8, LOW);
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}

void TaskBlink2(void *pvParameters)
{
    pinMode(7, OUTPUT); // Mengontrol pin digital 7
    while (1)
    {
        Serial.println("Task2");
        digitalWrite(7, HIGH);
        vTaskDelay(300 / portTICK_PERIOD_MS);
        digitalWrite(7, LOW);
        vTaskDelay(300 / portTICK_PERIOD_MS);
    }
}

void Taskprint(void *pvParameters)
{
    int counter = 0;
    while (1)
    {
        counter++;
        Serial.println(counter);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}