#ifndef MoveController_h
#define MoveController_h
#include "Arduino.h"
#include "Motor.h"

class MoveController {
    public:
        MoveController();
        MoveController(Motor MOTOR_FR, Motor MOTOR_FL, Motor MOTOR_BR, Motor MOTOR_BL);

        void Stop();
        void MoveForward(uint8_t speed);
        void MoveBackward(uint8_t speed);
        void TurnRight();
        void TurnLeft();

    private:
        Motor _MOTOR_FR;
        Motor _MOTOR_FL;
        Motor _MOTOR_BR;
        Motor _MOTOR_BL;

};

#endif