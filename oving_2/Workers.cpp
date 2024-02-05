#include "Workers.h"
#include "Task.h"
#include <thread>
#include <condition_variable>

/**
 * Submits a task without any delay.
 *
 * @param task A std::function representing the task to be executed.
 */
void Workers::post(const std::function<void()>& task) {
    // Calls post_timeout with a timeout of 0, effectively posting a task without delay
    post_timeout(task, 0);
}

/**
 * Submits a task with a specified delay.
 *
 * @param task A std::function representing the task to be executed.
 * @param timeout The delay in milliseconds before the task is executed.
 */
void Workers::post_timeout(const std::function<void()>& task, const int timeout) {
    // Locks the mutex to ensure thread-safe access to the task queue
    std::unique_lock<std::mutex> lock(this->task_mutex);
    // Emplaces the task and its timeout at the back of the task queue
    this->task_queue.emplace_back(task, timeout);
    // Notifies one waiting thread, if any
    this->cv.notify_one();
}

/**
 * Starts the worker threads.
 */
void Workers::start() {
    // Creates threads based on the number of threads specified in num_threads
    for(int i = 0; i < this->num_threads; i++) {
        this->thread_pool.emplace_back([&] {
            while(true) {
                Task task{};
                {
                    // Locks the mutex to safely access the task queue
                    std::unique_lock<std::mutex> lock(this->task_mutex);
                    // Waits while the task queue is empty and running is true
                    while(this->task_queue.empty()) {
                        if(!running) return;
                        this->cv.wait(lock);
                    }
                    // Retrieves and removes the first task from the queue
                    task = *task_queue.begin();
                    task_queue.pop_front();
                }
                // Notifies one waiting thread, if any
                this->cv.notify_one();
                // Executes the task if it's valid
                if (task.task) {
                    task.perform_task();
                }
            }
        });
    }
}

/**
 * Stops all worker threads after completing all tasks.
 */
void Workers::stop() {
    // Locks the mutex to safely modify shared resources
    std::unique_lock<std::mutex> lock(this->task_mutex);
    // Waits until the task queue is empty
    while(!this->task_queue.empty()) {
        this->cv.wait(lock);
    }
    // Sets the running flag to false, indicating no more tasks should be processed
    this->running = false;
    // Notifies all waiting threads
    cv.notify_all();
}

/**
 * Joins all worker threads, ensuring they complete before proceeding.
 */
void Workers::join() {
    // Creates a thread to handle the stopping and joining of worker threads
    std::thread t([&] {
        // Stops all worker threads
        this->stop();
        // Joins each thread in the thread pool
        for(auto& thread: this->thread_pool) {
            thread.join();
        }
    });
    // Joins the managing thread
    t.join();
}

// The namespace and function below are user-defined and specific to the context of this code
namespace oving_2 {
    /**
     * Entry point to demonstrate the usage of Workers class.
     */
    void run() {
        // Create worker threads and an event loop with specified number of threads
        Workers worker_threads(4); // 4 worker threads
        Workers event_loop(1);     // 1 thread for event loop

        // Start the threads
        worker_threads.start(); // Create 4 internal threads
        event_loop.start();     // Create 1 internal thread

        // Posting tasks to worker_threads and event_loop
        worker_threads.post([]{
            std::cout << "Task A\n"; // Task A
        });
        worker_threads.post([]{
            std::cout << "Task B\n"; // Task B
        });
        event_loop.post([]{
            std::cout << "Task C\n"; // Task C
        });
        event_loop.post([] {
            std::cout << "Task D\n"; // Task D
        });
        worker_threads.post_timeout([]{
            std::cout << "Task E\n"; // Task E
        }, 1000);

        // Joining all threads
        worker_threads.join(); // Calls join() on the worker threads
        event_loop.join();     // Calls join() on the event thread
    }
}
