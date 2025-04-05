

# Class qqmusic::utils::PathManager



[**ClassList**](annotated.md) **>** [**qqmusic**](namespaceqqmusic.md) **>** [**utils**](namespaceqqmusic_1_1utils.md) **>** [**PathManager**](classqqmusic_1_1utils_1_1PathManager.md)



_返回默认路径_ 

* `#include <paths.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|  fs::path | [**get\_cache\_path**](#function-get_cache_path) () const<br>_获取缓存路径_  |
|  fs::path | [**get\_download\_path**](#function-get_download_path) () const<br>_获取下载路径_  |
|  fs::path | [**get\_log\_path**](#function-get_log_path) () const<br>_获取日志路径_  |
|  void | [**set\_cache\_path**](#function-set_cache_path-12) (std::string\_view path) <br>_设置缓存路径_  |
|  void | [**set\_cache\_path**](#function-set_cache_path-22) (const fs::path & path) <br> |
|  void | [**set\_download\_path**](#function-set_download_path-12) (std::string\_view path) <br>_设置下载路径_  |
|  void | [**set\_download\_path**](#function-set_download_path-22) (const fs::path & path) <br> |
|  void | [**set\_log\_path**](#function-set_log_path-12) (std::string\_view path) <br>_设置日志路径_  |
|  void | [**set\_log\_path**](#function-set_log_path-22) (const fs::path & path) <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**PathManager**](classqqmusic_1_1utils_1_1PathManager.md) & | [**get\_instance**](#function-get_instance) () <br>_获取实例引用_  |


























## Public Functions Documentation




### function get\_cache\_path 

_获取缓存路径_ 
```C++
fs::path qqmusic::utils::PathManager::get_cache_path () const
```




<hr>



### function get\_download\_path 

_获取下载路径_ 
```C++
fs::path qqmusic::utils::PathManager::get_download_path () const
```




<hr>



### function get\_log\_path 

_获取日志路径_ 
```C++
fs::path qqmusic::utils::PathManager::get_log_path () const
```




<hr>



### function set\_cache\_path [1/2]

_设置缓存路径_ 
```C++
void qqmusic::utils::PathManager::set_cache_path (
    std::string_view path
) 
```




<hr>



### function set\_cache\_path [2/2]

```C++
void qqmusic::utils::PathManager::set_cache_path (
    const fs::path & path
) 
```




<hr>



### function set\_download\_path [1/2]

_设置下载路径_ 
```C++
void qqmusic::utils::PathManager::set_download_path (
    std::string_view path
) 
```




<hr>



### function set\_download\_path [2/2]

```C++
void qqmusic::utils::PathManager::set_download_path (
    const fs::path & path
) 
```




<hr>



### function set\_log\_path [1/2]

_设置日志路径_ 
```C++
void qqmusic::utils::PathManager::set_log_path (
    std::string_view path
) 
```




<hr>



### function set\_log\_path [2/2]

```C++
void qqmusic::utils::PathManager::set_log_path (
    const fs::path & path
) 
```




<hr>
## Public Static Functions Documentation




### function get\_instance 

_获取实例引用_ 
```C++
static PathManager & qqmusic::utils::PathManager::get_instance () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/qqmusic/utils/paths.h`

