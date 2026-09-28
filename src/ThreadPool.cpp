#include "ThreadPool.hpp"

ThreadPool::ThreadPool(size_t num_threads) {
    for (size_t i = 0; i < num_threads; ++i) {
        workers.emplace_back([this]() {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(this->queue_mutex);
                    this->cv_has_task.wait(lock, [this]() {
                        return this->stop_flag || !this->task_queue.empty();
                    });

                    if (this->stop_flag && this->task_queue.empty()) {
                        return;
                    }

                    task = std::move(this->task_queue.front());
                    this->task_queue.pop();
                    this->active_tasks++;
                }

                task();

                {
                    std::unique_lock<std::mutex> lock(this->queue_mutex);
                    this->active_tasks--;
                    if (this->task_queue.empty() && this->active_tasks == 0) {
                        this->cv_idle.notify_all();
                    }
                }
            }
        });
    }
}

void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        task_queue.push(std::move(task));
    }
    cv_has_task.notify_one();
}

void ThreadPool::wait_until_idle() {
    std::unique_lock<std::mutex> lock(queue_mutex);
    cv_idle.wait(lock, [this]() {
        return this->task_queue.empty() && this->active_tasks == 0;
    });
}

ThreadPool::~ThreadPool() {
    stop_flag = true;
    cv_has_task.notify_all();
    for (std::thread &worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}
