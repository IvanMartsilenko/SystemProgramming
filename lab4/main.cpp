// Created by Ivan Martsilenko SADMT - 4
// lab 4 Theory and practice of lenguage proccesors
// task 2 blok 1

#include <iostream>
#include <unistd.h>
#include <thread>  
#include <mutex>
#include <deque>
#include <condition_variable>

std::mutex service;
std::mutex outdoor;
std::mutex indoor;
std::condition_variable notifyService;
std::condition_variable notifyOutdoor;
std::condition_variable notifyIndoor;
std::deque<int> serving;
std::deque<int> outdoorQueue;
std::deque<int> indoorQueue;
const int chair = 1; 
const int seats = 7;
const int maxBufferSize = 20 ;

void clientGenerator (int val)
{
    while (val)
    {
        if (outdoorQueue.size() < 1&& indoorQueue.size() < 1  && serving.size() < chair)
        {
            std::unique_lock<std::mutex> lockerHairdresser(service);
            notifyService.wait(lockerHairdresser, [](){return serving.size() < chair;});
            serving.push_back(val);
            lockerHairdresser.unlock();
            notifyService.notify_one();
        }
        else if (outdoorQueue.size() < 1 && indoorQueue.size() < seats)
        {
            std::unique_lock<std::mutex> lockerIndoor(indoor);
            notifyIndoor.wait(lockerIndoor, [](){return indoorQueue.size() < seats;});
            std::cout << "User " << val << " is waiting inside barber shop" << std::endl;
            indoorQueue.push_back(val);
            lockerIndoor.unlock();
            notifyIndoor.notify_one();
        }
        else 
        {
            std::unique_lock<std::mutex> lockerOutdoor(outdoor);
            notifyOutdoor.wait(lockerOutdoor, [](){return outdoorQueue.size() < maxBufferSize;});
            outdoorQueue.push_back(val);
            std::cout << "User " << val << " is waiting outside" << std::endl;
            lockerOutdoor.unlock();
            notifyOutdoor.notify_one();
        }
        val--;
        usleep(rand() % 15000);
    }
}

static void waitingRoom()
{
    int user;
    while (true)
    {
        std::unique_lock<std::mutex> lockerIndoor(indoor);
        std::unique_lock<std::mutex> lockerOutdoor(outdoor);
        std::unique_lock<std::mutex> lockerHairdresser(service);
        if (serving.size() < chair)
        {
            notifyService.wait(lockerHairdresser, [](){return serving.size() < chair;});
            serving.push_back(indoorQueue.back());
            indoorQueue.pop_back();
            lockerHairdresser.unlock();
            notifyService.notify_one();
        }
        if( indoorQueue.size() < seats)
        {
            notifyIndoor.wait(lockerIndoor, [](){return indoorQueue.size() >= seats  - 1 ;});
            notifyOutdoor.wait(lockerOutdoor, [](){return indoorQueue.size() >= seats  - 1;});
            user = outdoorQueue.back();
            indoorQueue.push_back(user);
            outdoorQueue.pop_back();
            std::cout << "User " << user << " is waiting inside barber shop" << std::endl;
            lockerIndoor.unlock();
            notifyService.notify_one();
            lockerOutdoor.unlock();
            notifyOutdoor.notify_one();
        }


    }
}


void hairdresser(int duration)
{
    int user;
    while(true)
    {
        std::unique_lock<std::mutex> lockerHairdresser(service);
        notifyService.wait(lockerHairdresser, [](){return serving.size() > 0;});
        user = serving.back();
        serving.pop_back();
        lockerHairdresser.unlock();
        std::cout << "User " << user << " is serving" << std::endl;
        usleep(duration);
        notifyService.notify_one();
    }

}

int main()
{
    std::cout << "lab 4" << std::endl;
    std::cout << "Plaese input number of clients" << std::endl;
    int number;
    std::cin >> number; 
    std::cout << "Plaese input duration of haircut" << std::endl;
    int duration;
    std::cin >> duration; 
    std::thread generator(clientGenerator, number);
    std::thread room(waitingRoom);
    std::thread worker(hairdresser, duration);
    
    generator.join();
    room.join();
    worker.join();
    
    std::cout << "Finished" << std::endl;
    return 0;
}