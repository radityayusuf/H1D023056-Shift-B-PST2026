#include <Servo.h>

// Inisialisasi pin
const int pinButton = 2;
const int pinLedHijau = 13;
const int pinLedMerah = 12;
const int pinServo = 9;

// Variabel status
Servo servoPintu;
int statusPintu = 0;
int buttonState = 0;
int lastButtonState = LOW;

void setup()
{
    pinMode(pinButton, INPUT);
    pinMode(pinLedHijau, OUTPUT);
    pinMode(pinLedMerah, OUTPUT);

    servoPintu.attach(pinServo);

    servoPintu.write(0);
    digitalWrite(pinLedMerah, HIGH);
    digitalWrite(pinLedHijau, LOW);
}

void loop()
{

    int reading = digitalRead(pinButton);

    if (reading == HIGH && lastButtonState == LOW)
    {
        if (statusPintu == 0)
        {
            // PROSES MEMBUKA PINTU
            servoPintu.write(90);
            digitalWrite(pinLedHijau, HIGH);
            digitalWrite(pinLedMerah, LOW);
            statusPintu = 1;
        }
        else
        {
        }

        delay(500);
    }

    lastButtonState = reading;
}