#include "GenRndChars.h"

mutex mt;
counting_semaphore<1> sem(1);
SemaphoreSlim semSlim(1);
barrier br(1);
SpinLock sl;
SpinWait sw;
bool conditions = false;
Monitor mn;

bool foo(bool &cond)
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::microseconds(100000));
        cond = !cond;
    }
}

static void GenRndChar(const int &countChar)
{
    for (size_t i = 0; i < countChar; i++)
    {
        std::random_device rd;
        std::mt19937 generator(rd()); // Инициализируем генератор Мерсеннского твистера
        std::uniform_int_distribution<int> distribution(32, 126);
        int num = distribution(generator);
        std::cout << char(num) << ' ';
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    cout << "\n";
}

void await(vector<thread> &threads)
{
    int len = threads.size();
    for (size_t i = 0; i < len; i++)
    {
        threads[i].join();
    }
}

static void GenRndCharMutex(const int &countChar)
{
    mt.lock();
    GenRndChar(countChar);
    mt.unlock();
}

void RunMutex(const int &countThreads, const int &countChar)
{
    vector<thread> threads;

    for (size_t i = 0; i < countThreads; i++)
    {
        threads.emplace_back(GenRndCharMutex, countChar);
    }
    await(threads);
}

static void genRndCharSemaphore(const int &countChar)
{
    sem.acquire();
    GenRndChar(countChar);
    sem.release();
}

void RunSemaphore(const int &countThreads, const int &countChar)
{
    vector<thread> threads;

    for (size_t i = 0; i < countThreads; i++)
    {
        threads.emplace_back(genRndCharSemaphore, countChar);
    }
    await(threads);
}

static void genRndCharSemaphoreSlim(const int &countChar)
{
    semSlim.Wait();
    GenRndChar(countChar);
    semSlim.Release();
}

void RunSemaphoreSlim(const int &countThreads, const int &countChar)
{
    vector<thread> threads;

    for (size_t i = 0; i < countThreads; i++)
    {
        threads.emplace_back(genRndCharSemaphoreSlim, countChar);
    }
    await(threads);
}

static void genRndCharBarrier(const int &countChar)
{

    br.arrive_and_wait();
    GenRndChar(countChar);
}

void RunBarrier(const int &countThreads, const int &countChar)
{
    vector<thread> threads;

    for (size_t i = 0; i < countThreads; i++)
    {
        threads.emplace_back(genRndCharBarrier, countChar);
    }
    await(threads);
}

static void genRndCharSpinLock(const int &countChar)
{
    sl.lock();
    GenRndChar(countChar);
    sl.unlock();
}

void RunSpinLock(const int &countThreads, const int &countChar)
{
    vector<thread> threads;

    for (size_t i = 0; i < countThreads; i++)
    {
        threads.emplace_back(genRndCharSpinLock, countChar);
    }
    await(threads);
}

static void genRndCharSpinWait(const int &countChar)
{

    int count = 0;
    sw.waitUntil([&count]()
                 {
                    if(count++&1)
                     return true; // Проверка условия
                     return false;
                 });
    //sw.waitUntil(conditions);
    GenRndChar(countChar);
}

void RunSpinWait(const int &countThreads, const int &countChar)
{
    vector<thread> threads;

    for (size_t i = 0; i < countThreads; i++)
    {
        threads.emplace_back([&countChar]()
                             {
            //sw.setCondition(condition);
            genRndCharSpinWait(countChar); });
    }
    await(threads);
}



static void genRndCharMonitor(const int &countChar)
{
    mn.consume();
    GenRndChar(countChar);
    mn.produce(0);
}

void RunMonitor(const int &countThreads, const int &countChar)
{
    vector<thread> threads;

    for (size_t i = 0; i < countThreads; i++)
    {
        threads.emplace_back(genRndCharMonitor, countChar);
    }
    await(threads);
}