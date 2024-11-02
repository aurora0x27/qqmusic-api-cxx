//-------------utils/buffer.h---------------
//        http request buffer class
#ifndef BUFFER_H
#define BUFFER_H
#include <stddef.h>
#include <stdint.h>

namespace qqmusic::utils {
    class buffer {
    public:
                    buffer          ();
                    buffer          (size_t size);
                    buffer          (const uint8_t* src_head, size_t src_size);
                    ~buffer         ();
        size_t      get_size        () const ;
        uint8_t*    get_head        () const ;
        size_t      append          (const void*  src_data_buf,
                                           size_t data_buf_size);
        void        clear           ();
        bool        resize          (size_t new_size);
    private:
        uint8_t*    head;
        size_t      size;
        void        builtin_memcpy  (const void* src, void* dest, size_t size);
    };

    // this function is specially writen as http request callback
    size_t          http_writebuf  (void*  src_data_buf,
                                    size_t data_block_size,
                                    size_t data_block_num,
                                    void*  buffer_ptr);
}
#endif // !BUFFER_H
