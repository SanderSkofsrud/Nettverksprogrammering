#ifndef EXERCISE_2_TASK_H
#define EXERCISE_2_TASK_H
#include <functional>
#include <chrono>
#include <thread>

/**
 * Class representing a task that can be executed by a worker thread.
 */
class Task{
public:
    int timeout = 0; // Time in milliseconds to wait before executing the task
    std::function<void()> task; // The function to be executed as the task

    /**
     * Default constructor.
     */
    Task() = default;

    /**
     * Constructor to create a task with a specific function and timeout.
     *
     * @param task The function to be executed.
     * @param timeout The time in milliseconds to wait before executing the task.
     */
    Task(const std::function<void()>& task, const int timeout);

    /**
     * Executes the task after waiting for the specified timeout.
     */
    void perform_task();

private:
    /**
     * Waits for the specified timeout duration.
     */
    void call_timeout() const;
};
#endif //EXERCISE_2_TASK_H
