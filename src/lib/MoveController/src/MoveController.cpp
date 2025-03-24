#include "MoveController.h"
#include <Arduino.h>

MoveController::MoveController(){}

MoveController::MoveController(Motor MOTOR_RIGHT, Motor MOTOR_LEFT) {
    _MOTOR_RIGHT = MOTOR_RIGHT;
    _MOTOR_LEFT = MOTOR_LEFT;
}

void MoveController::Stop() {
    _MOTOR_RIGHT.stop();
    _MOTOR_LEFT.stop();
}

void MoveController::MoveForward(uint8_t speed) {
    _MOTOR_RIGHT.forward(speed);
    _MOTOR_LEFT.forward(speed);
}

void MoveController::MoveBackward(uint8_t speed) {
    _MOTOR_RIGHT.backward(speed);
    _MOTOR_LEFT.backward(speed);
}

void MoveController::TurnRight(uint8_t speed) {
    _MOTOR_RIGHT.backward(speed);
    _MOTOR_LEFT.forward(speed);
}

void MoveController::TurnLeft(uint8_t speed) {
    _MOTOR_RIGHT.forward(speed);
    _MOTOR_LEFT.backward(speed);
}