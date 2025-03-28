

# Class qqmusic::utils::AsyncExecutor



[**ClassList**](annotated.md) **>** [**qqmusic**](namespaceqqmusic.md) **>** [**utils**](namespaceqqmusic_1_1utils.md) **>** [**AsyncExecutor**](classqqmusic_1_1utils_1_1AsyncExecutor.md)



_全局调度器类_ 

* `#include <async-executor.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**AsyncExecutor**](#function-asyncexecutor-12) (const [**AsyncExecutor**](classqqmusic_1_1utils_1_1AsyncExecutor.md) &) = delete<br> |
|  void | [**async\_exec**](#function-async_exec-14) (qqmusic::Task&lt; T &gt; task, CompletionCallback && callback) <br>_异步执行任务, 并在任务结束后触发回调_  |
|  void | [**async\_exec**](#function-async_exec-24) (qqmusic::Task&lt; void &gt; task, CompletionCallback && callback) <br>_异步执行任务函数的void特化, 并在任务结束后触发回调_  |
|  void | [**async\_exec**](#function-async_exec-34) (qqmusic::Task&lt; T &gt; task, CompletionCallback && callback, std::chrono::steady\_clock::duration timeout, TimeoutCallback && timeout\_callback) <br>_异步执行任务函数, 带有超时限制, 并在任务结束后触发回调_  |
|  void | [**async\_exec**](#function-async_exec-44) (qqmusic::Task&lt; void &gt; task, CompletionCallback && callback, std::chrono::steady\_clock::duration timeout, TimeoutCallback && timeout\_callback) <br>_异步执行任务函数void特化, 带有超时限制. 并在任务结束后触发回调_  |
|  [**AsyncExecutor**](classqqmusic_1_1utils_1_1AsyncExecutor.md) & | [**operator=**](#function-operator) (const [**AsyncExecutor**](classqqmusic_1_1utils_1_1AsyncExecutor.md) &) = delete<br> |
|  void | [**shutdown**](#function-shutdown) () <br> |
|  auto | [**when\_all**](#function-when_all) (std::vector&lt; boost::asio::awaitable&lt; T &gt; &gt; tasks) <br>_等待任务列表中所有的任务执行完毕之后返回结果列表_  |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**AsyncExecutor**](classqqmusic_1_1utils_1_1AsyncExecutor.md) & | [**get\_instance**](#function-get_instance) () <br>_获取全局单例引用_  |


























## Public Functions Documentation




### function AsyncExecutor [1/2]

```C++
qqmusic::utils::AsyncExecutor::AsyncExecutor (
    const AsyncExecutor &
) = delete
```




<hr>



### function async\_exec [1/4]

_异步执行任务, 并在任务结束后触发回调_ 
```C++
template<typename T, BOOST_ASIO_COMPLETION_TOKEN_FOR(void(T &&)) CompletionCallback>
inline void qqmusic::utils::AsyncExecutor::async_exec (
    qqmusic::Task< T > task,
    CompletionCallback && callback
) 
```





**Parameters:**


* `task` 异步任务 
* `callback` 回调函数, 会将`task`的执行结果传递给回调函数 回调函数不返回任何值 




        

<hr>



### function async\_exec [2/4]

_异步执行任务函数的void特化, 并在任务结束后触发回调_ 
```C++
template<BOOST_ASIO_COMPLETION_TOKEN_FOR(void()) CompletionCallback>
inline void qqmusic::utils::AsyncExecutor::async_exec (
    qqmusic::Task< void > task,
    CompletionCallback && callback
) 
```





**Parameters:**


* `task` 异步任务 
* `callback` 回调函数, 会将`task`的执行结果传递给回调函数 回调函数不返回任何值 




        

<hr>



### function async\_exec [3/4]

_异步执行任务函数, 带有超时限制, 并在任务结束后触发回调_ 
```C++
template<typename T, BOOST_ASIO_COMPLETION_TOKEN_FOR(void(T &&)) CompletionCallback, BOOST_ASIO_COMPLETION_TOKEN_FOR(void()) TimeoutCallback>
inline void qqmusic::utils::AsyncExecutor::async_exec (
    qqmusic::Task< T > task,
    CompletionCallback && callback,
    std::chrono::steady_clock::duration timeout,
    TimeoutCallback && timeout_callback
) 
```





**Parameters:**


* `task` 异步任务 
* `callback` 回调函数, 会将`task`的执行结果传递给回调函数 回调函数不返回任何值 
* `timeout` 超时时间限制 
* `timeout_callback` 超时之后需要执行的回调函数 




        

<hr>



### function async\_exec [4/4]

_异步执行任务函数void特化, 带有超时限制. 并在任务结束后触发回调_ 
```C++
template<BOOST_ASIO_COMPLETION_TOKEN_FOR(void()) CompletionCallback, BOOST_ASIO_COMPLETION_TOKEN_FOR(void()) TimeoutCallback>
inline void qqmusic::utils::AsyncExecutor::async_exec (
    qqmusic::Task< void > task,
    CompletionCallback && callback,
    std::chrono::steady_clock::duration timeout,
    TimeoutCallback && timeout_callback
) 
```





**Parameters:**


* `task` 异步任务 
* `callback` 回调函数, 会将`task`的执行结果传递给回调函数 回调函数不返回任何值 
* `timeout` 超时时间限制 
* `timeout_callback` 超时之后需要执行的回调函数 




        

<hr>



### function operator= 

```C++
AsyncExecutor & qqmusic::utils::AsyncExecutor::operator= (
    const AsyncExecutor &
) = delete
```




<hr>



### function shutdown 

```C++
void qqmusic::utils::AsyncExecutor::shutdown () 
```




<hr>



### function when\_all 

_等待任务列表中所有的任务执行完毕之后返回结果列表_ 
```C++
template<typename T>
inline auto qqmusic::utils::AsyncExecutor::when_all (
    std::vector< boost::asio::awaitable< T > > tasks
) 
```




```C++
// 使用示例:
std::vector<Task<nlohmann::json>> tasks{}; // 给定任务列表
auto& executor = qqmusic::utils::AsyncExecutor::get_instance();
auto results = executor.when_all(std::move(tasks));
for (auto& result : results) {
    // handle the result ...
}
```





**Parameters:**


* `tasks` 异步任务列表



**Returns:**

包含结果列表的任务`boost::asio::awaitable<std::vector<T>>`


@warn 原本任务的顺序和结果顺序不一定一致 


        

<hr>
## Public Static Functions Documentation




### function get\_instance 

_获取全局单例引用_ 
```C++
static AsyncExecutor & qqmusic::utils::AsyncExecutor::get_instance () 
```





**Returns:**

全局调度器实例引用 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/qqmusic/utils/async-executor.h`

