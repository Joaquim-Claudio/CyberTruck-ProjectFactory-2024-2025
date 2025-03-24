#include "Param.h"

// Motor PWM channel setup
const uint8_t Param::MOTOR_RIGHT_CHANNEL = 0;
const uint8_t Param::MOTOR_LEFT_CHANNEL = 1;

// WiFi configuration parameters
const char* Param::WIFI_SSID = "...";
const char* Param::WIFI_PASSWORD = "...";

// MQTT configuration parameters
const char* Param::MQTT_BROKER_ADDRESS = "9b6eee11960f44d39f52cd73134c6651.s1.eu.hivemq.cloud";
const char* Param::MQTT_USERNAME = "cybertruck-mqtt-user";
const char* Param::MQTT_PASSWORD = "Cybertruck-mqtt-passw0rd";
const uint16_t Param::MQTT_PORT = 8883;

// MQTT topics
const char* Param::DRIVE_TOPIC = "drive/command";

// Default values
const uint8_t Param::MOTOR_DEFAULT_SPEED = 225;
