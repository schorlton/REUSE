//
// Created by Nolan Woods on 10/12/2018.
// Taken from https://stackoverflow.com/a/36763257
//

#ifndef REUSE_SHAREDQUEUE_H
#define REUSE_SHAREDQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class SharedQueue
{
    public:
        using ItemPointer = std::unique_ptr<T>;
        SharedQueue();
        ~SharedQueue();

        ItemPointer pop();

        void push(const T &item);
        void push(T &&item);

        unsigned int size(bool blocking = true);

        void signal_done();

    private:
        bool done;
        std::deque<T> queue_;
        std::mutex mutex_;
        std::condition_variable cond_;
};

#include "SharedQueue.cpp" //Move implementation into header due to template instantiation

#endif //REUSE_SHAREDQUEUE_H
