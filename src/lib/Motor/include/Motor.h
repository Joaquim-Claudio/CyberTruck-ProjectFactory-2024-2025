#ifndef MOTOR_H
#define MOTOR_H
#include "Arduino.h"


class Motor {
    public:
        uint8_t _PIN1;
        uint8_t _PIN2;
        uint8_t _EN; 
        uint8_t _CHANNEL;
        
        Motor();
        Motor(uint8_t pin1, uint8_t pin2, uint8_t enable, uint8_t pwmChannel);
        

        void stop();
        void forward();
        void forward(uint8_t speed);
        void backward();
        void backward(uint8_t speed);
};

#endif