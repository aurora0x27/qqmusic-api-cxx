

# Class qqmusic::crypto::KeyDerive



[**ClassList**](annotated.md) **>** [**qqmusic**](namespaceqqmusic.md) **>** [**crypto**](namespaceqqmusic_1_1crypto.md) **>** [**KeyDerive**](classqqmusic_1_1crypto_1_1KeyDerive.md)












































## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**qqmusic::utils::buffer**](classqqmusic_1_1utils_1_1buffer.md) | [**derive**](#function-derive) ([**qqmusic::utils::buffer**](classqqmusic_1_1utils_1_1buffer.md) && raw\_key) <br>_主要的密钥派生函数，处理派生逻辑 当检测到V2前缀时，先进行V2解密，然后进行V1解密 如果没有V2前缀，则直接进行V1解密_  |


























## Public Static Functions Documentation




### function derive 

_主要的密钥派生函数，处理派生逻辑 当检测到V2前缀时，先进行V2解密，然后进行V1解密 如果没有V2前缀，则直接进行V1解密_ 
```C++
static qqmusic::utils::buffer qqmusic::crypto::KeyDerive::derive (
    qqmusic::utils::buffer && raw_key
) 
```





**Parameters:**


* `raw_key` 原始密钥 



**Returns:**

[**qqmusic::utils::buffer**](classqqmusic_1_1utils_1_1buffer.md) 解密后的密钥 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/qqmusic/crypto/key_derive.h`

