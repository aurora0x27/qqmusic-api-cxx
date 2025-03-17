---
title: Paths
---

全局单例, 管理Api库的下载, 日志和缓存库

## ***class*** PathManager

各个平台下的默认路径:

- Linux:

     - cache: `$XDG_CACHE_HOME/qqmusic-api-cxx/cache`
              OR `$HOME/.cache/qqmusic-api-cxx/cache`

     - log: `$XDG_CACHE_HOME/qqmusic-api-cxx/log`
            OR `$HOME/.cache/qqmusic-api-cxx/log`

     - download: `$HOME/Download/qqmusic-api-cxx`


- Windows:

     - cache: `%LOCALAPPDATA%\cache`
              OR `%USERPROFILE%\cahce`

     - log: `%LOCALAPPDATA%\log`
            OR `%USERPROFILE%\log`

     - download: `%USERPROFILE%\Download\qqmusic-api-cxx`

- MacOS:

     - cache: `$HOME/Library/Caches/qqmusic-api-cxx/cache`

     - log: `$HOME/Library/Caches/qqmusic-api-cxx/log`

     - download: `$HOME/Downloads/qqmusic-api-cxx`

### ***fn*** get_instance

获得PathManager全局单例的引用

```cpp
static PathManager& get_instance();
```

### ***fn*** set_download_path

修改默认下载路径

```cpp
void set_download_path(std::string_view path);
```

### ***fn*** set_cache_path

修改默认缓存路径

```cpp
void set_cache_path(std::string_view path);
```

### ***fn*** set_log_path

修改默认日志路径

```cpp
void set_log_path(std::string_view path);
```

### ***fn***  get_cache_path

获取默认缓存路径

```cpp
[[nodiscard]] fs::path get_cache_path() const;
```

### ***fn*** get_download_path

获取默认下载路径

```cpp
[[nodiscard]] fs::path get_download_path() const;
```

### ***fn*** get_log_path

获取默认日志路径

```cpp
[[nodiscard]] fs::path get_log_path() const;
```
