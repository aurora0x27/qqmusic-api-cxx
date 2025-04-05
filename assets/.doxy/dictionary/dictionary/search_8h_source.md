

# File search.h

[**File List**](files.md) **>** [**include**](dir_d44c64559bbebec7f509842c48db8b23.md) **>** [**qqmusic**](dir_d63c0418b33b823a308efea67b8f3df2.md) **>** [**search.h**](search_8h.md)

[Go to the documentation of this file](search_8h.md)


```C++

#ifndef QQMUSIC_SEARCH_H
#define QQMUSIC_SEARCH_H

#include <qqmusic/result.h>
#include <string_view>

namespace qqmusic {

enum class SearchType : unsigned {
    SONG = 0,         
    SINGER = 1,       
    ALBUM = 2,        
    SONG_LIST = 3,    
    MV = 4,           
    LYRIC = 7,        
    USER = 8,         
    AUDIO_ALBUM = 15, 
    AUDIO = 18,       
};

Task<Result<nlohmann::json>> hotkey();

Task<Result<nlohmann::json>> keyword_complete(std::string_view keyword);

Task<Result<nlohmann::json>> quick_search(std::string_view keyword);

Task<Result<nlohmann::json>> general_search(std::string_view keyword,
                                            unsigned num = 15,
                                            unsigned page = 1,
                                            bool highlight = true);

Task<Result<nlohmann::json>> search_by_type(std::string_view keyword,
                                            SearchType type = SearchType::SONG,
                                            unsigned num_per_page = 10,
                                            unsigned page = 1,
                                            bool highlight = true);

} // namespace qqmusic

#endif // !QQMUSIC_SEARCH_H
```


