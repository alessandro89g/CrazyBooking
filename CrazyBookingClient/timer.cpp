#include "timer.h"

Timer::Timer()
{
    start = std::chrono::high_resolution_clock::now();
}

Timer::~Timer() {
    end = std::chrono::high_resolution_clock::now();
    duration = end-start;
    std::cout << "Time needed to book: " << duration.count()*1000.f << " ms\n" << std::endl;
}


