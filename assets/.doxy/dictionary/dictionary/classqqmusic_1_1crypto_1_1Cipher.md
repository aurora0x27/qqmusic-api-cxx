

# Class qqmusic::crypto::Cipher



[**ClassList**](annotated.md) **>** [**qqmusic**](namespaceqqmusic.md) **>** [**crypto**](namespaceqqmusic_1_1crypto.md) **>** [**Cipher**](classqqmusic_1_1crypto_1_1Cipher.md)










Inherited by the following classes: [qqmusic::crypto::MapCipher](classqqmusic_1_1crypto_1_1MapCipher.md),  [qqmusic::crypto::RC4Cipher](classqqmusic_1_1crypto_1_1RC4Cipher.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual void | [**decrypt**](#function-decrypt) ([**qqmusic::utils::buffer**](classqqmusic_1_1utils_1_1buffer.md) & buf, size\_t offset) = 0<br>_密钥解密算法_  |
| virtual  | [**~Cipher**](#function-cipher) () = default<br> |




























## Public Functions Documentation




### function decrypt 

_密钥解密算法_ 
```C++
virtual void qqmusic::crypto::Cipher::decrypt (
    qqmusic::utils::buffer & buf,
    size_t offset
) = 0
```





**Parameters:**


* `buf` 待解密的密钥 
* `offset` 偏移量 




        

<hr>



### function ~Cipher 

```C++
virtual qqmusic::crypto::Cipher::~Cipher () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/qqmusic/crypto/cipher.h`

