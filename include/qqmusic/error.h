
//-------------qqmusic/error.h---------------
//            global error info
#ifndef ERROR_H
#define ERROR_H

#include <string>

namespace qqmusic {

enum class state {
    ok,
    error
};

class result {
public:
                    result          () = delete;
                    result          (qqmusic::state     stat,
                                     const std::string& desc);
                    ~result         ();
    qqmusic::state  get_state       () const;
    std::string     get_description () const;

private:
    qqmusic::state       stat;
    std::string          desc;
};

} // namespace qqmusic

#endif // !ERROR_H
