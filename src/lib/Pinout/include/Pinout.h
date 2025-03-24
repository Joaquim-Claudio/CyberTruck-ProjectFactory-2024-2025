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
};

#endif