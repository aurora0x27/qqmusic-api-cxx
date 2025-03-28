

# Class qqmusic::TabType



[**ClassList**](annotated.md) **>** [**qqmusic**](namespaceqqmusic.md) **>** [**TabType**](classqqmusic_1_1TabType.md)



_标签类_ 

* `#include <singer.h>`

















## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Type**](#enum-type)  <br>_标签类型枚举_  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**TabType**](#function-tabtype-12) () = delete<br>_使用枚举值初始化标签类_  |
|   | [**TabType**](#function-tabtype-22) ([**Type**](classqqmusic_1_1TabType.md#enum-type) type) <br> |
|  std::string | [**get\_id**](#function-get_id) () const<br>_获取tab id_  |
|  std::string | [**get\_name**](#function-get_name) () const<br>_获取tab名称_  |




























## Public Types Documentation




### enum Type 

_标签类型枚举_ 
```C++
enum qqmusic::TabType::Type {
    WIKI,
    ALBUM,
    COMPOSER,
    LYRICIST,
    PRODUCER,
    ARRANGER,
    MUSICIAN,
    SONG,
    VIDEO
};
```




<hr>
## Public Functions Documentation




### function TabType [1/2]

_使用枚举值初始化标签类_ 
```C++
qqmusic::TabType::TabType () = delete
```





**Parameters:**


* `type` 标签类型枚举



**Exception:**


* `若提供的枚举值不合法` 




        

<hr>



### function TabType [2/2]

```C++
inline qqmusic::TabType::TabType (
    Type type
) 
```




<hr>



### function get\_id 

_获取tab id_ 
```C++
inline std::string qqmusic::TabType::get_id () const
```





**Returns:**

std::string格式tab id 





        

<hr>



### function get\_name 

_获取tab名称_ 
```C++
inline std::string qqmusic::TabType::get_name () const
```





**Returns:**

std::string格式tab name 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/qqmusic/singer.h`

