#pragma once

#include <thread>
#include <mutex>
#include <functional>
#include <queue>

class ThreadPool {
public:
    ThreadPool(size_t numThreads);

    ~ThreadPool();

    void enqueueTask(const std::function<void()> &task);

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop = false;
};