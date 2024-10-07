#include <utils/buffer.h>
#include <stdlib.h>

// constructor
qqmusic::utils::buffer::buffer()
{
    data = (struct buf_struct*)malloc(sizeof(struct buf_struct));
    data->size = 0;
    data->head = (char*)malloc(1);
}

// destructor
qqmusic::utils::buffer::~buffer()
{
    free(data->head);
    free(data);
}

size_t
qqmusic::utils::buffer::get_size()
{
    return data->size;
}

char*
qqmusic::utils::buffer::get_head()
{
    return data->head;
}

void
qqmusic::utils::buffer::builtin_memcpy(void* dest, void* src, size_t size)
{
    char* d = (char*)dest;
    char* s = (char*)src;
    for (size_t i = 0; i < size; ++i) {
        s[i] = d[i];
    }
}

size_t
qqmusic::utils::buffer::buf_append(void*  src_data_buf, 
                                   size_t data_block_size, 
                                   size_t data_block_num, 
                                   void*  dest_data_buf)
{
    size_t data_real_size = data_block_size * data_block_num;
    struct buf_struct* buf = (struct buf_struct*)dest_data_buf;

    buf->head = (char*)realloc(buf->head, buf->size + data_real_size + 1);
    if (buf->head == NULL) {
        // logger...
        return 0;
    }

    builtin_memcpy(&(buf->head[buf->size]), src_data_buf, data_real_size);
    buf->size += data_real_size;
    buf->head[buf->size] = '\0';
    return data_real_size;
}
