//The child that will receive data from the parent
#include <SoftwareSerial.h>
#include <Arduino.h>
//Instatiate an object

SoftwareSerial RX_arduino(2, 3); //RX, TX

#define myTX 3
#define myRX 2

void setup() {
    Serial.begin(9600);

    //Pin 2 = input
    //Pin 3 = output
    pinMode(myRX, INPUT);
    pinMode(myTX, OUTPUT);
    RX_arduino.begin(9600);
    RX_arduino.flush();
}

void loop() {
    //Receive Hi via TX
    while (RX_arduino.available() > 0) {
        Serial.print(RX_arduino.readString());
    }
    tone();
    delay(500);
}