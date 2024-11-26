#pragma once
#include <iostream>
#include <mutex>
#include <condition_variable>

class SemaphoreSlim
{
public:
    explicit SemaphoreSlim(int initialCount = 0) : count(initialCount) {}

    void Wait()
    {
        std::unique_lock<std::mutex> lock(mutex);
        // Ожидаем, пока счетчик не станет больше 0
        condition.wait(lock, [this]()
                       { return count > 0; });
        --count; // Уменьшаем счетчик
    }

    void Release(int releaseCount = 1)
    {
        std::lock_guard<std::mutex> lock(mutex);
        count += releaseCount; // Увеличиваем счетчик
        for (int i = 0; i < releaseCount; ++i)
        {
            condition.notify_one(); // Уведомляем один поток
        }
    }

private:
    std::mutex mutex;
    std::condition_variable condition;
    int count;
};