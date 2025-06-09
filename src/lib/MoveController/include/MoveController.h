#ifndef MOVE_CONTROLLER_H
#define MOVE_CONTROLLER_H
#include "Arduino.h"
#include "Motor.h"

class MoveController {
    public:
        MoveController();
        MoveController(Motor MOTOR_RIGHT, Motor MOTOR_LEFT);

        void Stop();
        void MoveForward(uint8_t speed);
        void MoveBackward(uint8_t speed);
        void TurnRight(uint8_t speed);
        void TurnLeft(uint8_t speed);

    private:
        Motor _MOTOR_RIGHT;
        Motor _MOTOR_LEFT;
};

#endif