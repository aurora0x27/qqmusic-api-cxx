//-------------qqmusic/result.h---------------
//       global api result and error
#ifndef RESULT_H
#define RESULT_H

#include <string>

namespace qqmusic {

enum class api_state {
    ok,
    error
};

class result {
public:
                        result          () = delete;
                        result          (qqmusic::api_state stat,
                                        const std::string& desc);
                        ~result         ();
    qqmusic::api_state  get_state       () const;
    std::string         get_description () const;

private:
    qqmusic::api_state   stat;
    std::string          desc;
};

} // namespace qqmusic

#endif // !RESULT_H
