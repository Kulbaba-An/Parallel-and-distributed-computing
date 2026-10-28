//=========================== Завдання 1.2.1 =================================

#include <iostream>
#include <thread>
#include <mutex>
#include <Windows.h>
#include <condition_variable>
#include <queue>

using namespace std;

queue<int> queue1;
mutex mutexForQueue;

bool flag;

void DataPreparationFunc() {

    unique_lock<mutex> lock1(mutexForQueue);

    int x;
    cout << "Enter numbers (^Z + Enter - exit) (space - separator): ";
    while (cin >> x) {
        queue1.push(x);
    }
    flag = true;
}

void DataProcessingFunc() {

    unique_lock<mutex> lock1(mutexForQueue);

    while (!flag) {
        lock1.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        lock1.lock();
    }

    cout << "Queue: ";
    while (!queue1.empty()) {
        cout << queue1.front() << " ";
        queue1.pop();
    }
}


int main()
{
    thread DataPreparation(DataPreparationFunc);
    thread DataProcessing(DataProcessingFunc);

    DataPreparation.detach();
    DataProcessing.join();
}

//=========================== Завдання 1.2.2 =================================

#include <iostream>
#include <thread>
#include <mutex>
#include <Windows.h>
#include <condition_variable>
#include <queue>

using namespace std;

int i = 0;
mutex m;
condition_variable cv;

void waits() {

    unique_lock<mutex> lock(m);

    cout << "Start waiting\n";
    cv.wait(lock, [] {return i == 1; });
    cout << "Finish waiting\n";
}

void awakeFunction() {

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    {
        unique_lock<mutex> lock(m);
        cout << "Notify all!\n";
    }
    cv.notify_all();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    {
        unique_lock<mutex> lock(m);
        i = 1;
        cout << "Notify all (now i =1)!\n";
    }
    cv.notify_all();
}

int main()
{
    thread t1(waits);
    thread t2(waits);
    thread t3(waits);
    thread awake(awakeFunction);

    t1.join();
    t2.join();
    t3.join();
    awake.join();
}

//=========================== Завдання 1.2.3 =================================

#include <iostream>
#include <thread>
#include <mutex>
#include <Windows.h>
#include <condition_variable>
#include <queue>

using namespace std;

int i = 0;
mutex m;
condition_variable cv;

void waits() {

    unique_lock<mutex> lock(m);

    cout << "Start waiting\n";
    cv.wait(lock, [] {return i != 0; });
    cout << "Finish waiting i = " << i << endl;
}

void notifyFunction() {

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    {
        unique_lock<mutex> lock(m);
        cout << "Notify all!\n";
    }
    cv.notify_all();

    for (int j = 1; j <= 3; j++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        {
            unique_lock<mutex> lock(m);
            i = j;
            cout << "Notify one (with changed i)!\n";
        }
        cv.notify_one();
    }
}

int main()
{
    thread Thread1(waits);
    thread Thread2(waits);
    thread Thread3(waits);

    thread notify(notifyFunction);

    Thread1.join();
    Thread2.join();
    Thread3.join();

    notify.join();
}

//=========================== Завдання 1.2.4 =================================

#include <iostream>
#include <thread>
#include <mutex>
#include <Windows.h>
#include <condition_variable>
#include <queue>

using namespace std;

queue<int> queue1;
mutex mutexForQueue;

condition_variable cv;

void DataPreparationFunc() {

    unique_lock<mutex> lock1(mutexForQueue);

    int x;

    cout << "Enter numbers (^Z + Enter - exit) (space - separator): ";
    while (cin >> x) {
        queue1.push(x);
    }
    cv.notify_all();
}

void DataProcessingFunc() {

    unique_lock<mutex> lock1(mutexForQueue);

    cv.wait(lock1, [] {return !(queue1.empty()); });

    cout << "Queue: ";
    while (!queue1.empty()) {
        cout << queue1.front() << " ";
        queue1.pop();
    }
}


int main()
{
    thread DataPreparation(DataPreparationFunc);
    thread DataProcessing(DataProcessingFunc);

    DataPreparation.detach();
    DataProcessing.join();
}

//=========================== Завдання 1.2.5 =================================

#include <iostream>
#include <thread>
#include <mutex>
#include <Windows.h>
#include <condition_variable>
#include <queue>
#include <future>
#include <string>
#include <cmath>

using namespace std;

int findPrime(int const x) {

    vector<int> vec;

    vec.push_back(2);
    vec.push_back(3);

    bool flag = false;
    int i = 2;
    int num = 4;

    for (; i < x; num++) {
        for (int element : vec) {
            if (num % element == 0) {
                flag = true;
                break;
            }
            else if (num < element * element) {
                break;
            }
        }
        if (flag) {
            flag = false;
            continue;
        }
        i++;
        vec.push_back(num);
    }
    return vec.back();
}

double func(string f, int n) {
    if (f == "sine") {
        return sin(n);
    }
    else if (f == "root") {
        return sqrt(n);
    }
    else if (f == "logarithm") {
        return log(n);
    }
    else {
        return 0;
    }
}

int main()
{
    int n = 0;

    cout << "Number :";
    cin >> n;

    future<int> result1 = async(std::launch::deferred, findPrime, n);

    string f;

    cout << "Func name(sine,root,logarithm): ";
    cin >> f;

    while (f != "root" && f != "sine" && f != "logarithm") {
        cout << "Nope, Try again: ";
        cin >> f;
    }

    future<double> result2 = async(std::launch::async, func, f, n);

    cout << "Result 1: " << result1.get() << endl;
    cout << "Result 2:" << result2.get();
}

//=========================== Завдання 1.2.6 =================================

#include <iostream>
#include <deque>
#include <future>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <string>
#include <sstream>
#include <windows.h>


using namespace std;

int findPrime(int const x) {

    vector<int> vec;

    vec.push_back(2);
    vec.push_back(3);

    bool flag = false;
    int i = 2;
    int num = 4;

    for (; i < x; num++) {
        for (int element : vec) {
            if (num % element == 0) {
                flag = true;
                break;
            }
            else if (num < element * element) {
                break;
            }
        }
        if (flag) {
            flag = false;
            continue;
        }
        i++;
        vec.push_back(num);
    }
    return vec.back();
}
bool isNumber(const string& s) {
    return !s.empty() &&
        all_of(s.begin(), s.end(), [](unsigned char c) {
        return isdigit(c);
            });
}


deque<packaged_task<int()>> taskQ;

deque<int> numberQ;
bool stop = false;

mutex m;
condition_variable cv;

void doWork() {
    while (true) {
        packaged_task<int()> t;

        {
            unique_lock<mutex> locker(m);

            cv.wait(locker, [] {
                return stop || !taskQ.empty();
                });

            if (taskQ.empty() && stop) {
                return;
            }

            t = move(taskQ.front());
            taskQ.pop_front();
            numberQ.pop_front();
        }

        t();
    }
}

int main() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    future<int> resultFuture;

    thread worker(doWork);

    string input;
    int n;

    while (true) {
        cout << "\nВведіть n (q для завершення): ";
        cin >> input;

        if (input == "q" || input == "Q") {
            break;
        }
        else if (isNumber(input)) {
            lock_guard<mutex> lock(m);
            numberQ.push_back(stoi(input));
        }
    }

    for (int i : numberQ) {

        packaged_task<int()> task(
            [i]() {
                return findPrime(i);
            }
        );

        future<int> f = task.get_future();

        {
            unique_lock<mutex> lock(m);
            taskQ.push_back(move(task));

        }

        cv.notify_all();
        cout << f.get() << endl;
    }

    {
        unique_lock<mutex> lock(m);
        stop = true;
        worker.join();
    }
}


//=========================== Завдання 1.2.7 =================================
#include <iostream>
#include <thread>
#include <mutex>
#include <Windows.h>
#include <condition_variable>
#include <queue>
#include <future>
#include <string>
#include <cmath>

using namespace std;

int findPrime(int const x) {

    vector<int> vec;

    vec.push_back(2);
    vec.push_back(3);

    bool flag = false;
    int i = 2;
    int num = 4;

    for (; i < x; num++) {
        for (int element : vec) {
            if (num % element == 0) {
                flag = true;
                break;
            }
            else if (num < element * element) {
                break;
            }
        }
        if (flag) {
            flag = false;
            continue;
        }
        i++;
        vec.push_back(num);
    }
    return vec.back();
}
int findRoot(int x) {
    return sqrt(x);
}

void func1(int const x, promise<int> prom1, promise<bool> prom2) {
    prom1.set_value(findPrime(x));
    prom2.set_value(true);
}


void func2(future<bool> boolValue, int const x, promise<int> prom) {
    boolValue.get();

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    prom.set_value(findRoot(x));
}

int main()
{
    int n = 0;

    cout << "Number :";
    cin >> n;

    promise<int> promise1;
    promise<bool> promise2;

    promise<int> promise3;

    future<int> result1 = promise1.get_future();
    future<bool> boolValue = promise2.get_future();

    future<int> result2 = promise3.get_future();

    thread thread1(func1, n, move(promise1), move(promise2));
    thread thread2(func2, move(boolValue), n, move(promise3));

    cout << "Result 1 = " << result1.get() << endl;
    cout << "Result 2 = " << result2.get() << endl;



    thread1.join();
    thread2.join();
}
