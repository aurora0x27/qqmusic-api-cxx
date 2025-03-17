# Result

提供统一的返回接口, 便于进行错误处理

相关头文件: `qqmusic/result.h`

## `qqmusic::Task<T>`

通用异步任务, 所有的异步Api都使用`qqmusic::Task<T>`包装返回结果

```cpp
template <typename T>
using qqmusic::Task<T> = boost::asio::awaitble<T>
```

## `qqmusic::Result<T>`

通用的返回结果类型, 正常的返回结果和[错误结果](./utils/exception.md)共用同一条通路返回

```cpp
template<typename T>
using Result = Result<T, qqmusic::utils::Exception>;
```

!!! example "Usage"

    ```cpp
    Result<double> func(double n) {
        try {
            auto res = sqrtl(n);
            // 返回正确结果
            return Ok(res);
        } catch (std::exception& e) {
            // 返回错误结果
            return Err(utils::Exception(utils::Exception::UnknownError, 
                        std::format("Error occurred: {}", e.what())));
        }
    }

    int main() {
        double n;
        std::cin >> n;
        auto res = func(n);
        if (res.isErr()) {
            // 判断错误结果
            std::cout << res.unwrapErr().what() << std::endl;
            return 1;
        }
        // 解包出正确结果
        double sqrt_n = res.unwrap()
        std::cout << sqrt_n << std::endl;
        return 0;
    }
    ```
