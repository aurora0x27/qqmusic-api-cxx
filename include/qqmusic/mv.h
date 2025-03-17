/*--------------------------------qqmusic/mv.h-----------------------------------
 * Api about music videos
 *-----------------------------------------------------------------------------*/
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
