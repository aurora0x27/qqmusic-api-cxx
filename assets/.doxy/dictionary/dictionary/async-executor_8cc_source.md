

# File async-executor.cc

[**File List**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**utils**](dir_313caf1132e152dd9b58bea13a4052ca.md) **>** [**async-executor.cc**](async-executor_8cc.md)

[Go to the documentation of this file](async-executor_8cc.md)


```C++
#include <boost/asio/signal_set.hpp>
#include <qqmusic/utils/async-executor.h>

namespace qqmusic::utils {

AsyncExecutor::AsyncExecutor() {
    ioc_thread = std::thread([this]() {
        boost::asio::signal_set sigset{ioc, SIGINT, SIGTERM};
        /*set handler for SIGINT and SIGTERM*/
        sigset.async_wait([&](auto, auto) { ioc.stop(); });
        /*keep ioc alive when no task left*/
        boost::asio::executor_work_guard<decltype(ioc.get_executor())> work{ioc.get_executor()};
        /*run the ioc event loop*/
        ioc.run();
    });
}

AsyncExecutor::~AsyncExecutor() {
    shutdown();
}

void AsyncExecutor::shutdown() {
    ioc.stop();
    if (ioc_thread.joinable()) {
        ioc_thread.join();
    }
}

AsyncExecutor& AsyncExecutor::get_instance() {
    static AsyncExecutor ae;
    return ae;
}

} // namespace qqmusic::utils
```


