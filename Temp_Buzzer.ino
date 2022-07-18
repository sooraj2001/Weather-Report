
#include <DHT.h>
#include <WiFi.h>
#include <WebServer.h>
//#include "DHT.h"
// Uncomment one of the lines below for whatever DHT sensor type you're using!

#define DHTTYPE DHT11 // DHT 11
#define BUZZER_PIN 26 // Buzzer connected to pin 26
#define Threshold 25 //Threshold temperature is set to 25
/*Put your SSID & Password*/
const char* ssid = " REPLACE_WITH_YOUR_SSID "; // Enter SSID here
const char* password = " REPLACE_WITH_YOUR_PASSWORD"; //Enter Password here
WebServer server(80);
// DHT Sensor
uint8_t DHTPin = 4;
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);
float Temperature;
float Humidity;

void setup() {
Serial.begin(115200);
delay(100);
pinMode(DHTPin, INPUT);
dht.begin();
Serial.println("Connecting to ");
Serial.println(ssid);
//connect to your local wi-fi network
WiFi.begin(ssid, password);
//check wi-fi is connected to wi-fi network
while (WiFi.status() != WL_CONNECTED) {
delay(1000);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi connected..!");
Serial.print("Got IP: "); Serial.println(WiFi.localIP());
server.on("/", handle_OnConnect);
server.onNotFound(handle_NotFound);
server.begin();
Serial.println("HTTP server started");
pinMode(BUZZER_PIN,OUTPUT); //Set Buzzer pin as Output
}

void loop() {
server.handleClient();
}

void handle_OnConnect() {
Temperature = dht.readTemperature(); // Gets the values of the temperature
Humidity = dht.readHumidity(); // Gets the values of the humidity
if(Temperature > Threshold)
  digitalWrite(BUZZER_PIN,HIGH);
  else
  digitalWrite(BUZZER_PIN,LOW);
server.send(200, "text/html", SendHTML(Temperature,Humidity));
}

void handle_NotFound(){
server.send(404, "text/plain", "Not found");
}

String SendHTML(float Temperaturestat,float Humiditystat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>ESP32 Weather Report</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>ESP32 Weather Report</h1>\n";
  
  ptr +="<p>Temperature: ";
  ptr +=(int)Temperaturestat;
  ptr +="°C</p>";
  ptr +="<p>Humidity: ";
  ptr +=(int)Humiditystat;
  ptr +="%</p>";
  
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
