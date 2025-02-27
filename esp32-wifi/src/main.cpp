#include <Arduino.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>

WebServer server(80);

bool ledStatus = false;
bool buzzStatus = false;
int ldrStatus = 0;

const int ledPin = 18;
const int buzzPin = 21;
const int ldrPin = 23;

// set the PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

unsigned long measureDelay = 3000;
unsigned long lastTimeRan;

JsonDocument jsonDocument;
char buffer[1024];

// put function declarations here:
void handlePost();
void handleGet();
void addJsonObject(String, int, String);
void setupWifi();
void setupAPI();

void setup() {
  // put your setup code here, to run once:
  ledcSetup(ledChannel, freq, resolution);

  pinMode(ledPin, OUTPUT);
  pinMode(buzzPin, OUTPUT);
  pinMode(buzzPin, INPUT);

  Serial.begin(9600);
  delay(1500);
  Serial.println("Serial communication successfully initialized.");

  Serial.println("Initializing wifi mode...");
  setupWifi();

  Serial.println("Initializing the API...");
  setupAPI();
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();

  digitalWrite(ledPin, ledStatus);
  (buzzStatus) ? tone(buzzPin, 500) : noTone(buzzPin);
  ldrStatus = digitalRead(ldrPin);
}

void setupAPI() {
  server.on("/getValues", handleGet);
  server.on("/setStatus", HTTP_POST, handlePost);

  server.begin();
  Serial.println("API successfully initialized.");
}

void handlePost() {
  if(!server.hasArg("plain")) {
    Serial.println("No plain found in a POST request.");
    return;
  }

  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);

  ledStatus = jsonDocument["ledStatus"];
  buzzStatus = jsonDocument["buzzStatus"];
  
  server.send(200, "application/json", "{}");
}

void handleGet() {

  jsonDocument.clear();
  addJsonObject("ldrStatus", ldrStatus, "%");
  addJsonObject("buzzStatus", buzzStatus, "boolean");
  addJsonObject("ledStatus", ledStatus, "boolean");

  serializeJson(jsonDocument, buffer);
  server.send(200, "application/json", buffer);
}

void addJsonObject(String name, int value, String unit) {
  JsonObject obj = jsonDocument.add<JsonObject>();
  obj["name"] = name;
  obj["value"] = value;
  obj["unit"]  = unit;
}

void setupWifi() {
  WiFi.mode(WIFI_STA);

  WiFiManager wm;
  // wm.resetSettings();

  bool res = wm.autoConnect("AutoConnectAP", "password");

  if(!res) {
    Serial.println("WiFi failed to connect.");
    ESP.restart();
    return;
  }

  Serial.println("WiFi mode is now on.");

}