#ifndef Timer_h
#define Timer_h

class Timer {
    public:
        unsigned long _start;
        unsigned long _current;
        unsigned long _period;

        Timer();
        Timer(unsigned long period);

        bool overlapsed();
};

#endif