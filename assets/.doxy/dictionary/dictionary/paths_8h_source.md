

# File paths.h

[**File List**](files.md) **>** [**include**](dir_d44c64559bbebec7f509842c48db8b23.md) **>** [**qqmusic**](dir_d63c0418b33b823a308efea67b8f3df2.md) **>** [**utils**](dir_478616d8952f43e793f28d8ded6e3463.md) **>** [**paths.h**](paths_8h.md)

[Go to the documentation of this file](paths_8h.md)


```C++

#ifndef QQMUSIC_UTILS_CACHE_H
#define QQMUSIC_UTILS_CACHE_H
#include <filesystem>

namespace qqmusic::utils {

namespace fs = std::filesystem;

class PathManager {
public:
    static PathManager& get_instance();

    void set_download_path(std::string_view path);
    void set_download_path(const fs::path& path);

    void set_cache_path(std::string_view path);
    void set_cache_path(const fs::path& path);


    void set_log_path(std::string_view path);
    void set_log_path(const fs::path& path);

    [[nodiscard]] fs::path get_cache_path() const;

    [[nodiscard]] fs::path get_download_path() const;

    [[nodiscard]] fs::path get_log_path() const;

private:
    PathManager();
    fs::path cache_path;
    fs::path log_path;
    fs::path download_path;
};

} // namespace qqmusic::utils

#endif // !QQMUSIC_UTILS_CACHE_H
```


