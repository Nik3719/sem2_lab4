#include <iostream>
#include <vector>
#include <fstream>
#include <thread>
#include <mutex>
#include <fstream>
#include <cmath>
#include <numeric>
#include "Timer.h"

using namespace std;

struct times
{
    int hours, minutes, seconds;
};

// Перегрузка оператора >>
istream &operator>>(istream &is, times &t)
{
    // Чтение значений из потока
    cin >> t.hours >> t.minutes >> t.seconds;
    return is; // Возвращаем поток
}

void await(vector<thread> &threads)
{
    int len = threads.size();
    for (size_t i = 0; i < len; i++)
    {
        threads[i].join();
    }
}

int timeCheck(const vector<times> &arrTime, int len)
{
    mutex mt;
    mt.lock();
    vector<times> res;
    int count = 0;
    for (size_t i = 0; i < len; i++)
    {
        this_thread::sleep_for(chrono::milliseconds(10));
        times oneTime = arrTime[i];
        if (oneTime.hours > 23 ||
            oneTime.minutes > 59 ||
            oneTime.seconds > 59)
        {
            count++;
            res.push_back(oneTime);
        }
    }

    int lenRes = res.size();

    for (size_t i = 0; i < lenRes; i++)
    {
        cout << "\n"
             << res[i].hours << ":" << res[i].minutes << ":" << res[i].seconds << '\n';
    }
    mt.unlock();
    return lenRes;
}

vector<times> GetHunk(vector<times> t, int startIndex, int endIndex)
{
    vector<times> res;
    if (startIndex == endIndex)
    {
        res.push_back(t[endIndex]);
        return res;
    }
    for (size_t i = startIndex; i <= endIndex; i++)
    {
        res.push_back(t[i]);
    }
    return res;
}

ostream &operator<<(ostream &os, vector<times> &vt)
{
    int len = vt.size();
    for (size_t i = 0; i < len; i++)
    {
        os << "\n"
           << vt[i].hours << ":" << vt[i].minutes << ":" << vt[i].seconds << '\n';
    }
    return os;
}

vector<times> InputTimes(int quantity)
{
    vector<times> inputTimes;
    for (size_t i = 0; i < quantity; i++)
    {

        cout << "hour ";
        int h;
        cin >> h;
        cout << "minutes ";
        int m;
        cin >> m;
        cout << "seconds ";
        int s;
        cin >> s;
        times t{h, m, s};
        inputTimes.push_back(t);
    }
    return inputTimes;
}

vector<times> InputTimes(string path)
{
    vector<times> inputTimes;
    ifstream is(path);
    if (!is.is_open())
    {
        exit(1);
    }
    while (!is.eof())
    {
        string buf;
        getline(is, buf);
        int len = buf.size();
        vector<int> OneStr;
        string oneNum;
        for (size_t i = 0; i < len; i++)
        {
            oneNum += buf[i];

            if (buf[i] == ' ')
            {
                int num = atoi(oneNum.c_str());
                OneStr.push_back(num);
                oneNum.clear();
            }
        }
        int num = atoi(oneNum.c_str());
        OneStr.push_back(num);

        times t{OneStr[0], OneStr[1], OneStr[2]};
        inputTimes.push_back(t);
    }
    return inputTimes;
}

int main(int argc, char *argv[])
{

    cout << "1-synchronous\n2-asinchronous\n";
    int numerOfProgram;
    cin >> numerOfProgram;

    switch (numerOfProgram)
    {
    case 1:
    {
        cout << "input quantity = ";
        int quantity;
        cin >> quantity;

        vector<times> inputTimes = InputTimes("/home/nik/Desktop/прога/lab4/timeTest/src/1.txt");
        int len;
        {
            Timer timer;
            len = timeCheck(inputTimes, inputTimes.size());
        }
        cout << "len " << len;
        break;
    }
    case 2:
    {
        cout << "input quantity = ";
        int quantity;
        cin >> quantity;

        vector<times> inputTimes = InputTimes("/home/nik/Desktop/прога/lab4/timeTest/src/1.txt" /*quantity*/);
        cout << "ввод\n"
             << inputTimes;

        cout << "threads = ";
        int countThreads;
        cin >> countThreads;

        vector<thread> threads;
        int startIndex = 0, endIndex = 0;
        int lenHunk = ceil((double)quantity / countThreads);
        int allLen = 0;
        Timer t;
        bool isEnd = false;
        vector<int> lens;
        for (size_t i = 0; i < countThreads; i++)
        {
            endIndex = lenHunk + endIndex;
            if (endIndex >= quantity)
            {
                while (endIndex > quantity)
                {
                    endIndex--;
                }
                isEnd = true;
            }
            vector<times> hunk = GetHunk(inputTimes, startIndex, endIndex - 1);
            // cout << hunk;
            threads.emplace_back([hunk, &lens]() mutable
                                 {
                                    lens.push_back(timeCheck(hunk, hunk.size())); 
                                 });


            startIndex = endIndex;
            if (isEnd)
                break;
        }
        await(threads);

        int sum = std::accumulate(lens.begin(), lens.end(), 0);
        cout << "quantitu " << sum;
        break;
    }

    default:
        break;
    }
}