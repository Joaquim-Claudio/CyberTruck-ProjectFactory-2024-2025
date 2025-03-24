#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "Motor.h"
#include "MoveController.h"
#include "Pinout.h"
#include "Param.h"
#include "Timer.h"
#include "vehicle.pb.h"
#include "pb_decode.h"

Motor motor_right(Pinout::MOTOR_RIGHT_PIN1, Pinout::MOTOR_RIGHT_PIN2, Pinout::MOTOR_RIGHT_EN, Param::MOTOR_RIGHT_CHANNEL);
Motor motor_left(Pinout::MOTOR_LEFT_PIN1, Pinout::MOTOR_LEFT_PIN2, Pinout::MOTOR_LEFT_EN, Param::MOTOR_LEFT_CHANNEL);

MoveController move(motor_right, motor_left);


WiFiClientSecure wifiClient;
PubSubClient mqttClient(wifiClient);

// Method declarations
void wifi_setup();
void mqtt_setup();
void mqtt_reconnect();
void mqtt_handle_msg(char*, byte*, unsigned int);


void setup() {
    Serial.begin(9600);
    Serial.println("\nSerial communication initialized.");

    Serial.print("Setting up WiFi conection...");
    wifi_setup();

    Serial.print("Setting up MQTT configuration...");
    mqtt_setup();

    move.Stop();
}

void loop() {

    if(!mqttClient.connected()){
        mqtt_reconnect();
    }

    while(Serial.available()) {

        String line = Serial.readStringUntil('\n');
        char buffer[line.length()];
        line.toCharArray(buffer, line.length());

        String cmd = strtok(buffer, " ");
        String instruction = strtok(NULL, " ");

        int speed = Param::MOTOR_DEFAULT_SPEED;

        Serial.print("Entrada: ");
        Serial.println(line);


        if(cmd.equalsIgnoreCase("move")) {
            if(instruction.equalsIgnoreCase("--front")) {

                move.MoveForward(speed);
                Serial.println("Moving forward.");
            }

            else if(instruction.equalsIgnoreCase("--back")) {

                move.MoveBackward(speed);
                Serial.println("Moving backward.");
            }

            else if(instruction.equalsIgnoreCase("--left")) {
                move.TurnLeft(speed);
                Serial.println("Turning left.");
            }

            else if(instruction.equalsIgnoreCase("--right")) {
                move.TurnRight(speed);
                Serial.println("Turning right.");
            }

            else if(instruction.equalsIgnoreCase("--stop")) {
                Serial.println("Stopped.");
            }

            else {
                Serial.println("Instrução inválida.");
            }

            move.Stop();
        }
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

    VehicleCommand command;
    pb_istream_t stream = pb_istream_from_buffer(payload, msg_size);

    if(pb_decode(&stream, VehicleCommand_fields, &command)) {
        Serial.print("Actuator: ");
        Serial.println(command.actuator);
    } else {
        Serial.println("Deserialization failed.");
    }
}