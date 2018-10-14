//
// Created by Nolan Woods on 10/13/2018.
//
#include "SharedQueue.h"

template <typename T>
SharedQueue<T>::SharedQueue() = default;

template <typename T>
SharedQueue<T>::~SharedQueue() = default;

template <typename T>
T& SharedQueue<T>::pop(bool& predicate)
{
    std::unique_lock<std::mutex> mlock(mutex_);
    while (queue_.empty())
    {
        cond_.wait(mlock);
    }
    T& item = queue_.front();
    queue_.pop_front();
    return item;
}

template <typename T>
void SharedQueue<T>::push(const T &item)
{
    std::unique_lock<std::mutex> mlock(mutex_);
    queue_.push_back(item);
    mlock.unlock();     // unlock before notificiation to minimize mutex con
    cond_.notify_one(); // notify one waiting thread

}

template <typename T>
void SharedQueue<T>::push(T &&item)
{
    std::unique_lock<std::mutex> mlock(mutex_);
    queue_.push_back(std::move(item));
    mlock.unlock();     // unlock before notificiation to minimize mutex con
    cond_.notify_one(); // notify one waiting thread

}

template <typename T>
unsigned int SharedQueue<T>::size(bool blocking)
{
    if (blocking) {
        std::unique_lock<std::mutex> mlock(mutex_);
        int size = queue_.size();
        mlock.unlock();
        return size;
    } else {
        return queue_.size();
    }
}