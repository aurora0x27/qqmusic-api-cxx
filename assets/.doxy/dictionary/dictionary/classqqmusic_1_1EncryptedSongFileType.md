

# Class qqmusic::EncryptedSongFileType



[**ClassList**](annotated.md) **>** [**qqmusic**](namespaceqqmusic.md) **>** [**EncryptedSongFileType**](classqqmusic_1_1EncryptedSongFileType.md)



_加密歌曲文件类型_ [More...](#detailed-description)

* `#include <song.h>`



Inherits the following classes: [qqmusic::BaseMediaFileType](classqqmusic_1_1BaseMediaFileType.md)














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Type**](#enum-type)  <br>_加密歌曲类型枚举_  |








































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**EncryptedSongFileType**](#function-encryptedsongfiletype-12) ([**Type**](classqqmusic_1_1EncryptedSongFileType.md#enum-type) type) <br>_通过类型枚举初始化_  |
|   | [**EncryptedSongFileType**](#function-encryptedsongfiletype-22) () <br>_默认初始化函数, 默认格式: FLAC格式_  |
| virtual bool | [**encrypted**](#function-encrypted) () override<br>_该格式是否加密_  |
| virtual std::string\_view | [**expandation**](#function-expandation) () override<br>_获取歌曲类ID扩展_  |
| virtual std::string\_view | [**prefix**](#function-prefix) () override<br>_获取歌曲类ID前缀_  |
|   | [**~EncryptedSongFileType**](#function-encryptedsongfiletype) () override<br> |


## Public Functions inherited from qqmusic::BaseMediaFileType

See [qqmusic::BaseMediaFileType](classqqmusic_1_1BaseMediaFileType.md)

| Type | Name |
| ---: | :--- |
| virtual bool | [**encrypted**](classqqmusic_1_1BaseMediaFileType.md#function-encrypted) () = 0<br>_是否加密_  |
| virtual std::string\_view | [**expandation**](classqqmusic_1_1BaseMediaFileType.md#function-expandation) () = 0<br>_获取扩展名_  |
| virtual std::string\_view | [**prefix**](classqqmusic_1_1BaseMediaFileType.md#function-prefix) () = 0<br>_获取基类ID_  |
| virtual  | [**~BaseMediaFileType**](classqqmusic_1_1BaseMediaFileType.md#function-basemediafiletype) () = default<br> |






















































## Detailed Description




**See also:** [**BaseMediaFileType**](classqqmusic_1_1BaseMediaFileType.md) 



    
## Public Types Documentation




### enum Type 

_加密歌曲类型枚举_ 
```C++
enum qqmusic::EncryptedSongFileType::Type {
    MASTER,
    ATMOS_2,
    ATMOS_51,
    FLAC,
    OGG_640,
    OGG_320,
    OGG_192,
    OGG_96
};
```




<hr>
## Public Functions Documentation




### function EncryptedSongFileType [1/2]

_通过类型枚举初始化_ 
```C++
inline qqmusic::EncryptedSongFileType::EncryptedSongFileType (
    Type type
) 
```





**Parameters:**


* `type` 类型枚举



**See also:** [**EncryptedSongFileType::Type**](classqqmusic_1_1EncryptedSongFileType.md#enum-type)


**Exception:**


* `若提供的枚举不合法` 




        

<hr>



### function EncryptedSongFileType [2/2]

_默认初始化函数, 默认格式: FLAC格式_ 
```C++
inline qqmusic::EncryptedSongFileType::EncryptedSongFileType () 
```





**See also:** [**SongFileType::Type**](classqqmusic_1_1SongFileType.md#enum-type) 



        

<hr>



### function encrypted 

_该格式是否加密_ 
```C++
inline virtual bool qqmusic::EncryptedSongFileType::encrypted () override
```





**Returns:**

总是返回`true` 





        
Implements [*qqmusic::BaseMediaFileType::encrypted*](classqqmusic_1_1BaseMediaFileType.md#function-encrypted)


<hr>



### function expandation 

_获取歌曲类ID扩展_ 
```C++
inline virtual std::string_view qqmusic::EncryptedSongFileType::expandation () override
```





**Returns:**

返回`std::string`格式的类型扩展名 





        
Implements [*qqmusic::BaseMediaFileType::expandation*](classqqmusic_1_1BaseMediaFileType.md#function-expandation)


<hr>



### function prefix 

_获取歌曲类ID前缀_ 
```C++
inline virtual std::string_view qqmusic::EncryptedSongFileType::prefix () override
```





**Returns:**

返回`std::string`格式的类型前缀 





        
Implements [*qqmusic::BaseMediaFileType::prefix*](classqqmusic_1_1BaseMediaFileType.md#function-prefix)


<hr>



### function ~EncryptedSongFileType 

```C++
qqmusic::EncryptedSongFileType::~EncryptedSongFileType () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/qqmusic/song.h`

