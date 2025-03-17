#include "Timer.h"
#include <Arduino.h>

Timer::Timer(){}

Timer::Timer(unsigned long period) {
    _period = period;
}

bool Timer::overlapsed() {
    _current = millis();
    return _current - _start >= _period;
}