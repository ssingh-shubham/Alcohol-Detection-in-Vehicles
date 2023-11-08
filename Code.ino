#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <UrlEncode.h>

const char* ssid = "portable hotspot";
const char* password = "rampur121";

String phoneNumber = "918818999906";
String apiKey = "5181948";

const int alcoholSensorPin = A0;
const int buzzerPin = D3;
const int threshold = 450;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void sendMessage(String message) {
  String url = "http://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);
  WiFiClient client;
  HTTPClient http;
  http.begin(client, url);
  int httpResponseCode = http.POST(url);
  if (httpResponseCode == 200) {
    Serial.println("Message sent successfully");
  } else {
    Serial.println("Error sending the message");
    Serial.print("HTTP response code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  lcd.init();
  lcd.backlight();
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  int alcoholLevel = analogRead(alcoholSensorPin);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Alcohol Level:");
  lcd.setCursor(0, 1);
  lcd.print(alcoholLevel);

  if (alcoholLevel > threshold) {
    digitalWrite(buzzerPin, HIGH);
    sendMessage("Alcohol Alert: High alcohol level detected!");
  } else {
    digitalWrite(buzzerPin, LOW);
  }

  delay(1000);
}
