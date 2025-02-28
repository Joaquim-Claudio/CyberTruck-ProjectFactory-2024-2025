#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "DHTesp.h"
#include <PubSubClient.h>

#define DHT11_PIN 23
#define LED_PIN 22

DHTesp dht;

WiFiClientSecure wifiClient;
PubSubClient mqttClient(wifiClient);

// MQTT credentials
const char* MQTT_BROKER = "9b6eee11960f44d39f52cd73134c6651.s1.eu.hivemq.cloud";
const int MQTT_PORT = 8883;
const char* MQTT_USER = "cybertruck-mqtt-user";
const char* MQTT_PASSWORD = "Cybertruck-mqtt-passw0rd";

//WiFi credentials
const char* ssid = "...";
const char* psw = "...";

float temperature;
float humidity;

unsigned long measureDelay = 3000;
unsigned long lastMeasureTime;

// put function declarations here:
void setupWiFi();
void setupMqtt();
void mqttHandleMsg(char*, byte*, unsigned int);
void mqttReconnect();

void setup() {
  // put your setup code here, to run once:
  dht.setup(DHT11_PIN, DHTesp::DHT11);
  pinMode(LED_PIN, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("Serial communication successfully initialized.");

  Serial.println("Initializing WiFi mode...");
  setupWiFi();

  Serial.println("Initializing MQTT connection...");
  setupMqtt();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(!mqttClient.connected()) {
    mqttReconnect();
  }

  mqttClient.loop();

  if(millis() > lastMeasureTime + measureDelay) {
    temperature = dht.getTemperature();
    humidity = dht.getHumidity();

    Serial.print("Temperatura: "); Serial.println(temperature);
    Serial.print("Humidade: "); Serial.println(humidity);

    char strTemperature[8];
    char strHumidity[8];

    dtostrf(temperature, 1, 2, strTemperature);
    dtostrf(humidity, 1, 2, strHumidity);

    mqttClient.publish("cybertruck/temperature", strTemperature);
    mqttClient.publish("cybertruck/humidade", strHumidity);

    lastMeasureTime = millis();

  }
}

// put function definitions here:

// Setup and initialize WiFi mode
void setupWiFi() {
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, psw);

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("WiFi mode is now on.");
  Serial.print("IPv4: ");
  Serial.println(WiFi.localIP());

  wifiClient.setInsecure();
}

void setupMqtt() {
  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  mqttClient.setCallback(mqttHandleMsg);
  // mqttClient.setBufferSize(255);
}

// Handle message received from the Broker
void mqttHandleMsg(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.println(topic);
  Serial.print("Message: [");
  String tempMessage;

  for(int i=0; i<length; i++) {
    Serial.print( (char) payload[i]);
    tempMessage += (char) payload[i];
  }
  Serial.println("]");
  Serial.println();

  if((String) topic == "cybertruck/ledstatus") {
    Serial.print("Setting led status to ");

    if(tempMessage == "on") {
      Serial.println("on.");
      digitalWrite(LED_PIN, HIGH);
    }
    
    else {
      Serial.println("off.");
      digitalWrite(LED_PIN, LOW);
    }
  }

}

void mqttReconnect () {
  while(!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");

    String clientId = "esp32-client-" + WiFi.macAddress();

    Serial.print(" ClientID: ");
    Serial.println(clientId.c_str());

    if(mqttClient.connect(clientId.c_str(), MQTT_USER, MQTT_PASSWORD)) {

      Serial.println("Connected!");
      mqttClient.subscribe("cybertruck/ledstatus");
    } 
    else {

      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }

  }
}