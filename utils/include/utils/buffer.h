//-------------utils/buffer.h---------------
//        http request buffer class
#ifndef BUFFER_H
#define BUFFER_H

#include <stdio.h>
#include <stdint.h>

namespace qqmusic::utils
{
    class buffer
    {
    public:
                 buffer          ();
                 buffer          (uint8_t* src_head, size_t size);
                 ~buffer         ();
        size_t   get_size        ();
        uint8_t* get_head        ();
        // change buffer size
        bool     resize          (size_t new_size);

        // use these ONLY for http write buffer: get_data_addr for CURLOPT_WRITEDATA
        // and get_data_addr for CURLOPT_WRITEFUNCTION
        void*    get_data_addr   ();
        size_t   http_buf_append (void*  src_data_buf, 
                                  size_t data_block_size, 
                                  size_t data_block_num, 
                                  void*  dest_data_buf);

        bool     buf_append      (void*  src_data_buf,
                                  size_t data_buf_size);

        void     buf_clean       ();
    private:
        struct buf_struct
        {
            uint8_t* head;
            int      size;
        } data;
        void builtin_memcpy(void* dest, void* src, size_t size);
    };
}
#endif
