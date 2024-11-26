#include <atomic>
#include <chrono>
#include <thread>
#include <functional>
#include <mutex>

class SpinWait
{
public:
    // Метод для ожидания выполнения условия
    void waitUntil(std::function<bool()> condition)
    {
        while (!condition())
        {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
    }
    void waitUntil(bool &condition)
    {
        while (!condition)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
    }

private:
    std::atomic<bool> flag; // Флаг для состояния
};