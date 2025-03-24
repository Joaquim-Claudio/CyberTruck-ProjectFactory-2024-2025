#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <Arduino.h>

class Param {
    public:
        static const uint8_t MOTOR_RIGHT_CHANNEL;
        static const uint8_t MOTOR_LEFT_CHANNEL;

        static const char* WIFI_SSID;
        static const char* WIFI_PASSWORD;

        static const char* MQTT_BROKER_ADDRESS;
        static const char* MQTT_USERNAME;
        static const char* MQTT_PASSWORD;
        static const uint16_t MQTT_PORT;

        static const char* DRIVE_TOPIC;
        static const uint8_t MOTOR_DEFAULT_SPEED;
};

#endif