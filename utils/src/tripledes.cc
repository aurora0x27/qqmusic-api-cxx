#include <cstdio>
#include <utils/tripledes.h>

static std::vector<std::vector<uint32_t>>
key_schedule(uint8_t* key_head, size_t key_size, qqmusic::utils::tripledes_crypt_mode mode);

static uint32_t
bitnum(uint8_t* head, size_t size, uint32_t b, uint32_t c);

static uint32_t
bitnum_intl(uint32_t a, uint32_t b, uint32_t c);

static uint32_t
bitnum_intr(uint32_t a, uint32_t b, uint32_t c);

static void
crypt(qqmusic::utils::buffer* buf, std::vector<std::vector<uint32_t>> key);

static void
initial_permutation(uint32_t* a, uint32_t* b, qqmusic::utils::buffer* buf);

static void
inverse_permutation(uint32_t a, uint32_t b, qqmusic::utils::buffer* buf);

static uint32_t
transform(uint32_t state, std::vector<uint32_t> key);

static uint32_t
sbox_bit(uint32_t a);

// extern api
qqmusic::utils::tripledes_key_schedule
qqmusic::utils::tripledes_key_setup(uint8_t*                             key_head,
                                    size_t                               key_size,
                                    qqmusic::utils::tripledes_crypt_mode mode)
{
    std::vector<std::vector<std::vector<uint32_t>>> res;

    if (mode == qqmusic::utils::tripledes_crypt_mode::encrypt) {
        res.push_back(key_schedule(key_head, key_size, qqmusic::utils::tripledes_crypt_mode::encrypt));
        res.push_back(key_schedule(key_head + 8, key_size - 8, qqmusic::utils::tripledes_crypt_mode::decrypt));
        res.push_back(key_schedule(key_head + 16, key_size - 16, qqmusic::utils::tripledes_crypt_mode::encrypt));
    } else {
        res.push_back(key_schedule(key_head + 16, key_size - 16, qqmusic::utils::tripledes_crypt_mode::decrypt));
        res.push_back(key_schedule(key_head + 8, key_size - 8, qqmusic::utils::tripledes_crypt_mode::encrypt));
        res.push_back(key_schedule(key_head, key_size, qqmusic::utils::tripledes_crypt_mode::decrypt));
    }

    return res;
}

// extern api
void
qqmusic::utils::tripledes_crypt(qqmusic::utils::buffer*                buf_in,
                                qqmusic::utils::buffer*                buf_out,
                                qqmusic::utils::tripledes_key_schedule key_schedule)
{
    for (int i = 0; i < 3; ++i) {
        crypt(buf_in, key_schedule[i]);
    }

    // insert processed data into output buffer
    buf_out->append(buf_in->get_head(), buf_in->get_size());
}

static std::vector<std::vector<uint32_t>>
key_schedule(uint8_t* key_head, size_t key_size, qqmusic::utils::tripledes_crypt_mode mode)
{
    std::vector<std::vector<uint32_t>> schedule(16, std::vector<uint32_t>(6, 0L));
    const uint32_t key_rnd_shift[]   = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};
    const uint32_t key_perm_c[]      = {56, 48, 40, 32, 24, 16, 8, 0, 57, 49, 41, 33, 25, 17,
                                        9, 1, 58, 50, 42, 34, 26, 18, 10, 2, 59, 51, 43, 35};
    const uint32_t key_perm_d[]      = {62, 54, 46, 38, 30, 22, 14, 6, 61, 53, 45, 37, 29, 21,
                                        13, 5, 60, 52, 44, 36, 28, 20, 12, 4, 27, 19, 11, 3};
    const uint32_t key_compression[] = {13, 16, 10, 23, 0, 4, 2, 27, 14, 5, 20, 9, 22, 18, 11, 3, 25,
                                        7, 15, 6, 26, 19, 12, 1, 40, 51, 30, 36, 46, 54, 29, 39, 50,
                                        44, 32, 47, 43, 48, 38, 55, 33, 52, 45, 41, 49, 35, 28, 31};

    uint32_t c = 0, d = 0;
    for (int i = 0; i < 28; ++i) {
        c += bitnum(key_head, key_size, key_perm_c[i], 31 - i);
        d += bitnum(key_head, key_size, key_perm_d[i], 31 - i);
    }

    for (int i = 0; i < 16; ++i) {
        c = ((c << key_rnd_shift[i]) | (c >> (28 - key_rnd_shift[i]))) & 0xfffffff0;
        d = ((d << key_rnd_shift[i]) | (d >> (28 - key_rnd_shift[i]))) & 0xfffffff0;

        int togen = mode == qqmusic::utils::tripledes_crypt_mode::decrypt ? 15 - i : i;

        for (int j = 0; j < 6; ++j) {
            schedule[togen][j] = 0;
        }

        for (int j = 0; j < 24; ++j) {
            schedule[togen][j / 8] |= bitnum_intr(c, key_compression[j], 7 - (j % 8));
        }

        for (int j = 24; j < 48; ++j){
            schedule[togen][j / 8] |= bitnum_intr(d, key_compression[j] - 27, 7 - (j % 8));
        }
    }

    return schedule;
}

static uint32_t
bitnum(uint8_t* head, size_t size, uint32_t b, uint32_t c)
{
    return ((head[(b / 32) * 4 + 3 - (b % 32) / 8] >> (7 - b % 8)) & 1) << c;
}

static uint32_t
bitnum_intl(uint32_t a, uint32_t b, uint32_t c)
{
    return ((a << b) & 0x80000000) >> c;
}

static uint32_t
bitnum_intr(uint32_t a, uint32_t b, uint32_t c)
{
    return ((a >> (31 - b)) & 1) << c;
}

static void
crypt(qqmusic::utils::buffer* buf, std::vector<std::vector<uint32_t>> key)
{
    uint32_t a = 0, b = 0;
    initial_permutation(&a, &b, buf);

    for (int i = 0; i < 15; ++i) {
        long tmp = b;
        b = transform(b, key[i]) ^ a;
        a = tmp;
    }
    a = transform(b, key[15]) ^ a;

    inverse_permutation(a, b, buf);
}

static void
initial_permutation(uint32_t* a, uint32_t* b,qqmusic::utils::buffer* buf)
{
    uint8_t* input_data = buf->get_head();
    size_t   size       = buf->get_size();

    *a = (bitnum(input_data, size, 57, 31) | bitnum(input_data, size, 49, 30) | bitnum(input_data, size, 41, 29) | bitnum(input_data, size, 33, 28) |
          bitnum(input_data, size, 25, 27) | bitnum(input_data, size, 17, 26) | bitnum(input_data, size,  9, 25) | bitnum(input_data, size,  1, 24) |  
          bitnum(input_data, size, 59, 23) | bitnum(input_data, size, 51, 22) | bitnum(input_data, size, 43, 21) | bitnum(input_data, size, 35, 20) |
          bitnum(input_data, size, 27, 19) | bitnum(input_data, size, 19, 18) | bitnum(input_data, size, 11, 17) | bitnum(input_data, size,  3, 16) | 
          bitnum(input_data, size, 61, 15) | bitnum(input_data, size, 53, 14) | bitnum(input_data, size, 45, 13) | bitnum(input_data, size, 37, 12) |
          bitnum(input_data, size, 29, 11) | bitnum(input_data, size, 21, 10) | bitnum(input_data, size, 13, 9)  | bitnum(input_data, size,  5, 8)  |
          bitnum(input_data, size, 63, 7)  | bitnum(input_data, size, 55, 6)  | bitnum(input_data, size, 47, 5)  | bitnum(input_data, size, 39, 4)  |  
          bitnum(input_data, size, 31, 3)  | bitnum(input_data, size, 23, 2)  | bitnum(input_data, size, 15, 1)  | bitnum(input_data, size,  7, 0));

    *b = (bitnum(input_data, size, 56, 31) | bitnum(input_data, size, 48, 30) | bitnum(input_data, size, 40, 29) | bitnum(input_data, size, 32, 28) |
          bitnum(input_data, size, 24, 27) | bitnum(input_data, size, 16, 26) | bitnum(input_data, size,  8, 25) | bitnum(input_data, size,  0, 24) |
          bitnum(input_data, size, 58, 23) | bitnum(input_data, size, 50, 22) | bitnum(input_data, size, 42, 21) | bitnum(input_data, size, 34, 20) |
          bitnum(input_data, size, 26, 19) | bitnum(input_data, size, 18, 18) | bitnum(input_data, size, 10, 17) | bitnum(input_data, size,  2, 16) |
          bitnum(input_data, size, 60, 15) | bitnum(input_data, size, 52, 14) | bitnum(input_data, size, 44, 13) | bitnum(input_data, size, 36, 12) |
          bitnum(input_data, size, 28, 11) | bitnum(input_data, size, 20, 10) | bitnum(input_data, size, 12, 9)  | bitnum(input_data, size,  4, 8)  |
          bitnum(input_data, size, 62, 7)  | bitnum(input_data, size, 54, 6)  | bitnum(input_data, size, 46, 5)  | bitnum(input_data, size, 38, 4)  |
          bitnum(input_data, size, 30, 3)  | bitnum(input_data, size, 22, 2)  | bitnum(input_data, size, 14, 1)  | bitnum(input_data, size,  6, 0));
}

static void
inverse_permutation(uint32_t a, uint32_t b, qqmusic::utils::buffer* buf)
{
    uint8_t data[8] = {0};

    data[3] = (bitnum_intr(b, 7,  7) | bitnum_intr(a, 7,  6) | bitnum_intr(b, 15, 5) |
               bitnum_intr(a, 15, 4) | bitnum_intr(b, 23, 3) | bitnum_intr(a, 23, 2) |
               bitnum_intr(b, 31, 1) | bitnum_intr(a, 31, 0));

    data[2] = (bitnum_intr(b, 6,  7) | bitnum_intr(a, 6,  6) | bitnum_intr(b, 14, 5) |
               bitnum_intr(a, 14, 4) | bitnum_intr(b, 22, 3) | bitnum_intr(a, 22, 2) |
               bitnum_intr(b, 30, 1) | bitnum_intr(a, 30, 0));

    data[1] = (bitnum_intr(b, 5,  7) | bitnum_intr(a, 5,  6) | bitnum_intr(b, 13, 5) |
               bitnum_intr(a, 13, 4) | bitnum_intr(b, 21, 3) | bitnum_intr(a, 21, 2) |
               bitnum_intr(b, 29, 1) | bitnum_intr(a, 29, 0));

    data[0] = (bitnum_intr(b, 4,  7) | bitnum_intr(a, 4,  6) | bitnum_intr(b, 12, 5) |
               bitnum_intr(a, 12, 4) | bitnum_intr(b, 20, 3) | bitnum_intr(a, 20, 2) |
               bitnum_intr(b, 28, 1) | bitnum_intr(a, 28, 0));

    data[7] = (bitnum_intr(b, 3,  7) | bitnum_intr(a, 3, 6)  | bitnum_intr(b, 11, 5) |
               bitnum_intr(a, 11, 4) | bitnum_intr(b, 19, 3) | bitnum_intr(a, 19, 2) |
               bitnum_intr(b, 27, 1) | bitnum_intr(a, 27, 0));

    data[6] = (bitnum_intr(b, 2,  7) | bitnum_intr(a, 2,  6) | bitnum_intr(b, 10, 5) |
               bitnum_intr(a, 10, 4) | bitnum_intr(b, 18, 3) | bitnum_intr(a, 18, 2) |
               bitnum_intr(b, 26, 1) | bitnum_intr(a, 26, 0));

    data[5] = (bitnum_intr(b, 1, 7) | bitnum_intr(a, 1,  6) | bitnum_intr(b, 9, 5)  |
               bitnum_intr(a, 9, 4) | bitnum_intr(b, 17, 3) | bitnum_intr(a, 17, 2) |
               bitnum_intr(b, 25, 1)| bitnum_intr(a, 25, 0));

    data[4] = (bitnum_intr(b, 0, 7) | bitnum_intr(a, 0,  6) | bitnum_intr(b, 8,  5) |
               bitnum_intr(a, 8, 4) | bitnum_intr(b, 16, 3) | bitnum_intr(a, 16, 2) |
               bitnum_intr(b, 24,1) | bitnum_intr(a, 24, 0));

    buf->clear();
    buf->append(data, 8);
}

static uint32_t
transform(uint32_t state, std::vector<uint32_t> key)
{
    uint32_t sbox[8][64] = {
        // sbox1
        {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
        0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
        4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
        15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13},

        // sbox2
        {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
        3, 13, 4, 7, 15, 2, 8, 15, 12, 0, 1, 10, 6, 9, 11, 5,
        0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
        13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9},

        // sbox3
        {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
        13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
        13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
        1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12},

        // sbox4
        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
        13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
        10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
        3, 15, 0, 6, 10, 10, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14},

        // sbox5
        {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
        14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
        4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
        11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3},

        // sbox6
        {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
        10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
        9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
        4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13},

        // sbox7
        {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
        13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
        1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
        6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12},

        // sbox8
        {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
        1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
        7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
        2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11},
    };

    uint32_t t1 = (bitnum_intl(state, 31,  0) | ((state & 0xf0000000) >> 1) | bitnum_intl(state, 4,   5) |
                   bitnum_intl(state,  3,  6) | ((state & 0x0f000000) >> 3) | bitnum_intl(state, 8,  11) |
                   bitnum_intl(state,  7, 12) | ((state & 0x00f00000) >> 5) | bitnum_intl(state, 12, 17) |
                   bitnum_intl(state, 11, 18) | ((state & 0x000f0000) >> 7) | bitnum_intl(state, 16, 23));

    uint32_t t2 = (bitnum_intl(state, 15,  0) | ((state & 0x0000f000) << 15) | bitnum_intl(state, 20,  5) |
                   bitnum_intl(state, 19,  6) | ((state & 0x00000f00) << 13) | bitnum_intl(state, 24, 11) |
                   bitnum_intl(state, 23, 12) | ((state & 0x000000f0) << 11) | bitnum_intl(state, 28, 17) |
                   bitnum_intl(state, 27, 18) | ((state & 0x0000000f) <<  9) | bitnum_intl(state, 0,  23));

    uint32_t lrgstate[] = {
        (t1 >> 24) & 0x000000ff, (t1 >> 16) & 0x000000ff, (t1 >> 8) & 0x000000ff,
        (t2 >> 24) & 0x000000ff, (t2 >> 16) & 0x000000ff, (t2 >> 8) & 0x000000ff,
    };

    for (int i = 0; i < 6; ++i) {
        lrgstate[i] ^= key[i];
    }

    state = ((sbox[0][sbox_bit(lrgstate[0] >> 2)] << 28) |
             (sbox[1][sbox_bit(((lrgstate[0] & 0x03) << 4) | (lrgstate[1] >> 4))] << 24) |
             (sbox[2][sbox_bit(((lrgstate[1] & 0x0f) << 2) | (lrgstate[2] >> 6))] << 20) |
             (sbox[3][sbox_bit(lrgstate[2] & 0x3f)] << 16) |
             (sbox[4][sbox_bit(lrgstate[3] >> 2)] << 12) |
             (sbox[5][sbox_bit(((lrgstate[3] & 0x03) << 4) | (lrgstate[4] >> 4))] << 8) |
             (sbox[6][sbox_bit(((lrgstate[4] & 0x0f) << 2) | (lrgstate[5] >> 6))] << 4) |
              sbox[7][sbox_bit(lrgstate[5] & 0x3f)]);

    return (bitnum_intl(state, 15,  0) | bitnum_intl(state,  6,  1) | bitnum_intl(state, 19,  2) |
            bitnum_intl(state, 20,  3) | bitnum_intl(state, 28,  4) | bitnum_intl(state, 11,  5) |
            bitnum_intl(state, 27,  6) | bitnum_intl(state, 16,  7) | bitnum_intl(state,  0,  8) |
            bitnum_intl(state, 14,  9) | bitnum_intl(state, 22, 10) | bitnum_intl(state, 25, 11) |
            bitnum_intl(state,  4, 12) | bitnum_intl(state, 17, 13) | bitnum_intl(state, 30, 14) |
            bitnum_intl(state,  9, 15) | bitnum_intl(state,  1, 16) | bitnum_intl(state,  7, 17) |
            bitnum_intl(state, 23, 18) | bitnum_intl(state, 13, 19) | bitnum_intl(state, 31, 20) |
            bitnum_intl(state, 26, 21) | bitnum_intl(state,  2, 22) | bitnum_intl(state,  8, 23) |
            bitnum_intl(state, 18, 24) | bitnum_intl(state, 12, 25) | bitnum_intl(state, 29, 26) |
            bitnum_intl(state,  5, 27) | bitnum_intl(state, 21, 28) | bitnum_intl(state, 10, 29) |
            bitnum_intl(state,  3, 30) | bitnum_intl(state, 24, 31));
}

static uint32_t
sbox_bit(uint32_t a)
{
    return (a & 32) | ((a & 31) >> 1) | ((a & 1) << 4);
}
