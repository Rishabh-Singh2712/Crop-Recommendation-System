#include <ESP8266WiFi.h> 
const int potPin=A0;
float ph;
float Value=0;
 
String apiKey = "VF6I13J5H3GAX48O"; // Enter your Write API key from ThingSpeak
const char *ssid = "CROP";// replace with your wifi ssid and wpa2 key
const char *password = "88888888";
const char* server = "api.thingspeak.com";// don't change this
 
WiFiClient client;
 
 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(potPin,INPUT);
  delay(1000);
 
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
}
 
void loop() {
  // put your main code here, to run repeatedly:
Value= analogRead(potPin);
Serial.print(Value);
Serial.print(" | ");
float voltage=Value*(3.3/1024.0);
ph=(3.3*voltage);
Serial.println(ph);
delay(500);
 
if (client.connect(server, 80)) // "184.106.153.149" or api.thingspeak.com
   {
      String postStr = apiKey;
      postStr += "&field1=";
      postStr += String(ph);
      postStr += "\r\n";
    
      client.print("POST /update HTTP/1.1\n");
      client.print("Host: api.thingspeak.com\n");
      client.print("Connection: close\n");
      client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
      client.print("Content-Type: application/x-www-form-urlencoded\n");
      client.print("Content-Length: ");
      client.print(postStr.length());
      client.print("\n\n");
      client.print(postStr); 
    }    
}