#include <Arduino.h>
#include "DHT.h"
#include <ESP8266WiFi.h>
#include "ThingSpeak.h"

#define READ_PIN D1
#define LED D5
#define DHT_TYPE DHT22
#define DEBUG true

DHT dht(READ_PIN, DHT_TYPE);
WiFiClient  client;
ThingSpeakClass ts;

String mySSID = "R-Link";
String myPWD = "w7kehdwr2n1";
const char myAPI[] = "KVLN5ULAJ5T1JIUP";
const String hostName = "Creeper's gotta win";
String status = "";
unsigned long channelID = 971825;
String field2 = "field2";

//This is a function for ESP communication
String espData(String command, const int timeout, boolean debug) {
    Serial.print("AT Command ==> ");
    Serial.print(command);
    Serial.println("     ");
    
    String response = "";
    Serial.println(command);
    long int time = millis();
    while ( (time + timeout) > millis())
    {
        while (Serial.available())
        {
        char c = Serial.read();
        response += c;
        }
    }
    if (DEBUG) {
        Serial.print(response);
    }
    return response;
}

//A function to blink the LED
void blink_led(void) {
    digitalWrite(LED, HIGH);
    delay(1000);
    digitalWrite(LED, LOW);
    delay(1000);
}

//Set up the Wifi
void init_WiFi() {
    if(WiFi.status() != WL_CONNECTED){
        Serial.print("Attempting to connect to mySSID: ");
        Serial.println(mySSID);
        while(WiFi.status() != WL_CONNECTED) {
            // Connect to WPA/WPA2 network. Change this line if using open or WEP network
            WiFi.begin(mySSID, myPWD); 
            Serial.print(".");
            delay(5000);     
        } 
        Serial.println("\nConnected.");
  }
}

//Send the data to thingspeak
void send_data(float h, float t) {
    ts.setField((unsigned int)1, h);
    ts.setField((unsigned int)2, t);
    ts.setStatus(status);
    int x = ts.writeFields(channelID, myAPI);
    if (x == 200) {
        Serial.println("Channel update successful.");
    }
    else {
        Serial.println("Problem updating channel. HTTP error code " + String(x));
    }

    delay(10000);
    blink_led();
}
/////////////////////////////////////////////////////////////////////////////////
//Main here:
void setup() {
    Serial.begin(115200);
    Serial.print("\n");
    Serial.print("\n");
    Serial.print("Spooling systems!\n");
    WiFi.hostname(hostName);

    ts.begin(client);

    pinMode(LED, OUTPUT);
    dht.begin();
}
void loop() {
    double h = dht.readHumidity();
    double t = dht.readTemperature();

    //Print to terminal
    if (DEBUG) {
        Serial.print("Humidity: ");
        Serial.print(h);
        Serial.print("   ");
        Serial.print("Temperature(C): ");
        Serial.print(t);
        Serial.print("\n");
    }

    //Send packet to thingspeak
    init_WiFi();
    send_data(h, t);
}