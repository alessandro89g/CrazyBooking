#ifndef TIMER_H
#define TIMER_H

#include<iostream>
#include<chrono>

class Timer
{
public:
    Timer();
    ~Timer();

private:
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::chrono::duration<float> duration;
};

#endif // TIMER_H
