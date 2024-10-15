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
                 buffer       ();
                 ~buffer      ();
        size_t   get_size     ();
        uint8_t* get_head     ();
        size_t   buf_append   (void*  src_data_buf, 
                               size_t data_block_size, 
                               size_t data_block_num, 
                               void*  dest_data_buf);
        void     buf_clean    ();
    private:
        struct buf_struct
        {
            uint8_t* head;
            int      size;
        }* data;
        void builtin_memcpy(void* dest, void* src, size_t size);
    };
}
#endif
