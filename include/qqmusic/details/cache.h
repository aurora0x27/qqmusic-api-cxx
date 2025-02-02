/*------------------------------details/cache.h---------------------------------
 * CacheManager provide common interface to file system.
 *
 * CacheManager provide 3 types of files for io : log, cache, download
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
 *         - download $HOME/Download/qqmusic-api-cxx
 *
 *
 *    - Windows:
 *         - cache:
 *
 *         - log:
 *
 *         - download:
 *
 *
 *    - MacOs:
 *         - cache:
 *
 *         - log:
 *
 *         - download:
 * 
 * Applications can set the path by themselves
 * Usage:
 * ```
 * "get instance first"
 * auto& cm = CacheManager::get_instance();
 * auto cache_path = cm.get_cache_path();
 * ```
*-----------------------------------------------------------------------------*/
#ifndef QQMUSIC_DETAILS_CACHE_H
#define QQMUSIC_DETAILS_CACHE_H
#include <filesystem>

namespace qqmusic::details {

namespace fs = std::filesystem;

class CacheManager {
public:
    static CacheManager& get_instance();
    void set_download_path(std::string_view path);
    void set_cache_path(std::string_view path);
    void set_log_path(std::string_view path);
    void set_download_path(const fs::path& path);
    void set_cache_path(const fs::path& path);
    void set_log_path(const fs::path& path);
    [[nodiscard]] fs::path get_cache_path() const;
    [[nodiscard]] fs::path get_download_path() const;
    [[nodiscard]] fs::path get_log_path() const;
private:
    CacheManager();
    fs::path cache_path;
    fs::path log_path;
    fs::path download_path;
};

} // namespace qqmusic::details

#endif // !QQMUSIC_DETAILS_CACHE_H
