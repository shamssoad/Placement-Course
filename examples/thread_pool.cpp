// C++17. Teaching pool: drains accepted tasks on destruction.
#include <condition_variable>
#include <cstddef>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

class ThreadPool {
    std::mutex mutex_;
    std::condition_variable ready_;
    std::queue<std::function<void()>> jobs_;
    bool stopping_ = false;
    std::vector<std::thread> workers_;

    void work() {
        for (;;) {
            std::function<void()> job;
            {
                std::unique_lock<std::mutex> lock(mutex_);
                ready_.wait(lock, [this] { return stopping_ || !jobs_.empty(); });
                if (stopping_ && jobs_.empty()) return;
                job = std::move(jobs_.front());
                jobs_.pop();
            }
            job(); // Submitted work is wrapped in packaged_task.
        }
    }

    void stopAndJoin() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            stopping_ = true;
        }
        ready_.notify_all();
        for (auto& worker : workers_)
            if (worker.joinable()) worker.join();
    }

public:
    explicit ThreadPool(std::size_t count) {
        if (count == 0) throw std::invalid_argument("Need at least one worker");
        workers_.reserve(count);
        try {
            for (std::size_t i = 0; i < count; ++i)
                workers_.emplace_back([this] { work(); });
        } catch (...) {
            stopAndJoin(); // Clean up workers if thread creation partially fails.
            throw;
        }
    }
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ~ThreadPool() { stopAndJoin(); }

    template<class Function>
    auto submit(Function&& function)
        -> std::future<std::invoke_result_t<std::decay_t<Function>&>> {
        using Result = std::invoke_result_t<std::decay_t<Function>&>;
        auto task = std::make_shared<std::packaged_task<Result()>>(
            std::forward<Function>(function));
        auto result = task->get_future();
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (stopping_) throw std::runtime_error("Pool is stopping");
            jobs_.emplace([task] { (*task)(); });
        }
        ready_.notify_one();
        return result;
    }
};

int main() {
    ThreadPool pool(2);
    std::vector<std::future<int>> results;
    for (int i = 0; i < 8; ++i)
        results.push_back(pool.submit([i] { return i * i; }));
    for (auto& result : results) std::cout << result.get() << ' ';
    std::cout << '\n';

    auto failure = pool.submit([]() -> int {
        throw std::runtime_error("Example task failure");
    });
    try {
        std::cout << failure.get() << '\n';
    } catch (const std::exception& exception) {
        std::cout << "Caught through future: " << exception.what() << '\n';
    }
}
