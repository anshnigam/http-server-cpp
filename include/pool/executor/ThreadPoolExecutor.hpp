#ifndef THREADPOOLEXECUTOR_HPP
#define THREADPOOLEXECUTOR_HPP

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <thread>
#include <type_traits>
#include <vector>
#include <memory>
#include <queue>
#include <stdexcept>
#include <utility>

class ThreadPoolExecutor {
    public:
        ThreadPoolExecutor(size_t numberOfThreads);
        ~ThreadPoolExecutor();

        template<class F, class... Args>
        auto enqueue(F&& f, Args&&... args) -> std::future<typename std::invoke_result_t<F, Args...>>;
        void shutdown();

    private:
        std::vector<std::thread> workers_;
        std::queue<std::function<void()>> tasks_;
        
        std::mutex queueMutex_;
        std::condition_variable queueCondition_;
        bool stop_;
};

inline ThreadPoolExecutor::ThreadPoolExecutor(size_t numberOfThreads) : stop_(false) {
    for (size_t i = 0 ; i < numberOfThreads ; ++i ) {
        workers_.emplace_back([this] {
            while (true) {
                std::function<void()> task;

                // critical section
                {
                    std::unique_lock<std::mutex> lock(this->queueMutex_);

                    this->queueCondition_.wait(lock, [this] {
                        return this->stop_ || !this->tasks_.empty();
                    });

                    if (this->stop_ && this->tasks_.empty()) {
                        return;
                    }

                    task = std::move(this->tasks_.front());
                    this->tasks_.pop();
                }

                task();
            }
        });
    }
}

inline ThreadPoolExecutor::~ThreadPoolExecutor() {
    shutdown();
}

inline void ThreadPoolExecutor::shutdown() {
    {
        std::unique_lock<std::mutex> lock(queueMutex_);
        stop_ = true;
    }

    queueCondition_.notify_all();

    for (std::thread& worker : workers_) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}


template<class F, class... Args>
auto ThreadPoolExecutor::enqueue(F&& f, Args&&... args) -> std::future<typename std::invoke_result_t<F, Args...>> {
   
    using returnType = typename std::invoke_result_t<F, Args...>;

    auto task = std::make_shared<std::packaged_task<returnType()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );

    std::future<returnType> result = task->get_future();

    {
        std::unique_lock<std::mutex> lock(queueMutex_);
        if (stop_) {
            throw std::runtime_error("Cannot submit tasks to a stopped ThreadPoolExecutor");
        }
        tasks_.emplace([task]() { (*task)(); });
    }

    queueCondition_.notify_one();
    return result;
}
#endif
