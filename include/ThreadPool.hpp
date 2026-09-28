#pragma once
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

class ThreadPool {
public:
    explicit ThreadPool(size_t num_threads);
    ~ThreadPool();

    void enqueue(std::function<void()> task);
    void wait_until_idle();

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> task_queue;

    std::mutex queue_mutex;
    std::condition_variable cv_has_task;
    std::condition_variable cv_idle;

    std::atomic<bool> stop_flag{false};
    std::atomic<int> active_tasks{0};
};
