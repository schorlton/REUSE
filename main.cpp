#include <iostream>
#include <vector>
#include <thread>


#include "SharedQueue.h"

using Record = char*;
using Queue = SharedQueue<Record>;

//Thread interface declaration
void filter(Queue, Queue);
void output(Queue);

int main() {
    std::cerr << "Hello, World!" << std::endl;
    uint queue_limit = 10; //Default soft limit for queue before thread pool increase
    //Parse and validate parameters

    //Init thread pool
    Queue pending_records, output_records;
    std::vector<std::thread> thread_pool;
    thread_pool.emplace(thread_pool.end(), filter, pending_records, output_records);
    thread_pool.emplace(thread_pool.end(), output, output_records);

    //Read in records to queue

    //Check queue size and increase thread pool to desaturate
    if (pending_records.size() > queue_limit) thread_pool.emplace(thread_pool.end(), filter, pending_records, output_records);

    //Join thread pool
    for (auto& thread : thread_pool) thread.join();

    //Output statistics

    return 0;
}