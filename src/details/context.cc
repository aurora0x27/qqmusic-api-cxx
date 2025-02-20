#include <mutex>
#include <qqmusic/details/context.h>

qqmusic::details::NetworkContextManager::NetworkContextManager() {
    inner = NetworkContext();
}

qqmusic::details::NetworkContextManager& qqmusic::details::NetworkContextManager::get_instance() {
    static NetworkContextManager ncm;
    return ncm;
}

qqmusic::details::NetworkContext qqmusic::details::NetworkContextManager::get() {
    std::lock_guard<std::mutex> lg(lock);
    return inner;
}

void qqmusic::details::NetworkContextManager::set(const qqmusic::details::NetworkContext& ctx) {
    std::lock_guard<std::mutex> lg(lock);
    inner = ctx;
}
