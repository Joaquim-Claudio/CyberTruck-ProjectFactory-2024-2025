#include "Motor.h"
#include <Arduino.h>

const uint16_t DEFAULT_FREQ = 3000;
const uint8_t DEFAULT_RES = 8;
const uint8_t DEFAULT_SPEED = 255;

Motor::Motor(){}

Motor::Motor(uint8_t pin1, uint8_t pin2, uint8_t pwmChannel_1, uint8_t pwmChannel_2) {

    _PIN1 = pin1;
    _PIN2 = pin2;
    _CHANNEL1 = pwmChannel_1;
    _CHANNEL2 = pwmChannel_2;

    pinMode(_PIN1, OUTPUT);
    pinMode(_PIN2, OUTPUT);

    ledcSetup(_CHANNEL1, DEFAULT_FREQ, DEFAULT_RES);
    // ledcAttachPin(_PIN1, _CHANNEL1);

    ledcSetup(_CHANNEL2, DEFAULT_FREQ, DEFAULT_RES);
    // ledcAttachPin(_PIN2, _CHANNEL2);
}

void Motor::stop() {
    // ledcWrite(_CHANNEL1, 0);
    // ledcWrite(_CHANNEL2, 0);

    digitalWrite(_PIN1, LOW);
    digitalWrite(_PIN2, LOW);
}

void Motor::forward() {
    // ledcWrite(_CHANNEL1, DEFAULT_SPEED);
    // ledcWrite(_CHANNEL2, 0);

    digitalWrite(_PIN1, HIGH);
    digitalWrite(_PIN2, LOW);
}

void Motor::forward(uint8_t speed) {
    // ledcWrite(_CHANNEL1, speed);
    // ledcWrite(_CHANNEL2, 0);

    digitalWrite(_PIN1, HIGH);
    digitalWrite(_PIN2, LOW);
}

void Motor::backward() {
    // ledcWrite(_CHANNEL1, 0);
    // ledcWrite(_CHANNEL2, DEFAULT_SPEED);

    digitalWrite(_PIN1, LOW);
    digitalWrite(_PIN2, HIGH);
}

void Motor::backward(uint8_t speed) {
    // ledcWrite(_CHANNEL1, 0);
    // ledcWrite(_CHANNEL2, speed);

    digitalWrite(_PIN1, LOW);
    digitalWrite(_PIN2, HIGH);
}