#include <qqmusic/result.h>

qqmusic::result::result(qqmusic::api_state     stat,
                        const std::string& desc)
{
    this->stat = stat;
    this->desc = desc;
}

qqmusic::result::~result() {}

qqmusic::api_state
qqmusic::result::get_state() const
{
    return this->stat;
}

std::string
qqmusic::result::get_description() const
{
    return this->desc;
}
