// Task.cpp

#include "Task.h"

Task::Task(const std::function<void()>& task, int timeout)
        : task(task), timeout(timeout) {}

void Task::perform_task() {
    if (timeout > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
    }
    task();
}