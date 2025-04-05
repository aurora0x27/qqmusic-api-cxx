

# Namespace qqmusic::crypto::TeaCipher



[**Namespace List**](namespaces.md) **>** [**qqmusic**](namespaceqqmusic.md) **>** [**crypto**](namespaceqqmusic_1_1crypto.md) **>** [**TeaCipher**](namespaceqqmusic_1_1crypto_1_1TeaCipher.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**decrypt**](#function-decrypt) ([**qqmusic::utils::buffer**](classqqmusic_1_1utils_1_1buffer.md) & buf, size\_t offset, const std::vector&lt; uint8\_t &gt; & key) <br>_tea解密，每次解密8字节_  |




























## Public Functions Documentation




### function decrypt 

_tea解密，每次解密8字节_ 
```C++
void qqmusic::crypto::TeaCipher::decrypt (
    qqmusic::utils::buffer & buf,
    size_t offset,
    const std::vector< uint8_t > & key
) 
```





**Parameters:**


* `buf` 待解密的密钥 
* `offset` 偏移量 
* `key` 解密需要用到的密钥，在 key\_derive.cc 中生成 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/qqmusic/crypto/cipher_tea.h`

