#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "Motor.h"
#include "MoveController.h"
#include "Pinout.h"
#include "Param.h"
#include "Timer.h"
#include "VehicleCommand.h"

Motor motor_right(Pinout::MOTOR_RIGHT_PIN1, Pinout::MOTOR_RIGHT_PIN2, Pinout::MOTOR_RIGHT_EN, Param::MOTOR_RIGHT_CHANNEL);
Motor motor_left(Pinout::MOTOR_LEFT_PIN1, Pinout::MOTOR_LEFT_PIN2, Pinout::MOTOR_LEFT_EN, Param::MOTOR_LEFT_CHANNEL);

MoveController driver(motor_right, motor_left);

WiFiClientSecure wifiClient;
PubSubClient mqttClient(wifiClient);

JsonDocument jsonDocument;
VehicleCommand vehicleCommand;

Timer remoteCommandTimer(200);

// Method declarations
void wifi_setup();
void mqtt_setup();
void mqtt_reconnect();
void mqtt_handle_msg(char*, byte*, unsigned int);
void driverController();

void setup() {
    Serial.begin(9600);
    Serial.println("\nSerial communication initialized");

    Serial.print("Setting up WiFi conNection...");
    wifi_setup();

    Serial.print("Setting up MQTT configuration...");
    mqtt_setup();

    driver.Stop();
}

void loop() {

    if(!mqttClient.connected()){
        mqtt_reconnect();
    }

    mqttClient.loop();

    if(remoteCommandTimer.overlapsed()) {
        driver.Stop();
    }
}

void wifi_setup() {
    WiFi.mode(WIFI_STA);

    WiFi.begin(Param::WIFI_SSID, Param::WIFI_PASSWORD);
    while(WiFi.status() != WL_CONNECTED) {
        delay(200);
        Serial.print(".");
    }

    wifiClient.setInsecure();

    Serial.println("done");
    Serial.print("IPv4:\t");
    Serial.println(WiFi.localIP());
}

void mqtt_setup() {
    mqttClient.setServer(Param::MQTT_BROKER_ADDRESS, Param::MQTT_PORT);
    mqttClient.setCallback(mqtt_handle_msg);
    Serial.println("done");
}

void mqtt_reconnect() {
    Serial.print("Trying connection to MQTT Broker...");
    String clientID = "esp32-client-" + WiFi.macAddress();

    Timer timer(5000);

    while(!mqttClient.connected()) {

        if(timer.overlapsed()) {
            Serial.println("Connection attempt timed out.");
            return;
        }

        mqttClient.connect(clientID.c_str(), Param::MQTT_USERNAME, Param::MQTT_PASSWORD);
        delay(100);
        Serial.print(".");
    }

    Serial.println("done");
    mqttClient.subscribe(Param::DRIVE_TOPIC);
}

void mqtt_handle_msg(char* topic, byte* payload, unsigned int msg_size) {
    Serial.print("A message arrived on topic: \"");
    Serial.print(topic);
    Serial.println("\". Deserializing...");
    
    String message = "";
    for(int i=0; i<msg_size; i++) {
        message += (char) payload[i];
    }

    Serial.println(message);

    jsonDocument.clear();
    DeserializationError error = deserializeJson(jsonDocument, payload, msg_size);

    if(error) {
        Serial.println("Deserialization failed.");
    }

    vehicleCommand.vehicleId = jsonDocument["vehicleId"].as<String>();
    vehicleCommand.agentId = jsonDocument["agentId"].as<String>();
    vehicleCommand.action = jsonDocument["action"].as<String>();
    vehicleCommand.cmd = jsonDocument["cmd"].as<String>();
    vehicleCommand.value = jsonDocument["value"].as<int>();

    Serial.println(vehicleCommand.vehicleId);
    Serial.println(vehicleCommand.agentId);
    Serial.println(vehicleCommand.action);
    Serial.println(vehicleCommand.cmd);
    Serial.println(vehicleCommand.value);

    remoteCommandTimer.reset();
    driverController();
}

void driverController() {
    if(vehicleCommand.action.equalsIgnoreCase("move")) {

        if(vehicleCommand.cmd.equalsIgnoreCase("front")) driver.MoveForward(Param::MOTOR_DEFAULT_SPEED);
        else if(vehicleCommand.cmd.equalsIgnoreCase("back")) driver.MoveBackward(Param::MOTOR_DEFAULT_SPEED);
        else if(vehicleCommand.cmd.equalsIgnoreCase("right")) driver.TurnRight(Param::MOTOR_DEFAULT_SPEED);
        else if(vehicleCommand.cmd.equalsIgnoreCase("left")) driver.TurnLeft(Param::MOTOR_DEFAULT_SPEED);

    }
}