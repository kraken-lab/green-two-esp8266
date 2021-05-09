/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com  
*********/

// Load Wi-Fi library
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <random>

#include "LittleFS.h"

#include "SoftwareSerial.h"
#include "serial.h"

// Replace with your network credentials
const char* ssid     = "WLAN-BEF106_EXT";
const char* password = "cocacola";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output5State = "off";
String output4State = "off";

// Assign output variables to GPIO pins
const int output5 = 5;
const int output4 = 4;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

// Rx and Tx pins
const int _serialRx = 3;
const int _serialTx = 1;

// pins for serial communication
SoftwareSerial uno(_serialRx,_serialTx);

// delimiter for serial communication
const char DELIMITTER = ':';

// data from arduino
int _tempInteger = 1;
int _tempFraction = 1;
int _humidity = 0;

// Set your Static IP address
IPAddress local_IP(192, 168, 2, 184);
// Set your Gateway IP address
IPAddress gateway(192, 168, 2, 1);

IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

int random(int min, int max) //range : [min, max]
{
  static bool first = true;
  if (first) 
  {  
      srand( time(NULL) ); //seeding for the first time only!
      first = false;
  }
  return min + rand() % (( max + 1 ) - min);
}

String getTemperature() {
  String temperature = "not available";
  //Serial.println(temperature);

  if(uno.available()) {
    uno.write("CMD_TEMP");
    String temperature = uno.readStringUntil(DELIMITTER);
  }

  return String(temperature);
}

String getHumidity() {
  float humidity = random(30,95);
  Serial.println(humidity);
  return String(humidity);
}

String getSoilMoisture(int sensorNumber) {
  float soilMoisture = random(200,700);
  Serial.println(soilMoisture);
  return String(soilMoisture);
}

// // Replaces placeholder 
// String processor(const String& var){
//   Serial.println(var);
//   if (var == "TEMPERATURE"){
//     return getTemperature();
//   }
//   else if (var == "HUMIDITY"){
//     return getHumidity();
//   }
//   else if (var == "SOILMOISTURE-1") {
//     return getSoilMoisture(0);
//   }
// }

void setup() {
  // serial port for debugging purposes
  Serial.begin(115200);

  // serial connection for communication with arduino uno
  uno.begin(9600);

  // Initialize SPIFFS
  if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html", String(), false, processor);
  });
  
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/style.css", "text/css");
  });

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getTemperature().c_str());
  });
  
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getHumidity().c_str());
  });

  server.on("/soilmoisture-1", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", getSoilMoisture(0).c_str());
  });

  // Start server
  server.begin();
}

void loop(){
 
}