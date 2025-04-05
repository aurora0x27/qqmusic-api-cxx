

# File mv.h

[**File List**](files.md) **>** [**include**](dir_d44c64559bbebec7f509842c48db8b23.md) **>** [**qqmusic**](dir_d63c0418b33b823a308efea67b8f3df2.md) **>** [**mv.h**](mv_8h.md)

[Go to the documentation of this file](mv_8h.md)


```C++

#ifndef QQMUSIC_MV_H
#define QQMUSIC_MV_H

#include <qqmusic/result.h>
#include <qqmusic/utils/async-executor.h>

namespace qqmusic {

struct MvUrl {
    std::string vid;                        
    std::map<std::string, std::string> mp4; 
    std::map<std::string, std::string> hls; 
};

Task<Result<nlohmann::json>> get_mv_detail(std::span<std::string> vids);

Task<Result<std::vector<MvUrl>>> get_mv_urls(std::span<std::string> vids);

} // namespace qqmusic

#endif // !QQMUSIC_MV_H
```


