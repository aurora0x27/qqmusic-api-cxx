#include <memory>
#include <mutex>
#include <qqmusic/result.h>
#include <qqmusic/utils/session.h>
#include <utility>

namespace qqmusic::utils {

SessionManager::SessionManager()
    : ioc(std::make_shared<net::io_context>())
    , ctx(qqmusic::details::NetworkContext()) {}

SessionManager& SessionManager::get_instance() {
    static std::unique_ptr<SessionManager> instance;
    static std::once_flag flag;
    std::call_once(flag, []() { instance.reset(new SessionManager()); });
    return *instance;
}

Session SessionManager::get_session() {
    return std::move(Session(ctx, ioc, lock));
}

void SessionManager::set_context(const qqmusic::details::NetworkContext& context) {
    std::lock_guard<std::mutex> lg(lock);
    ctx = context;
}

void SessionManager::push_context(qqmusic::details::NetworkContext&& context) {
    std::lock_guard<std::mutex> lg(lock);
    context_stack.push(std::move(ctx));
    ctx = std::move(context);
}

void SessionManager::pop_context() {
    std::lock_guard<std::mutex> lg(lock);
    ctx = std::move(context_stack.top());
    context_stack.pop();
}

qqmusic::details::NetworkContext& Session::get_context_ref() {
    return local_ctx;
}

void Session::sync_global() {
    std::lock_guard<std::mutex> lg(lock);
    global_ctx = local_ctx;
}

void Session::update_local() {
    std::lock_guard<std::mutex> lg(lock);
    local_ctx = global_ctx;
}

qqmusic::Task<qqmusic::Result<HttpResponse>> Session::perform_request(
    http::request<http::string_body>& req) {
    co_return Err(Exception(Exception::UnknownError, ""));
}

} // namespace qqmusic::utils
