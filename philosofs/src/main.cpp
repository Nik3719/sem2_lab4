#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

const int NUM_PHILOSOPHERS = 5;

std::mutex forks[NUM_PHILOSOPHERS];

void philosopher(int id)
{
    while (true)
    {
        std::cout << "Philosopher " << id + 1 << " is thinking." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(5000)); // Философ думает

        // Берем вилки
        std::unique_lock<std::mutex> left_fork(forks[id]);
        std::unique_lock<std::mutex> right_fork(forks[(id + 1) % NUM_PHILOSOPHERS]);

        std::cout << "Philosopher " << id + 1 << " is eating." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(5000)); // Философ ест

        // Вилки автоматически освобождаются при выходе из области видимости
    }
}

int main()
{
    std::vector<std::thread> philosophers;

    for (int i = 0; i < NUM_PHILOSOPHERS; ++i)
    {
        philosophers.emplace_back(philosopher, i);
    }

    for (auto &philosopher : philosophers)
    {
        philosopher.join();
    }

    return 0;
}
