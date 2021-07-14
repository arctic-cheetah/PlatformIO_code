//A program to send temperature and humidity to the Thingspeak
//Using the ESP8266
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <DHT.h>
#include <Arduino.h>

//Wifi info
const char ssid[] = "R-Link";
const char pass_key[] = "w7kehdwr2n1";
#define THINGSPEAK_TIME_STEP 15000
unsigned long prev_time = 0;

//Thingspeak API
const char write_api[] = "KVLN5ULAJ5T1JIUP";
const long thing_speak_channel = 963270;

//DHT sensor info
#define DHT_PIN D1
#define DHT_TYPE 22 //DHT11

DHT dht(DHT_PIN, DHT_TYPE);
WiFiClient client;

//Miscellaneous
#define TIME_STEP 2000
#define SERIAL_OUTPUT 1

////////////////////////////////////////////////////////////
//Function declarations
void init_wifi();


//////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);
  Serial.print("\n\nInitialising...\n\n");
  pinMode(LED_BUILTIN, OUTPUT);
  dht.begin();

  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
}

void loop() {
  //Reconnect to the wifi if disconnected
  if (WiFi.status() != WL_CONNECTED) {
    init_wifi();
  }
  digitalWrite(LED_BUILTIN, LOW);
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float humid = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float temp = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(humid) || isnan(temp)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  #ifdef SERIAL_OUTPUT
  Serial.print(F("Humidity: "));
  Serial.print(humid);
  Serial.print(F("%  Temperature: "));
  Serial.print(temp);
  Serial.print(F("°C \n"));
  #endif

  //Thingspeak API has a 15sec cool down
  if (millis() - prev_time >= long(THINGSPEAK_TIME_STEP)) {
    ThingSpeak.setField(1, humid);
    ThingSpeak.setField(2, temp);
    int x = ThingSpeak.writeFields(thing_speak_channel, write_api);
    if (x == 200) {
      Serial.print("Channel update successful\n");
    }
    else {
      Serial.print("Channel update failed, error was: " + String(x) + "\n");
    }
    //Update the prev_time
    prev_time = millis();
  }
  
  delay(TIME_STEP);
  digitalWrite(LED_BUILTIN, HIGH);
}

/////////////////////////////////////////////////////////////////////////
//Functions
void init_wifi() {
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.print("...\n");
  WiFi.begin(ssid, pass_key);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting...\n");
    delay(1000);
  }
  Serial.print("Successfully connected...\n");
}