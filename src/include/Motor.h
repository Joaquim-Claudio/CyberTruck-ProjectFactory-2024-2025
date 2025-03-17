#ifndef Motor_h
#define Motor_h
#include "Arduino.h"

class Motor {
    public:
        uint8_t _PIN1;
        uint8_t _PIN2;
        uint8_t _EN; 
        
        Motor();
        Motor(uint8_t pin1, uint8_t pin2, uint8_t enable, 
            uint8_t freq=0, uint8_t res=0);
        

        void stop();
        void forward();
        void forward(uint8_t speed);
        void backward();
        void backward(uint8_t speed);
};

#endif