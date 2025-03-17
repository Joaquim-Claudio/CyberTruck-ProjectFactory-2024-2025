#ifndef Pinout_h
#define Pinout_h
#include "Arduino.h"

class Pinout {
    public:
        const static uint8_t MOTOR_FR_PIN1 = 2;
        const static uint8_t MOTOR_FR_PIN2 = 4;
        const static uint8_t MOTOR_FR_EN = 15;

        const static uint8_t MOTOR_FL_PIN1 = 17;
        const static uint8_t MOTOR_FL_PIN2 = 5;
        const static uint8_t MOTOR_FL_EN = 16;

        const static uint8_t MOTOR_BR_PIN1 = 19;
        const static uint8_t MOTOR_BR_PIN2 = 21;
        const static uint8_t MOTOR_BR_EN = 18;

        const static uint8_t MOTOR_BL_PIN1 = 1;
        const static uint8_t MOTOR_BL_PIN2 = 22;
        const static uint8_t MOTOR_BL_EN = 3;
};

#endif