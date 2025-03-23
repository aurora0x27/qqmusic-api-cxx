---
title: AsyncExecutor
---

## AsyncExecutor

默认的异步调度器, 也提供了将异步任务转换成同步任务执行的函数

!!! warn "Warning"

    AsyncExecutor类没有禁用异常

::: doxy.dictionary.namespace.function
    namespace: qqmusic::utils
    name: T sync_exec(boost::asio::io_context &ioc, qqmusic::Task< T > task)

::: doxy.dictionary.namespace.function
    namespace: qqmusic::utils
    name: T sync_exec(qqmusic::Task< T > task)

::: doxy.dictionary.class
    name: qqmusic::utils::AsyncExecutor
