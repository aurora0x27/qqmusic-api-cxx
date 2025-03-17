---
title: Async Executor
---

默认的异步调度器, 也提供了将异步任务转换成同步任务执行的函数

!!! warn "Warning"

    AsyncExecutor类没有禁用异常

## ***fn*** sync_exec

同步执行异步函数

```cpp
template<typename T>
T sync_exec(qqmusic::Task<T> task);
```

## ***class*** AsyncExecutor

全局单例的异步调度器

### ***fn*** get_instance

获取全局AsyncExecutor实例的引用

```cpp
static AsyncExecutor& get_instance();
```

### ***fn*** async_exec

- 一般异步执行函数

```cpp
template<typename T, BOOST_ASIO_COMPLETION_TOKEN_FOR(void(T&)) CompletionCallback>
void async_exec(qqmusic::Task<T> task, CompletionCallback&& callback);
```

- 一般异步执行函数(返回void类型特化)

```cpp
template<BOOST_ASIO_COMPLETION_TOKEN_FOR(void()) CompletionCallback>
void async_exec(qqmusic::Task<void> task, CompletionCallback&& callback);
```

- 带有超时限制的异步执行

```cpp
template<typename T,
            BOOST_ASIO_COMPLETION_TOKEN_FOR(void(T&)) CompletionCallback,
            BOOST_ASIO_COMPLETION_TOKEN_FOR(void()) TimeoutCallback>
void async_exec(qqmusic::Task<T> task,
                CompletionCallback&& callback,
                std::chrono::steady_clock::duration timeout,
                TimeoutCallback&& timeout_callback);
```

- 带有超时限制的异步执行(返回void类型的特化)

```cpp
/*void specialization*/
template<BOOST_ASIO_COMPLETION_TOKEN_FOR(void()) CompletionCallback,
            BOOST_ASIO_COMPLETION_TOKEN_FOR(void()) TimeoutCallback>
void async_exec(qqmusic::Task<void> task,
                CompletionCallback&& callback,
                std::chrono::steady_clock::duration timeout,
                TimeoutCallback&& timeout_callback);
```

### ***fn*** shutdown

停止AsyncExecutor内部线程的执行, 终止所有异步任务
