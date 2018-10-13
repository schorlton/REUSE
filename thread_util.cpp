//
// Created by Nolan Woods on 10/13/2018.
//

#include "thread_util.h"

#ifdef _WIN32

#elif __APPLE__
    #include "TargetConditionals.h"
#elif __linux__
    #include <pthread.h>
    #include <sched.h>
#endif


void increment_priority(std::thread& thread, int amount) {
#ifdef _WIN32
    //TODO implement windows functionality
    //SetThreadPriority()
#elif __APPLE__
#include "TargetConditionals.h"
    #if TARGET_OS_MAC
        // TODO implement OSX functionality
    #else
    #   error "Unknown Apple platform"
    #endif
#elif __linux__
    int policy;
    sched_param sch_params;
    int errno;
    if(errno = pthread_getschedparam(thread.native_handle(), &policy, &sch_params)) {
        std::cerr << "Failed to get thread scheduling: " << std::strerror(errno) << std::endl;
        return;
    }
    sch_params.sched_priority += amount;
    int max = sched_get_priority_max(policy);
    int min = sched_get_priority_min(policy);
    if (max <= sch_params.sched_priority && min >= sch_params.sched_priority) {
        std::cerr << "Thread priority of " << sch_params.sched_priority << " out of range: " << min << " to " max << std::endl;
        return;
    }
    if(errno = pthread_setschedparam(thread.native_handle(), policy, &sch_params)) {
        std::cerr << "Failed to set thread scheduling: " << std::strerror(errno) << std::endl;
    }
#endif
}