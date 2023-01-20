#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <deque>
#include <condition_variable>

#include <semaphore.h>


std::mutex mtx;
std::deque<int> q;

std::atomic<int> globalVariable(0);

// int globalVariable = 0;

void task1(int a)
{
    for(int i = 0; i < 1000; i++)
    {    
        // std::lock_guard<std::mutex> lock(mtx1);
        // std::lock_guard<std::mutex> lock(mtx2);

        // mtx.lock(); 
        globalVariable++;
        globalVariable--;
        // mtx.unlock();

    }
}

void task2()
{
    for(int i = 0; i < 1000; i++)
    {    
        // std::lock_guard<std::mutex> lock(mtx1);
        // std::lock_guard<std::mutex> lock(mtx2);

        // mtx.lock(); 
        globalVariable++;
        globalVariable--;
        // mtx.unlock();
    }
}

int main()
{
    std::thread thread1(task1, 1);
    std::thread thread2(task1, 2);

    thread1.join();
    thread2.join();
    std::cout<<globalVariable;
    return 0;
}





// void func(int a)
// {
//         // std::cout << "Hello, World!\n";
//         std::this_thread::sleep_for(std::chrono::microseconds(50));
//         std::cout<< "this thread is "<< std::this_thread::get_id()<<"\n";
// }

// int main()
// {
//     int a = 0;
//     std::thread thread1(func, a);
//     std::cout<<"thread id is "<< thread1.get_id()<<"\n";
//     std::cout<<"currency is "<< thread1.hardware_concurrency()<<"\n";

//     thread1.join();

//     // while (true)
//     // {
//     //     std::cout << "Cat!\n";
//     //     std::this_thread::sleep_for(std::chrono::microseconds(50));
//     // }
    

//     // while (true)
//     // {
//     //     /* code */
//     // }
// }

