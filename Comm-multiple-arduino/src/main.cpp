#include <Arduino.h>
#include "SoftwareSerial.h"

SoftwareSerial TX_arduino(2, 3); //RX, TX

#define myTX 3
#define myRX 2

void setup() {
    Serial.begin(9600);
    pinMode(myTX, OUTPUT);
    pinMode(myRX, INPUT);
    TX_arduino.begin(9600);
}

void loop() {
    //Send Hi via TX
    TX_arduino.print("Hello World!\n");
    delay(1000);
}