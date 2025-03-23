/**------------------------------------------utils/paths.h------------------------------------------
 * PathManager provide common interface to file system.
 *
 * PathManager provide 3 types of files for io : log, cache, download
 *    - *log*        for debug log info
 *    - *cache*      for cached credential, identification, recent cached songs
 *    - *download*   for downloaded media and lyrics
 *
 * Default paths are:
 *    - Linux:
 *         - cache: $XDG_CACHE_HOME/qqmusic-api-cxx/cache
 *                  OR $HOME/.cache/qqmusic-api-cxx/cache
 *
 *         - log: $XDG_CACHE_HOME/qqmusic-api-cxx/log
 *                OR $HOME/.cache/qqmusic-api-cxx/log
 *
 *         - download: $HOME/Download/qqmusic-api-cxx
 *
 *
 *    - Windows:
 *         - cache: %LOCALAPPDATA%\cache
 *                  OR %USERPROFILE%\cahce
 *
 *         - log: %LOCALAPPDATA%\log  
 *                OR %USERPROFILE%\log
 *
 *         - download:%USERPROFILE%\Download\qqmusic-api-cxx
 *
 *    - MacOS:
 *         - cache: $HOME/Library/Caches/qqmusic-api-cxx/cache
 *
 *         - log: $HOME/Library/Caches/qqmusic-api-cxx/log
 *
 *         - download: $HOME/Downloads/qqmusic-api-cxx
 *
 * Applications can set the path by themselves
 * Usage:
 * ```
 * "get instance first"
 * auto& pm = PathManager::get_instance();
 * auto cache_path = cm.get_cache_path();
 * ```
 *------------------------------------------------------------------------------------------------*/
#ifndef QQMUSIC_UTILS_CACHE_H
#define QQMUSIC_UTILS_CACHE_H
#include <filesystem>

namespace qqmusic::utils {

namespace fs = std::filesystem;

/**
 * @brief 返回默认路径
 * */
class PathManager {
public:
    /**
     * @brief 获取实例引用
     * */
    static PathManager& get_instance();

    /**
     * @brief 设置下载路径
     * */
    void set_download_path(std::string_view path);
    void set_download_path(const fs::path& path);

    /**
     * @brief 设置缓存路径
     * */
    void set_cache_path(std::string_view path);
    void set_cache_path(const fs::path& path);


    /**
     * @brief 设置日志路径
     * */
    void set_log_path(std::string_view path);
    void set_log_path(const fs::path& path);

    /**
     * @brief 获取缓存路径
     * */
    [[nodiscard]] fs::path get_cache_path() const;

    /**
     * @brief 获取下载路径
     * */
    [[nodiscard]] fs::path get_download_path() const;

    /**
     * @brief 获取日志路径
     * */
    [[nodiscard]] fs::path get_log_path() const;

private:
    PathManager();
    fs::path cache_path;
    fs::path log_path;
    fs::path download_path;
};

} // namespace qqmusic::utils

#endif // !QQMUSIC_UTILS_CACHE_H
