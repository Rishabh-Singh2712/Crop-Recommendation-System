#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
const char* ssid = "CROP";
const char* password = "88888888";
const int RE = D2; // Define RE pin
const int DE = D1; // Define DE pin
const byte nitro[] = {0x01,0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};
const byte phos[] = {0x01,0x03, 0x00, 0x1f, 0x00, 0x01, 0xb5, 0xcc};
const byte pota[] = {0x01,0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0};
byte values[11];
void setup() {
Serial.begin(9600);
pinMode(RE, OUTPUT);
pinMode(DE, OUTPUT);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(1000);
Serial.println("Connecting to WiFi...");
}
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
delay(500);
display.clearDisplay();
display.setCursor(25, 15);
display.setTextSize(1);
display.setTextColor(WHITE);
display.println(" NPK Sensor");
display.setCursor(25, 35);
display.setTextSize(1);
display.print("Initializing");
display.display();
delay(3000);
}
void loop() {
byte val1,val2,val3;
val1 = nitrogen();
delay(250);
val2 = phosphorous();
delay(250);
val3 = potassium();
delay(250);
Serial.print("Nitrogen: ");
Serial.print(val1);
Serial.println(" mg/kg");
Serial.print("Phosphorous: ");
Serial.print(val2);
Serial.println(" mg/kg");
Serial.print("Potassium: ");
Serial.print(val3);
Serial.println(" mg/kg");
delay(2000);
display.clearDisplay();
display.setTextSize(2);
display.setCursor(0, 5);
display.print("N: ");
display.print(val1);
display.setTextSize(1);
display.print(" mg/kg");
display.setTextSize(2);
display.setCursor(0, 25);
display.print("P: ");
display.print(val2);
display.setTextSize(1);
display.print(" mg/kg");
display.setTextSize(2);
display.setCursor(0, 45);
display.print("K: ");
display.print(val3);
display.setTextSize(1);
display.print(" mg/kg");
display.display();
}
byte nitrogen() {
digitalWrite(DE, HIGH);
digitalWrite(RE, HIGH);
delay(10);
if (Serial1.write(nitro, sizeof(nitro)) == 8) {
digitalWrite(DE, LOW);
digitalWrite(RE, LOW);
for (byte i = 0; i < 7; i++) {
values[i] = Serial1.read();
Serial.print(values[i], HEX);
}
Serial.println();
}
return values[4];
}
byte phosphorous() {
digitalWrite(DE, HIGH);
digitalWrite(RE, HIGH);
delay(10);
if (Serial1.write(phos, sizeof(phos)) == 8) {
digitalWrite(DE, LOW);
digitalWrite(RE, LOW);
for (byte i = 0; i < 7; i++) {
values[i] = Serial1.read();
Serial.print(values[i], HEX);
}
Serial.println();
}
return values[4];
}
byte potassium() {
digitalWrite(DE, HIGH);
digitalWrite(RE, HIGH);
delay(10);
if (Serial1.write(pota, sizeof(pota)) == 8) {
digitalWrite(DE, LOW);
digitalWrite(RE, LOW);
for (byte i = 0; i < 7; i++) {
values[i] = Serial1.read();
Serial.print(values[i], HEX);
}
Serial.println();
}
return values[4];
}