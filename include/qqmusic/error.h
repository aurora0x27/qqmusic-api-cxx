
//-------------qqmusic/error.h---------------
//            global error info
namespace qqmusic
{
    enum class result
    {
        // normally return this
        excecuted_success,

        // net work error
        network_connect_timeout,

        mem_alloc_error,
        data_destroy,

        // cannot sure what happened
        unknown_error
    };
}
// namespace qqmusic
