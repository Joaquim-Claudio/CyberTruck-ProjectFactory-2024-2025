#include "Param.h"

// Motor PWM channel setup
const uint8_t Param::MOTOR_RIGHT_CHANNEL_1 = 0;
const uint8_t Param::MOTOR_RIGHT_CHANNEL_2 = 1;
const uint8_t Param::MOTOR_LEFT_CHANNEL_1 = 2;
const uint8_t Param::MOTOR_LEFT_CHANNEL_2 = 3;

// WiFi configuration parameters
const char* Param::WIFI_SSID = "Visitors";
const char* Param::WIFI_PASSWORD = "";

// MQTT configuration parameters
const char* Param::MQTT_BROKER_ADDRESS = "10.36.254.21";
const char* Param::MQTT_USERNAME = "cybertruck-mqtt-user";
const char* Param::MQTT_PASSWORD = "Cybertruck-mqtt-passw0rd";
const uint16_t Param::MQTT_PORT = 1883;

// MQTT topics
const char* Param::DRIVE_TOPIC = "drive/command";
const char* Param::DISTANCE_TOPIC = "ultrasonic/command";

// Default values
const uint8_t Param::MOTOR_DEFAULT_SPEED = 255;
const float Param::SOUND_SPEED = 0.034;
