

# Class qqmusic::crypto::MapCipher



[**ClassList**](annotated.md) **>** [**qqmusic**](namespaceqqmusic.md) **>** [**crypto**](namespaceqqmusic_1_1crypto.md) **>** [**MapCipher**](classqqmusic_1_1crypto_1_1MapCipher.md)








Inherits the following classes: [qqmusic::crypto::Cipher](classqqmusic_1_1crypto_1_1Cipher.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**MapCipher**](#function-mapcipher) (const std::vector&lt; uint8\_t &gt; & key) <br> |
| virtual void | [**decrypt**](#function-decrypt) ([**qqmusic::utils::buffer**](classqqmusic_1_1utils_1_1buffer.md) & buf, size\_t offset) override<br>_map解密算法_  |


## Public Functions inherited from qqmusic::crypto::Cipher

See [qqmusic::crypto::Cipher](classqqmusic_1_1crypto_1_1Cipher.md)

| Type | Name |
| ---: | :--- |
| virtual void | [**decrypt**](classqqmusic_1_1crypto_1_1Cipher.md#function-decrypt) ([**qqmusic::utils::buffer**](classqqmusic_1_1utils_1_1buffer.md) & buf, size\_t offset) = 0<br>_密钥解密算法_  |
| virtual  | [**~Cipher**](classqqmusic_1_1crypto_1_1Cipher.md#function-cipher) () = default<br> |






















































## Public Functions Documentation




### function MapCipher 

```C++
explicit qqmusic::crypto::MapCipher::MapCipher (
    const std::vector< uint8_t > & key
) 
```




<hr>



### function decrypt 

_map解密算法_ 
```C++
virtual void qqmusic::crypto::MapCipher::decrypt (
    qqmusic::utils::buffer & buf,
    size_t offset
) override
```





**Parameters:**


* `buf` 待解密的密钥 
* `offset` 偏移量 




        
Implements [*qqmusic::crypto::Cipher::decrypt*](classqqmusic_1_1crypto_1_1Cipher.md#function-decrypt)


<hr>

------------------------------
The documentation for this class was generated from the following file `include/qqmusic/crypto/cipher_map.h`

