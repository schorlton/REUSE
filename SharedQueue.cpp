//
// Created by Nolan Woods on 10/13/2018.
//

#include "SharedQueue.h"

template <typename T>
SharedQueue<T>::SharedQueue() : done(false) {}

template <typename T>
SharedQueue<T>::~SharedQueue() = default;

template <typename T>
typename SharedQueue<T>::ItemPointer SharedQueue<T>::pop()
{
    std::unique_lock<std::mutex> mlock(mutex_);
    while (queue_.empty())
    {
        cond_.wait(mlock, [this]{return done;});
        if (done) {
            SharedQueue<T>::ItemPointer none;
            return none;
        }
    }
    ItemPointer item;
    std::swap(item, queue_.front());
    queue_.pop_front();
    return item;
}

template <typename T>
void SharedQueue<T>::push(ItemPointer item)
{
    std::unique_lock<std::mutex> mlock(mutex_);
    queue_.push_back(std::move(item));
    mlock.unlock();     // unlock before notificiation to minimize mutex con
    cond_.notify_one(); // notify one waiting thread

}

template <typename T>
void SharedQueue<T>::signal_done() {
    this->done = true;
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