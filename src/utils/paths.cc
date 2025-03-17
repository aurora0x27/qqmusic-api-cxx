#include <cstdlib>
#include <filesystem>
#include <qqmusic/utils/paths.h>

namespace qqmusic::utils {

PathManager& PathManager::get_instance() {
    static PathManager cm;
    return cm;
}

std::filesystem::path PathManager::get_log_path() const {
    return log_path;
}

std::filesystem::path PathManager::get_cache_path() const {
    return cache_path;
}

std::filesystem::path PathManager::get_download_path() const {
    return download_path;
}

void PathManager::set_log_path(std::string_view path) {
    log_path = fs::path(path);
}

void PathManager::set_cache_path(std::string_view path) {
    cache_path = fs::path(path);
}

void PathManager::set_download_path(std::string_view path) {
    download_path = fs::path(path);
}

void PathManager::set_log_path(const std::filesystem::path& path) {
    log_path = path;
}

void PathManager::set_cache_path(const std::filesystem::path& path) {
    cache_path = path;
}

void PathManager::set_download_path(const std::filesystem::path& path) {
    download_path = path;
}

PathManager::PathManager() {
#ifdef PLATFORM_WINDOWS
    /*Windows Related Code*/
    auto local_app_data = getenv("%LOCALAPPDATA%");
    if (local_app_data == nullptr) {
        auto home = getenv("%USERPROFILE%");
        if (home == nullptr) {
            cache_path = fs::path(fs::current_path()) / fs::path("cache");
            log_path = fs::path(fs::current_path()) / fs::path("log");
            download_path = fs::path(fs::current_path()) / fs::path("download");
        } else {
            cache_path = fs::path(home) / fs::path(".cache/qqmusic-api-cxx/cache");
            log_path = fs::path(home) / fs::path(".cache/qqmusic-api-cxx/log");
            download_path = fs::path(home) / fs::path("Downloads/qqmusic-api-cxx");
        }
    } else {
        cache_path = fs::path(local_app_data) / fs::path("qqmusic-api-cxx/cache");
        log_path = fs::path(local_app_data) / fs::path("qqmusic-api-cxx/log");
        auto home = getenv("%USERPROFILE%");
        if (home == nullptr) {
            download_path = fs::path(fs::current_path()) / fs::path("download");
        } else {
            download_path = fs::path(home) / fs::path("Downloads/qqmusic-api-cxx");
        }
    }

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
            download_path = fs::path(home) / fs::path("Downloads/qqmusic-api-cxx");
        }
    } else {
        cache_path = fs::path(xdg_cache_home) / fs::path("qqmusic-api-cxx/cache");
        log_path = fs::path(xdg_cache_home) / fs::path("qqmusic-api-cxx/log");
        auto home = getenv("HOME");
        if (home == nullptr) {
            download_path = fs::path(fs::current_path()) / fs::path("download");
        } else {
            download_path = fs::path(home) / fs::path("Downloads/qqmusic-api-cxx");
        }
    }

#elif defined(PLATFORM_APPLE)
    /*MacOS Related Code*/
    auto home = getenv("HOME");
    if (home == nullptr) {
        cache_path = fs::path(fs::current_path()) / fs::path("cache");
        log_path = fs::path(fs::current_path()) / fs::path("log");
        download_path = fs::path(fs::current_path()) / fs::path("download");
    } else {
        cache_path = fs::path(home) / fs::path("Library/Caches/qqmusic-api-cxx/cache");
        log_path = fs::path(home) / fs::path("Library/Caches/qqmusic-api-cxx/log");
        download_path = fs::path(home) / fs::path("Downloads/qqmusic-api-cxx");
    }
#else
    /*Warining platform not supported*/
#error "Platform not supported"
#endif
    if (!fs::exists(cache_path)) {
        fs::create_directories(cache_path);
    }

    if (!fs::exists(download_path)) {
        fs::create_directories(download_path);
    }

    if (!fs::exists(log_path)) {
        fs::create_directories(log_path);
    }
}

} // namespace qqmusic::utils
