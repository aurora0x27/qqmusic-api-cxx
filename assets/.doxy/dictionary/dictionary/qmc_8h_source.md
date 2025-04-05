

# File qmc.h

[**File List**](files.md) **>** [**crypto**](dir_544e59fdeba566aba49a0802b8225c84.md) **>** [**qmc.h**](qmc_8h.md)

[Go to the documentation of this file](qmc_8h.md)


```C++

#ifndef QQMUSIC_CRYPTO_QMC_H
#define QQMUSIC_CRYPTO_QMC_H

#include <cstdint>
#include <memory>
#include <qqmusic/crypto/cipher_map.h>
#include <qqmusic/crypto/cipher_rc4.h>
#include <qqmusic/crypto/cipher_tea.h>
#include <qqmusic/crypto/key_derive.h>
#include <qqmusic/details/result.h>
#include <qqmusic/utils/buffer.h>
#include <vector>

namespace qqmusic::crypto {

class Decoder {
public:
    explicit Decoder(std::string&& ekey);

    bool decrypt();

    bool read2buf(qqmusic::utils::buffer&& buf);

private:
    qqmusic::utils::buffer ekey;                     
    std::vector<uint8_t> key;                        
    qqmusic::utils::buffer buf_in{1024};             
    qqmusic::utils::buffer buf_out{1024};            
    std::unique_ptr<qqmusic::crypto::Cipher> cipher; 
};
} // namespace qqmusic::crypto

#endif // !QQMUSIC_CRYPTO_QMC_H
```


