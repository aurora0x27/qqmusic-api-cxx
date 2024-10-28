#include <cstddef>
#include <cstring>
#include <stdlib.h>

#include <utils/buffer.h>

qqmusic::utils::buffer::buffer()
{
    this->head = (uint8_t*)malloc(1);
    this->size = 0;
    this->head[0] = '\0';
}

qqmusic::utils::buffer::buffer(size_t size)
{
    this->head = (uint8_t*)malloc(size + 1);
    this->size = size;
    memset(this->head, '\0', size + 1);
}

qqmusic::utils::buffer::buffer(const uint8_t* src_head, size_t src_size)
{
    this->head = (uint8_t*)malloc(src_size + 1);
    this->head[src_size] = '\0';
    this->size = src_size;
    builtin_memcpy(src_head, this->head, src_size);
}

qqmusic::utils::buffer::~buffer()
{
    free(this->head);
}

size_t
qqmusic::utils::buffer::append(const void*  src_data_buf,
                                     size_t data_buf_size)
{
    uint8_t* tmp = (uint8_t*)realloc(this->head, this->size + data_buf_size + 1);
    if (tmp == NULL) {
        return -1;
    }
    this->head = tmp;
    builtin_memcpy(src_data_buf, &(this->head[this->size]), data_buf_size);
    this->size = data_buf_size + this->size;
    return data_buf_size;
}

size_t
qqmusic::utils::buffer::get_size()
{
    return this->size;
}

uint8_t*
qqmusic::utils::buffer::get_head()
{
    return this->head;
}

void
qqmusic::utils::buffer::clear()
{
    free(this->head);
    this->head = (uint8_t*)malloc(1);
    this->head[0] = '\0';
    this->size = 0;
}

bool
qqmusic::utils::buffer::resize(size_t new_size)
{
    uint8_t* tmp = (uint8_t*)realloc(this->head, new_size + 1);
    if (tmp == NULL) {
        return true;
    }
    this->head = tmp;
    this->head[new_size] = '\0';
    this->size = new_size;
    return false;
}

void
qqmusic::utils::buffer::builtin_memcpy (const void* src, void* dest, size_t size)
{
    uint8_t* d = (uint8_t*)dest;
    uint8_t* s = (uint8_t*)src;
    for (size_t i = 0; i < size; ++i) {
        d[i] = s[i];
    }
}

size_t
qqmusic::utils::http_writebuf(void*  src_data_buf,
                              size_t data_block_size,
                              size_t data_block_num,
                              void*  buffer_ptr)
{
    utils::buffer* buf = (utils::buffer*)buffer_ptr;
    return buf->append(src_data_buf, data_block_num * data_block_size);
}
