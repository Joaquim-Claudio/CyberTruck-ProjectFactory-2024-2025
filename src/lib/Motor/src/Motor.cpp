#include "Motor.h"
#include <Arduino.h>

const uint16_t DEFAULT_FREQ = 3000;
const uint8_t DEFAULT_RES = 8;

Motor::Motor(){}

Motor::Motor(uint8_t pin1, uint8_t pin2, uint8_t enable, uint8_t pwmChannel) {

    _PIN1 = pin1;
    _PIN2 = pin2;
    _EN = enable;
    _CHANNEL = pwmChannel;

    pinMode(_PIN1, OUTPUT);
    pinMode(_PIN2, OUTPUT);
    pinMode(_EN, OUTPUT);

    // digitalWrite(_EN, HIGH);

    ledcSetup(_CHANNEL, DEFAULT_FREQ, DEFAULT_RES);
    ledcAttachPin(_EN, _CHANNEL);
}

void Motor::stop() {
    digitalWrite(_PIN1, LOW);
    digitalWrite(_PIN2, LOW);
}

void Motor::forward() {
    digitalWrite(_PIN1, HIGH);
    digitalWrite(_PIN2, LOW);
}

void Motor::forward(uint8_t speed) {
    digitalWrite(_PIN1, HIGH);
    digitalWrite(_PIN2, LOW);
    ledcWrite(_CHANNEL, speed);
}

void Motor::backward() {
    digitalWrite(_PIN1, LOW);
    digitalWrite(_PIN2, HIGH);
}

void Motor::backward(uint8_t speed) {
    digitalWrite(_PIN1, LOW);
    digitalWrite(_PIN2, HIGH);
    ledcWrite(_CHANNEL, speed);
}