#pragma once

#include <atomic>
#include <deque>
#include <functional>
#include <future>
#include <mutex>
#include <optional>
#include <semaphore>
#include <thread>

class ThreadPool
{
public:
    explicit ThreadPool(const auto &numberOfThreads = std::thread::hardware_concurrency())
        : mTasks(numberOfThreads)
    {
        for (auto id{0}; id < numberOfThreads; ++id) {
            mQueue.push_back(id);
            mThreads.emplace_back([=] {
                do {
                    mTasks[id].signal.acquire();

                    do {
                        while (!mTasks[id].tasks.empty()) {
                            auto task = mTasks[id].tasks.front();
                            mTasks[id].tasks.pop_front();
                            mUnassignedTasks.fetch_sub(1, std::memory_order_release);
                            std::invoke(std::move(task));
                            mOngoingTasks.fetch_sub(1, std::memory_order_release);
                        }

                    } while (mUnassignedTasks.load(std::memory_order_acquire) > 0);

                    if (mOngoingTasks.load(std::memory_order_acquire) == 0) {
                        mCompletedSignal.store(true, std::memory_order_release);
                        mCompletedSignal.notify_one();
                    }

                } while (mActive);
            });
        }
    }
    ThreadPool(const ThreadPool &) = delete;
    ThreadPool &operator=(const ThreadPool &) = delete;
    ~ThreadPool()
    {
        mActive = false;
        waitUntilDone();

        for (auto i{0}; i < mThreads.size(); ++i) {
            // threads_[i].request_stop();
            mTasks[i].signal.release();
            mThreads[i].join();
        }
    }

    [[nodiscard]] std::atomic_int &unassignedTasks() { return mUnassignedTasks; }

    void waitUntilDone()
    {
        if (mOngoingTasks.load(std::memory_order_acquire) > 0) {
            mCompletedSignal.wait(false);
        }
    }

    [[nodiscard]] auto enqueue(auto func, auto... args)
    {
        auto promise = std::make_shared<std::promise<std::invoke_result_t<decltype(func), decltype(args)...>>>();
        auto task = [func = std::move(func), ... args = std::move(args), promise = promise]() {
            try {
                promise->set_value(func(args...));
            } catch (...) {
                promise->set_exception(std::current_exception());
            }
        };

        auto future = promise->get_future();
        auto addTask = [this](auto &&func) {
            if (mQueue.empty()) {
                return;
            }

            auto id = mQueue.front();
            mQueue.pop_front();
            mQueue.push_back(id);

            mUnassignedTasks.fetch_add(1, std::memory_order_release);
            mTasks[id].tasks.push_back(std::forward<decltype(func)>(func));
            mTasks[id].signal.release();
        };

        addTask(std::move(task));
        return future;
    }

private:
    struct Task
    {
        std::deque<std::function<void()>> tasks;
        std::function<void()> task;
        std::binary_semaphore signal{0};
    };

    std::vector<std::thread> mThreads;
    std::deque<Task> mTasks;
    std::deque<std::size_t> mQueue;
    std::atomic_int mUnassignedTasks{0};
    std::atomic_int mOngoingTasks{0};
    std::atomic_bool mCompletedSignal{false};
    std::atomic_bool mActive{true};
};