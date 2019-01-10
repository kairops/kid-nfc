#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

const char* ssid = "SSID_WIFI";
const char* password = "PASSWORD";

const String raspberryIP = "192.168.100.100";
const String raspberryPORT = "3000";
String raspberryURL = "http://" + raspberryIP + ":" + raspberryPORT;

const int greenLed = 16;

String sendRequestPost(String host, char* JSONmessageBuffer)
{
  HTTPClient http;
  http.begin(host);
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.POST(JSONmessageBuffer);
  String payload = http.getString();
  http.end();
  return payload;
}

void sendLog(String value, String code = "1")
{
  StaticJsonBuffer<300> JSONbuffer;
  char JSONmessageBuffer[300];
  JsonObject& JSONencoder = JSONbuffer.createObject();

  JSONencoder["device"] = WiFi.macAddress();
  JSONencoder["code"] = code;
  JSONencoder["message"] = value;
  JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  
  sendRequestPost(raspberryURL + "/log", JSONmessageBuffer);
}

void startWifi()
{
  delay(10);   
  WiFi.mode(WIFI_STA); // Modo cliente WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);    
  }
}

void startPinMode()
{
  pinMode(greenLed, OUTPUT);
}

void ledFlashing(int pinLedOut, int num = 1, int microseconds = 100)
{
  for (int i = 0; i <= num; i++) {
    digitalWrite(pinLedOut, HIGH);
    delay(microseconds);
    digitalWrite(pinLedOut, LOW);
    delay(microseconds);
  }
}

void setup()
{
  Serial.begin(9600);
  startWifi();
  startPinMode();
  sendLog(WiFi.localIP() + " UP!");
  ledFlashing(greenLed, 3);
  Serial.print("UP!");
}

void loop()
{ 
    
}
