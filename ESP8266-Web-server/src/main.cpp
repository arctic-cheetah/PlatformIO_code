#include <Arduino.h>
/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

// Load Wi-Fi library
#include <ESP8266WiFi.h>

// Replace with your network credentials
const char* ssid     = "R-Link";
const char* password = "w7kehdwr2n1";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;
String html = "";

// Auxiliary variables to store the current output state
String output5State = "off";
String output4State = "off";
String output14State = "off";

// Assign output variables to GPIO pins
const int output5 = 5;
const int output4 = 4;
const int output14 = 14;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
#define TIMEOUT 2000
////////////////////////////////////////////////////////
//Function declarations
void send_html(WiFiClient client);

////////////////////////////////////////////////////////

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(output5, OUTPUT);
  pinMode(output4, OUTPUT);
  pinMode(output14, OUTPUT);
  // Set outputs to LOW
  digitalWrite(output5, LOW);
  digitalWrite(output4, LOW);
  digitalWrite(output14, LOW);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    bool end_connection = false;
    // loop while the client's connected
    // Timeout is needed to prevent client from waiting too long
    while (client.connected() && !end_connection && (currentTime - previousTime <= TIMEOUT)) { 
      
      currentTime = millis();         
      if (client.available()) {
        // read and print bytes from the client             
        char c = client.read();             
        Serial.print(c);                    
        header += c;
        if (c == '\n') {                    
          // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            send_html(client);
            end_connection = true;
          } 
          else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
  //To Do: Strobe LED

}

////////////////////////////////////////////////////////
//functions

//Send data to the client
void send_html(WiFiClient client) {

  // IMPORTANT: this is a response code to the client
  // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
  // and a content-type so the client knows what's coming, then a blank line:
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println("Connection: close");
  client.println();
  
  // turns the GPIOs on and off
  if (header.indexOf("GET /5/on") >= 0) {
    Serial.println("GPIO 5 on");
    output5State = "on";
    digitalWrite(output5, HIGH);
  } else if (header.indexOf("GET /5/off") >= 0) {
    Serial.println("GPIO 5 off");
    output5State = "off";
    digitalWrite(output5, LOW);
  } else if (header.indexOf("GET /4/on") >= 0) {
    Serial.println("GPIO 4 on");
    output4State = "on";
    digitalWrite(output4, HIGH);
  } else if (header.indexOf("GET /4/off") >= 0) {
    Serial.println("GPIO 4 off");
    output4State = "off";
    digitalWrite(output4, LOW);
  } else if (header.indexOf("GET /14/on") >= 0) {
    Serial.println("GPIO 14 on");
    output14State = "on";
    digitalWrite(output14, HIGH);
  } else if (header.indexOf("GET /14/off") >= 0) {
    Serial.println("GPIO 14 off");
    output14State = "off";
    digitalWrite(output14, LOW);
  }

  
  // Display the HTML web page
  html += "<!DOCTYPE html><html>";
  html += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<link rel=\"icon\" href=\"data:,\">";
  // CSS to style the on/off buttons 
  // Feel free to change the background-color and font-size attributes to fit your preferences
  html += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}";
  html += ".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}";
  html += ".button:hover {background-color: #358ca0;}.button2 {background-color: #4b5355;}.button2:hover {background-color: #77878A;}";
  html += "</style></head>";
  
  // Web Page Heading
  html += "<body><h1>ESP8266 Web Server</h1>";
  
  // Display current state, and ON/OFF buttons for GPIO 5  
  html += "<p>GPIO 5(D1) - State " + output5State + "</p>";
  // If the output5State is off, it displays the ON button       
  if (output5State=="off") {
    html += "<p><a href=\"/5/on\"><button class=\"button\">ON</button></a></p>";
  } else {
    html += "<p><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></p>";
  } 
      
  // Display current state, and ON/OFF buttons for GPIO 4  
  html += "<p>GPIO 4(D2) - State " + output4State + "</p>";
  // If the output4State is off, it displays the ON button       
  if (output4State=="off") {
    html += "<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>";
  } else {
    html += "<p><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p>";
  }

  //Display current state of D5 (GPIO 14)
  html += "<p>GPIO 14(D5) - State " + output14State + "</p>";
  // If the output4State is off, it displays the ON button  
  if (output14State == "off") {
    html += "<p><a href=\"/14/on\"><button class=\"button\">ON</button></a></p>";
  }
  else {
    html += "<p><a href=\"/14/off\"><button class=\"button\">OFF</button></a></p>";
  }

  //html += "<p>GPIO 4(D2) - Strobing off</p><p><a href=\"flash/off\"><button class=\"button button2\">OFF</button></a></p>";
  html += "</body></html>";

  client.println(html);
  // The HTTP response ends with another blank line
  client.println();
  html="";
}
