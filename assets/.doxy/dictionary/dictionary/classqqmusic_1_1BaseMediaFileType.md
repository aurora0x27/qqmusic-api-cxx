

# Class qqmusic::BaseMediaFileType



[**ClassList**](annotated.md) **>** [**qqmusic**](namespaceqqmusic.md) **>** [**BaseMediaFileType**](classqqmusic_1_1BaseMediaFileType.md)



_媒体文件类型基类_ 

* `#include <song.h>`





Inherited by the following classes: [qqmusic::EncryptedSongFileType](classqqmusic_1_1EncryptedSongFileType.md),  [qqmusic::SongFileType](classqqmusic_1_1SongFileType.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual bool | [**encrypted**](#function-encrypted) () = 0<br>_是否加密_  |
| virtual std::string\_view | [**expandation**](#function-expandation) () = 0<br>_获取扩展名_  |
| virtual std::string\_view | [**prefix**](#function-prefix) () = 0<br>_获取基类ID_  |
| virtual  | [**~BaseMediaFileType**](#function-basemediafiletype) () = default<br> |




























## Public Functions Documentation




### function encrypted 

_是否加密_ 
```C++
virtual bool qqmusic::BaseMediaFileType::encrypted () = 0
```




<hr>



### function expandation 

_获取扩展名_ 
```C++
virtual std::string_view qqmusic::BaseMediaFileType::expandation () = 0
```




<hr>



### function prefix 

_获取基类ID_ 
```C++
virtual std::string_view qqmusic::BaseMediaFileType::prefix () = 0
```




<hr>



### function ~BaseMediaFileType 

```C++
virtual qqmusic::BaseMediaFileType::~BaseMediaFileType () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/qqmusic/song.h`

