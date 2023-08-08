#include <ESP8266WiFi.h>
#include <SPI.h>
#include <Wire.h>
String apiKey = "*API KEY FROM THINGSPEAK*"; 
const char *ssid = "PRP108"; 
const char *pass = "iotlab108";
const char* server = "api.thingspeak.com";
#define soilWet 500 
#define soilDry 750 
#define sensorPower 14 
#define sensorPin A0
WiFiClient client;
void setup() {
pinMode(sensorPower, OUTPUT);
digitalWrite(sensorPower, LOW);
Serial.begin(115200);
Serial.println("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, pass);
while (WiFi.status() != WL_CONNECTED)
{
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi connected");
delay(4000);
}
void loop() {
int moisture = readSensor();
Serial.print("Analog Output: ");
Serial.println(moisture);
if (moisture < soilWet)
{
Serial.println("Status: Soil is too wet");
} else if (moisture >= soilWet && moisture < soilDry)
{
Serial.println("Status: Soil moisture is perfect");
} else
{
Serial.println("Status: Soil is too dry - time to water!");
}
delay(1000); 
Serial.println();
if (client.connect(server, 80)) 
{
String postStr = apiKey;
postStr += "&field1=";
postStr += String(moisture);
postStr += "r\n";
client.print("POST /update HTTP/1.1\n");
client.print("Host: api.thingspeak.com\n");
client.print("Connection: close\n");
client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
client.print("Content-Type: application/x-www-form-urlencoded\n");
client.print("Content-Length: ");
client.print(postStr.length());
client.print("\n\n");
client.print(postStr);
Serial.println("Data Send to Thingspeak");
}
client.stop();
Serial.println("Waiting...");
delay(2000); 
}
int readSensor() {
digitalWrite(sensorPower, HIGH); 
delay(10); 
int val = analogRead(sensorPin); 
digitalWrite(sensorPower, LOW); 
return val; 
}
