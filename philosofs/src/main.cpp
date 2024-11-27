#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

using namespace std;

const int NUM_PHILOSOPHERS = 5;

mutex forks[NUM_PHILOSOPHERS];
int philosofs[NUM_PHILOSOPHERS]{};
mutex mt;

void SafePrint(int phId, string doing)
{
    mt.lock();
    string print = "Philosopher " + to_string(phId + 1) + " " + doing + "\n";
    cout << print;
    mt.unlock();
}



void philosof(int philosopher_id)
{
    while (true)
    {
        SafePrint(philosopher_id, "is thinking");
        this_thread::sleep_for(chrono::milliseconds(5000)); // Философ думает

        // Определяем вилки
        int left_fork = philosopher_id;
        int right_fork = (philosopher_id + 1) % NUM_PHILOSOPHERS;
        {
            lock(forks[left_fork], forks[right_fork]);
            lock_guard<mutex> left_lock(forks[left_fork], adopt_lock);
            lock_guard<mutex> right_lock(forks[right_fork], adopt_lock);

            SafePrint(philosopher_id, "is eating");
            this_thread::sleep_for(chrono::milliseconds(5000)); // Философ ест
        }
    }
}

int main()
{
    vector<thread> philosophers;

    for (int i = 0; i < NUM_PHILOSOPHERS; ++i)
    {
        philosophers.emplace_back(philosof, i);
    }

    for (auto &philosopher : philosophers)
    {
        philosopher.join();
    }

    // for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    // {
    //     cout << philosofs[i] << " ";
    // }

    return 0;
}
