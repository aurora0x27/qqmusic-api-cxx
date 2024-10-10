#include <cstdint>
#include <utils/buffer.h>

#include <stdlib.h>

// constructor
qqmusic::utils::buffer::buffer()
{
    data.size = 0;
    data.head = (uint8_t*)malloc(1);
}

// destructor
qqmusic::utils::buffer::~buffer()
{
    free(data.head);
}

size_t
qqmusic::utils::buffer::get_size()
{
    return data.size;
}

uint8_t*
qqmusic::utils::buffer::get_head()
{
    return data.head;
}

void
qqmusic::utils::buffer::builtin_memcpy(void* dest, void* src, size_t size)
{
    uint8_t* d = (uint8_t*)dest;
    uint8_t* s = (uint8_t*)src;
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

    uint8_t* tmp = (uint8_t*)realloc(buf->head, buf->size + data_real_size + 1);
    if (tmp == NULL) {
        return -1;
    }

    data.head = tmp;

    builtin_memcpy(&(buf->head[buf->size]), src_data_buf, data_real_size);
    buf->size += data_real_size;
    buf->head[buf->size] = '\0';
    return data_real_size;
}

void
qqmusic::utils::buffer::buf_clean()
{
    free(data.head);
    data.size = 0;
    data.head = (uint8_t*)malloc(1);
}

bool
qqmusic::utils::buffer::resize(size_t new_size)
{
    data.size = new_size;
    uint8_t* tmp = (uint8_t*)realloc(data.head, new_size + 1);
    if (tmp == NULL) {
        return true;
    }
    data.head = tmp;
    data.head[data.size] = '\0';
    return false;
}
