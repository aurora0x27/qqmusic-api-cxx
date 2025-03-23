/**----------------------------------qqmusic/utils/async-executor.h---------------------------------
 *
 * @file qqmusic/utils/async-executor.h
 *
 * @brief 提供了一个默认的简单协程调度器, 以及同步执行异步函数的方法.
 *
 * @date 2025-3-21
 *
 *------------------------------------------------------------------------------------------------*/
#ifndef QQMUSIC_UTILS_ASYNC_EXECUTOR_H
#define QQMUSIC_UTILS_ASYNC_EXECUTOR_H

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/use_future.hpp>
#include <exception>
#include <qqmusic/result.h>
#include <stdexcept>
#include <thread>
#include <utility>

namespace qqmusic::utils {

/**
 * @brief Result<>不能进行拷贝, 也没有默认构造函数. boost::asio::co_spawn中会调用默认构造函数.
 *        所以需要包装原本的异步任务, 使用移动构造函数来构造result
 *
 * @param aw 异步任务
 * @param handler 回调函数
 *
 * @note 回调函数的签名是`void(std::exception_ptr, std::optional<T>)`, 前一个参数是处理异常的,
 *       后一个参数是正常的返回结果. 如果出错, 则返回结果的`std::optional`置空
 *
 * @see qqmusic::Result
 * */
/*custom co_spawn entry point to avoid using deleted default Result<T> constructor*/
template<typename T, typename Executor>
boost::asio::awaitable<void> custom_co_spawn_entry_point(
    boost::asio::awaitable<T, Executor> aw,
    std::function<void(std::exception_ptr, std::optional<T>)> handler) {
    try {
        T result = co_await std::move(aw);
        handler(nullptr, std::move(result));
    } catch (...) {
        handler(std::current_exception(), std::optional<T>());
    }
}

/**
 * @brief 同步执行异步函数
 *
 * @param ioc 当前io_context引用
 * @param task 异步任务
 *
 * @return 返回执行结果
 *
 * @throw std::runtime_error, 当执行出现异常或没有返回预期值
 * */
template<typename T>
T sync_exec(boost::asio::io_context& ioc, qqmusic::Task<T> task) {
    /*synchronously execute the task and get reslt, need io_context*/
    std::optional<T> result;
    std::exception_ptr exception;

    std::function<void(std::exception_ptr, std::optional<T>)> handler =
        [&](std::exception_ptr e, std::optional<T> value) {
            if (e) {
                exception = e;
            } else {
                result.emplace(std::move(*value));
            }
        };

    boost::asio::co_spawn(ioc,
                          custom_co_spawn_entry_point(std::move(task), handler),
                          boost::asio::detached);

    ioc.run();

    if (exception) {
        std::rethrow_exception(exception);
    }

    if (!result.has_value()) {
        throw std::runtime_error("Expect result has a value");
    }

    return std::move(*result);
}

/**
 * @brief 同步执行异步函数
 *
 * @param task 异步任务
 *
 * @return 返回执行结果
 * */
template<typename T>
T sync_exec(qqmusic::Task<T> task) {
    /*sync get result*/
    boost::asio::io_context ioc;
    return sync_exec(ioc, std::move(task));
}

/**
 * @class qqmusic::utils::AsyncExecutor
 *
 * @brief全局调度器类
 * */
class AsyncExecutor {
    /*Global AsyncExecutor class*/
public:
    AsyncExecutor(const AsyncExecutor&) = delete;
    AsyncExecutor& operator=(const AsyncExecutor&) = delete;

    /**
     * @brief 获取全局单例引用
     *
     * @return 全局调度器实例引用
     * */
    static AsyncExecutor& get_instance();

    /**
     * @brief 异步执行任务, 并在任务结束后触发回调
     *
     * @param task 异步任务
     * @param callback 回调函数, 会将`task`的执行结果传递给回调函数
     *                 回调函数不返回任何值
     * */
    template<typename T, BOOST_ASIO_COMPLETION_TOKEN_FOR(void(T&&)) CompletionCallback>
    void async_exec(qqmusic::Task<T> task, CompletionCallback&& callback) {
        std::optional<T> result;
        std::exception_ptr exception;

        std::function<void(std::exception_ptr, std::optional<T>)> handler =
            [&](std::exception_ptr e, std::optional<T> value) {
                if (e) {
                    exception = e;
                } else {
                    result.emplace(std::move(*value));
                }
            };

        boost::asio::co_spawn(ioc,
                              custom_co_spawn_entry_point(std::move(task), handler),
                              boost::asio::detached);

        callback(std::move(result.value()));
    }

    /**
     * @brief 异步执行任务函数的void特化, 并在任务结束后触发回调
     *
     * @param task 异步任务
     * @param callback 回调函数, 会将`task`的执行结果传递给回调函数
     *                 回调函数不返回任何值
     * */
    template<BOOST_ASIO_COMPLETION_TOKEN_FOR(void()) CompletionCallback>
    void async_exec(qqmusic::Task<void> task, CompletionCallback&& callback) {
        boost::asio::co_spawn(ioc,
                              std::move(task),
                              [callback = std::forward<CompletionCallback>(callback)](
                                  std::exception_ptr e) {
                                  if (!e) {
                                      callback();
                                  } else {
                                      try {
                                          std::rethrow_exception(e);
                                      } catch (const std::exception& ex) {
                                          /* TODO: get a logger*/
                                          std::cout << std::format("Exception in async task: {}",
                                                                   ex.what())
                                                    << std::endl;
                                      }
                                  }
                              });
    }

    /**
     * @brief 异步执行任务函数, 带有超时限制, 并在任务结束后触发回调
     *
     * @param task 异步任务
     * @param callback 回调函数, 会将`task`的执行结果传递给回调函数
     *                 回调函数不返回任何值
     * @param timeout 超时时间限制
     * @param timeout_callback 超时之后需要执行的回调函数
     * */
    template<typename T,
             BOOST_ASIO_COMPLETION_TOKEN_FOR(void(T&&)) CompletionCallback,
             BOOST_ASIO_COMPLETION_TOKEN_FOR(void()) TimeoutCallback>
    void async_exec(qqmusic::Task<T> task,
                    CompletionCallback&& callback,
                    std::chrono::steady_clock::duration timeout,
                    TimeoutCallback&& timeout_callback) {
        auto timer = std::make_shared<boost::asio::steady_timer>(ioc, timeout);
        auto task_ptr = std::make_shared<boost::asio::awaitable<void>>(std::move(task));

        boost::asio::co_spawn(ioc,
                              std::move(*task_ptr),
                              [callback = std::forward<CompletionCallback>(callback),
                               timer](std::exception_ptr e, T value) {
                                  timer->cancel();
                                  if (!e) {
                                      callback(std::move(value));
                                  } else {
                                      try {
                                          std::rethrow_exception(e);
                                      } catch (const std::exception& ex) {
                                          std::cout << std::format("Exception in async task: {}",
                                                                   ex.what())
                                                    << std::endl;
                                      }
                                  }
                              });

        /*if timeout*/
        timer->async_wait([timeout_callback = std::forward<TimeoutCallback>(timeout_callback),
                           task_ptr](boost::system::error_code& ec) {
            if (!ec) {
                timeout_callback();
            } else {
                std::cout << std::format("Exception in async task: {}", ec.what()) << std::endl;
            }
        });
    }

    /**
     * @brief 异步执行任务函数void特化, 带有超时限制. 并在任务结束后触发回调
     *
     * @param task 异步任务
     * @param callback 回调函数, 会将`task`的执行结果传递给回调函数
     *                 回调函数不返回任何值
     * @param timeout 超时时间限制
     * @param timeout_callback 超时之后需要执行的回调函数
     * */
    template<BOOST_ASIO_COMPLETION_TOKEN_FOR(void()) CompletionCallback,
             BOOST_ASIO_COMPLETION_TOKEN_FOR(void()) TimeoutCallback>
    void async_exec(qqmusic::Task<void> task,
                    CompletionCallback&& callback,
                    std::chrono::steady_clock::duration timeout,
                    TimeoutCallback&& timeout_callback) {
        auto timer = std::make_shared<boost::asio::steady_timer>(ioc, timeout);
        auto task_ptr = std::make_shared<boost::asio::awaitable<void>>(std::move(task));

        boost::asio::co_spawn(ioc,
                              std::move(*task_ptr),
                              [callback = std::forward<CompletionCallback>(callback),
                               timer](std::exception_ptr e) {
                                  timer->cancel();
                                  if (!e) {
                                      callback();
                                  } else {
                                      try {
                                          std::rethrow_exception(e);
                                      } catch (const std::exception& ex) {
                                          std::cout << std::format("Exception in async task: {}",
                                                                   ex.what())
                                                    << std::endl;
                                      }
                                  }
                              });

        /*if timeout*/
        timer->async_wait([timeout_callback = std::forward<TimeoutCallback>(timeout_callback),
                           task_ptr](boost::system::error_code& ec) {
            if (!ec) {
                timeout_callback();
            } else {
                std::cout << std::format("Exception in async task: {}", ec.what()) << std::endl;
            }
        });
    }

    /**
     * @brief 等待任务列表中所有的任务执行完毕之后返回结果列表
     *
     * @code{cpp}
     * // 使用示例:
     * std::vector<Task<nlohmann::json>> tasks{}; // 给定任务列表
     * auto& executor = qqmusic::utils::AsyncExecutor::get_instance();
     * auto results = executor.when_all(std::move(tasks));
     * for (auto& result : results) {
     *     // handle the result ...
     * }
     * @endcode
     *
     * @param tasks 异步任务列表
     *
     * @return 包含结果列表的任务`boost::asio::awaitable<std::vector<T>>`
     *
     * @warn 原本任务的顺序和结果顺序不一定一致
     * */
    template<typename T>
    auto when_all(std::vector<boost::asio::awaitable<T>> tasks)
        -> boost::asio::awaitable<std::vector<T>> {
        std::vector<std::shared_ptr<std::promise<T>>> promises;
        std::vector<std::future<T>> futures;

        promises.reserve(tasks.size());
        futures.reserve(tasks.size());

        for (auto& task : tasks) {
            auto promise = std::make_shared<std::promise<T>>();
            futures.push_back(promise->get_future());

            boost::asio::co_spawn(
                ioc,
                [task = std::move(task), promise]() mutable -> boost::asio::awaitable<void> {
                    try {
                        T result = co_await std::move(task);
                        promise->set_value(std::move(result));
                    } catch (...) {
                        promise->set_exception(std::current_exception());
                    }
                },
                boost::asio::detached);

            promises.push_back(std::move(promise));
        }

        std::vector<T> results;
        results.reserve(tasks.size());
        for (auto& fut : futures) {
            results.push_back(std::move(fut.get()));
        }

        co_return results;
    }

    void shutdown();

private:
    AsyncExecutor();
    ~AsyncExecutor();
    boost::asio::io_context ioc;
    std::thread ioc_thread;
};

} // namespace qqmusic::utils

#endif // !QQMUSIC_UTILS_ASYNC_EXECUTOR_H
