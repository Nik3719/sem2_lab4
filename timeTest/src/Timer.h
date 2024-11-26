#pragma once
#include <iostream>
#include <chrono>

class Timer
{
public:
    Timer() {
        start = std::chrono::high_resolution_clock::now();
    }

    ~Timer() {
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        std::cout << "\ntime: " << duration.count() << " seconds" << std::endl;
    }

private:
    std::chrono::high_resolution_clock::time_point start, end;
};