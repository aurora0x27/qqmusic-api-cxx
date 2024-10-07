#ifndef BUFFER_H
#define BUFFER_H

#include <stdio.h>

namespace qqmusic::utils
{
    class buffer
    {
    public:
                buffer       ();
                ~buffer      ();
        size_t  get_size     ();
        char*   get_head     ();
        size_t  buf_append   (void*  src_data_buf, 
                              size_t data_block_size, 
                              size_t data_block_num, 
                              void*  dest_data_buf);
    private:
        struct buf_struct
        {
            char* head;
            int   size;
        }* data;
        void builtin_memcpy(void* dest, void* src, size_t size);
    };
}
#endif
