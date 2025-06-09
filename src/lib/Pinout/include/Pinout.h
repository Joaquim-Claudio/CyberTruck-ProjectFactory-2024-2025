#ifndef PINOUT_H
#define PINOUT_H
#include "Arduino.h"

class Pinout {
    public:
        static const uint8_t MOTOR_RIGHT_PIN1;
        static const uint8_t MOTOR_RIGHT_PIN2;
        static const uint8_t MOTOR_RIGHT_EN;

        static const uint8_t MOTOR_LEFT_PIN1;
        static const uint8_t MOTOR_LEFT_PIN2;
        static const uint8_t MOTOR_LEFT_EN;

        static const uint8_t ULTRASONIC_TRIGGER;
        static const uint8_t ULTRASONIC_ECHO;
        static const uint8_t ULTRASONIC_MOTOR;

        static const uint8_t LINETRACKING_IR2;
        static const uint8_t LINETRACKING_IR4;

        static const uint8_t BATTERY_SENSOR;
    };

#endif