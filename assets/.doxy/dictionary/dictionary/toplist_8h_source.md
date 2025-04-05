

# File toplist.h

[**File List**](files.md) **>** [**include**](dir_d44c64559bbebec7f509842c48db8b23.md) **>** [**qqmusic**](dir_d63c0418b33b823a308efea67b8f3df2.md) **>** [**toplist.h**](toplist_8h.md)

[Go to the documentation of this file](toplist_8h.md)


```C++

#ifndef QQMUSIC_TOP_H
#define QQMUSIC_TOP_H

#include <qqmusic/result.h>

namespace qqmusic {

Task<Result<nlohmann::json>> get_toplist_category();

Task<Result<nlohmann::json>> get_toplist_item_detail(unsigned top_id,
                                                     unsigned num = 10,
                                                     unsigned page = 1,
                                                     bool tag = true);

} // namespace qqmusic

#endif // !QQMUSIC_TOP_H
```


