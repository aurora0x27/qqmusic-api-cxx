

# File async-executor.h

[**File List**](files.md) **>** [**include**](dir_d44c64559bbebec7f509842c48db8b23.md) **>** [**qqmusic**](dir_d63c0418b33b823a308efea67b8f3df2.md) **>** [**utils**](dir_478616d8952f43e793f28d8ded6e3463.md) **>** [**async-executor.h**](async-executor_8h.md)

[Go to the documentation of this file](async-executor_8h.md)


```C++

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

template<typename T>
T sync_exec(qqmusic::Task<T> task) {
    /*sync get result*/
    boost::asio::io_context ioc;
    return sync_exec(ioc, std::move(task));
}

class AsyncExecutor {
    /*Global AsyncExecutor class*/
public:
    AsyncExecutor(const AsyncExecutor&) = delete;
    AsyncExecutor& operator=(const AsyncExecutor&) = delete;

    static AsyncExecutor& get_instance();

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
```


