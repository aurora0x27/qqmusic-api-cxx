#include <cstdlib>
#include <filesystem>
#include <qqmusic/details/cache.h>

qqmusic::details::CacheManager& qqmusic::details::CacheManager::get_instance() {
    static qqmusic::details::CacheManager cm;
    return cm;
}

std::filesystem::path qqmusic::details::CacheManager::get_log_path() const {
    return log_path;
}

std::filesystem::path qqmusic::details::CacheManager::get_cache_path() const {
    return cache_path;
}

std::filesystem::path qqmusic::details::CacheManager::get_download_path() const {
    return download_path;
}

void qqmusic::details::CacheManager::set_log_path(std::string_view path) {
    log_path = fs::path(path);
}

void qqmusic::details::CacheManager::set_cache_path(std::string_view path) {
    cache_path = fs::path(path);
}

void qqmusic::details::CacheManager::set_download_path(std::string_view path) {
    download_path = fs::path(path);
}

void qqmusic::details::CacheManager::set_log_path(const std::filesystem::path& path) {
    log_path = path;
}

void qqmusic::details::CacheManager::set_cache_path(const std::filesystem::path& path) {
    cache_path = path;
}

void qqmusic::details::CacheManager::set_download_path(const std::filesystem::path& path) {
    download_path = path;
}

qqmusic::details::CacheManager::CacheManager() {
#ifdef PLATFORM_WINDOWS
    /*Windows Related Code*/
#error "Platform not supported yet"
#elif defined(PLATFORM_LINUX)
    auto xdg_cache_home = getenv("XDG_CACHE_HOME");
    if (xdg_cache_home == nullptr) {
        auto home = getenv("HOME");
        if (home == nullptr) {
            cache_path = fs::path(fs::current_path()) / fs::path("cache");
            log_path = fs::path(fs::current_path()) / fs::path("log");
            download_path = fs::path(fs::current_path()) / fs::path("download");
        } else {
            cache_path = fs::path(home) / fs::path(".cache/qqmusic-api-cxx/cache");
            log_path = fs::path(home) / fs::path(".cache/qqmusic-api-cxx/log");
            download_path = fs::path(home) / fs::path("Download/qqmusic-api-cxx");
        }
    } else {
        cache_path = fs::path(xdg_cache_home) / fs::path("qqmusic-api-cxx/cache");
        log_path = fs::path(xdg_cache_home) / fs::path("qqmusic-api-cxx/log");
        auto home = getenv("HOME");
        if (home == nullptr) {
            download_path = fs::path(fs::current_path()) / fs::path("download");
        } else {
            download_path = fs::path(home) / fs::path("Download/qqmusic-api-cxx");
        }
    }
#elif defined(PLATFORM_APPLE)
    /*MacOS Related Code*/
#error "Platform not supported yet"
#else
    /*Warining platform not supported*/
#error "Platform not supported"
#endif
}
