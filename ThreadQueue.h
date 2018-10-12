//
// Created by Nolan Woods on 10/12/2018.
// Taken from https://stackoverflow.com/a/36763257
//

#ifndef REUSE_THREADQUEUE_H
#define REUSE_THREADQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class SharedQueue
{
    public:
        SharedQueue();
        ~SharedQueue();

        T& front();
        void pop();

        void push(const T &item);
        void push(T &&item);

        int size();
        bool empty();

    private:
        std::deque<T> queue_;
        std::mutex mutex_;
        std::condition_variable cond_;
};

#endif //REUSE_THREADQUEUE_H
