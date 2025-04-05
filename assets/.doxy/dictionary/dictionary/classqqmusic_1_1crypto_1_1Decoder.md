

# Class qqmusic::crypto::Decoder



[**ClassList**](annotated.md) **>** [**qqmusic**](namespaceqqmusic.md) **>** [**crypto**](namespaceqqmusic_1_1crypto.md) **>** [**Decoder**](classqqmusic_1_1crypto_1_1Decoder.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Decoder**](#function-decoder) (std::string && ekey) <br>_解码器构造函数_  |
|  bool | [**decrypt**](#function-decrypt) () <br>_解码器完整解码过程_  |
|  bool | [**read2buf**](#function-read2buf) ([**qqmusic::utils::buffer**](classqqmusic_1_1utils_1_1buffer.md) && buf) <br>_存取待解密数据到 buf\_in 中_  |




























## Public Functions Documentation




### function Decoder 

_解码器构造函数_ 
```C++
explicit qqmusic::crypto::Decoder::Decoder (
    std::string && ekey
) 
```





**Parameters:**


* `ekey` 需要传入加密的密钥 




        

<hr>



### function decrypt 

_解码器完整解码过程_ 
```C++
bool qqmusic::crypto::Decoder::decrypt () 
```




* 解密密钥
* 初始化解密算法
* 进行解密 




        

<hr>



### function read2buf 

_存取待解密数据到 buf\_in 中_ 
```C++
bool qqmusic::crypto::Decoder::read2buf (
    qqmusic::utils::buffer && buf
) 
```





**Parameters:**


* `buf` 缓冲区（存放未解密的数据） 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/qqmusic/crypto/qmc.h`

