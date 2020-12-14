// Created by Ivan Martsilenko SADMT - 4
// lab 4 Theory and practice of lenguage proccesors
// task 2 blok 1

#include <iostream>
#include <unistd.h>
#include <thread>  
#include <mutex>
#include <deque>
#include <condition_variable>

std::mutex outdoor;
std::mutex indoor;
std::condition_variable notifyOutdoor;
std::condition_variable notifyIndoor;
std::deque<int> outdoorQueue;
std::deque<int> indoorQueue;
const int seats = 7;

void clientGenerator(int quantity)
{
        while(quantity)
    {
        std::unique_lock<std::mutex> locker1(outdoor);
        notifyOutdoor.wait( locker1);
        outdoorQueue.push_back(quantity);
        std::cout << "User: " << quantity << " is waiting outside" << std::endl;
        quantity--;
        locker1.unlock();
        notifyOutdoor.notify_one();
        usleep(rand() % 5000);
    }
}

static void waitingRoom()
{
    int user;
    while (true)
    {
        std::unique_lock<std::mutex> locker1(outdoor);
        notifyOutdoor.wait( locker1);
        user = outdoorQueue.back();
        outdoorQueue.pop_back(); 
        locker1.unlock();
        notifyOutdoor.notify_one();

        std::unique_lock<std::mutex> locker2(indoor);
        notifyIndoor.wait( locker2, [](){return indoorQueue.size() < seats;});
        indoorQueue.push_back(user);
        std::cout << "User " << user << " is waiting inside barber shop" << std::endl;
        locker2.unlock();
        notifyIndoor.notify_one();
    }
}

void hairdresser(int val)
{
    int user;
    while (true)
    {
        std::unique_lock<std::mutex> locker2(indoor);
        notifyIndoor.wait( locker2);
        user = indoorQueue.back();
        indoorQueue.pop_back();
        locker2.unlock();
        std::cout << "User " << user << " is service" << std::endl;
        notifyIndoor.notify_one();
        usleep(val);
    }
}

int main()
{
    std::cout << "lab 4" << std::endl;
    std::cout << "Plaese input number of clients" << std::endl;
    int number;
    std::cin >> number; 
    std::thread generator(clientGenerator, number);
    std::thread room(waitingRoom);
    std::cout << "Plaese input duration of hircut" << std::endl;
    int duration;
    std::cin >> duration; 
    std::thread worker(hairdresser, duration);
    
    generator.join();
    room.join();
    worker.join();
    
    std::cout << "Finished" << std::endl;
    return 0;
}