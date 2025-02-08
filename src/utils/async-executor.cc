#include <boost/asio/signal_set.hpp>
#include <qqmusic/utils/async-executor.h>

qqmusic::utils::AsyncExecutor::AsyncExecutor() {
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

qqmusic::utils::AsyncExecutor::~AsyncExecutor() {
    shutdown();
}

void qqmusic::utils::AsyncExecutor::shutdown() {
    ioc.stop();
    if (ioc_thread.joinable()) {
        ioc_thread.join();
    }
}

qqmusic::utils::AsyncExecutor& qqmusic::utils::AsyncExecutor::get_instance() {
    static qqmusic::utils::AsyncExecutor ae;
    return ae;
}
