#ifndef EXERCISE_2_WORKERS_H
#define EXERCISE_2_WORKERS_H
#include <functional>
#include <iostream>
#include <list>
#include <vector>
#include <thread>
#include <condition_variable>
#include "Task.h"

namespace oving_2 {
    void run();
}

/**
 * Class representing a pool of worker threads that can execute tasks asynchronously.
 */
class Workers{

    int num_threads; // Number of threads in the pool
    std::list<Task> task_queue; // Queue of tasks to be executed
    std::vector<std::thread> thread_pool; // Pool of worker threads
    std::condition_variable cv; // Condition variable for thread synchronization
    std::mutex task_mutex; // Mutex for protecting the task queue
    bool running = true; // Flag to control the running state of threads

public:
    /**
     * Constructor for Workers class.
     *
     * @param num_threads Number of threads to be created in the pool.
     */
    explicit Workers(int num_threads);

    // Function prototypes
    void post(const std::function<void()>&);
    void post_timeout(const std::function<void()>&, int);
    void start();
    void stop();
    void join();
};
#endif //EXERCISE_2_WORKERS_H
