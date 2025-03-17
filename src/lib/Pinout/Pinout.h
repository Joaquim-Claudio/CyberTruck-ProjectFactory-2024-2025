#ifndef Pinout_h
#define Pinout_h
#include "Arduino.h"

class Pinout {
    public:
        const static uint8_t MOTOR_RIGHT_PIN1 = 2;
        const static uint8_t MOTOR_RIGHT_PIN2 = 4;
        const static uint8_t MOTOR_RIGHT_EN = 15;

        const static uint8_t MOTOR_LEFT_PIN1 = 18;
        const static uint8_t MOTOR_LEFT_PIN2 = 19;
        const static uint8_t MOTOR_LEFT_EN = 5;

        Pinout();
};

#endif