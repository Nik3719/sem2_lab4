#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <random>
#include <semaphore>
#include <barrier>
#include"Timer.h"
#include "SemaphoreSlim.h"
#include "SpinLock.h"
#include "SpinWait.h"
#include"Monitor.h"



using namespace std;



void RunMutex(const int &countThreads, const int &countChar);
void RunSemaphore(const int &countThreads, const int &countChar);
void RunSemaphoreSlim(const int &countThreads, const int &countChar);
void RunBarrier(const int &countThreads, const int &countChar);
void RunSpinLock(const int &countThreads, const int &countChar);
void RunSpinWait(const int &countThreads, const int &countChar);
void RunMonitor(const int &countThreads, const int &countChar);