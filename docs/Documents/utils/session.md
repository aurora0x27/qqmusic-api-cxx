---
title: Session
---

网络请求和上下文相关的Api

## ***class*** Session

Session类负责管理一部分上下文, 并负责处理请求

### ***fn*** Session

- nc: 上下文信息

- ioc_ptr: io_context, 用于处理HTTP请求

- ssl_ctx_ptr: SSL上下文, 用于处理HTTPS请求

```cpp
Session(qqmusic::details::NetworkContext& nc,
        std::shared_ptr<asio::io_context> ioc_ptr,
        std::shared_ptr<asio::ssl::context> ssl_ctx_ptr,
        std::mutex& lock)
    : global_ctx(nc)
    , local_ctx(nc)
    , ioc(std::move(ioc_ptr))
    , ssl_ctx(std::move(ssl_ctx_ptr))
    , lock(lock){};
```

### ***fn*** get_context_ref

获取本实例的上下文引用

```cpp
qqmusic::details::NetworkContext& get_context_ref();
```

### ***fn*** sync_global

线程安全的将本地的context更改同步到全局

```cpp
void sync_global();
```

### ***fn*** update_local

线程安全的使用全局上下文更新本地拷贝

```cpp
void update_local();
```

### ***fn*** perform_request

发送请求, 获得响应

- url: 请求的url

- req: 请求类

- auto_redirecting: 是否自动处理重定向(默认为true)

```cpp
qqmusic::Task<qqmusic::Result<HttpResponse>> 
perform_request(boost::url_view url,
                http::request<http::string_body>& req,
                bool auto_redirecting = true);
```

## ***class*** SessionManager

SessionManager是全局单例, 所有的Session应该使用SessionManager生成, 其维护了一个上下文栈, 便于切换上下文.

### ***fn*** get_instance

获取SessionManager全局单例引用

```cpp
static SessionManager& get_instance();
```

### ***fn*** get_session()

获取一个Session实例

```cpp
Session get_session();
```

### ***fn*** set_context

设置上下文栈栈顶的上下文

```cpp
void set_context(const qqmusic::details::NetworkContext& context);
```

### ***fn*** push_context

向上下文栈中压入上下文

```cpp
void push_context(qqmusic::details::NetworkContext&& context);
```

### ***fn*** pop_context

从上下文栈中弹出栈顶的上下文

```cpp
void pop_context();
```

## ***class*** SessionGuard

[RAII](https://en.cppreference.com/w/cpp/language/raii)风格的上下文切换器

### ***fn*** SessionGuard

- 从Session构造

```cpp
SessionGuard(SessionManager& sm, Session s)
    : session_manager(sm) {
    session_manager.push_context(std::move(s.local_ctx));
};
```

- 从NetworkContext构造

```cpp
SessionGuard(SessionManager& sm, details::NetworkContext ctx)
    : session_manager(sm) {
    session_manager.push_context(std::move(ctx));
};
```

!!! example "Usage"

    ```cpp
    int main(int argc, char** argv) {
        auto sm = qqmusic::utils::SessionManager::get_instance().get_session();
        auto session = sm.get_session();
        boost::url url{"https://example.com"};
        auto req = boost::beast::request<boost::beast::string_body>{boost::beast::http::verb,
                                                                    url,
                                                                    11};
        // 准备请求...

        // 这里使用的是默认的上下文(未加密接口)
        auto resp = qqmusic::utils::sync_exec(session.perform_request(url, req));

        {
            NetworkContext ctx{};
            ctx.verify = true;
            // 这里是独立的作用域, 可以使用SessionGuard在当前作用域中使用这个使用加密接口的上下文
            qqmusic::utils::SessionGuard sg(sm, ctx);
            // 这里的请求使用的是新的上下文(加密接口)
            auto resp1 = qqmusic::utils::sync_exec(session.perform_request(url, req));
            //离开作用域, 释放SessionGuard, 恢复原来的上下文
        }

        // 这里使用的还是默认的上下文(未加密接口)
        auto resp2 = qqmusic::utils::sync_exec(session.perform_request(url, req));
        return 0;
    }
    ```
