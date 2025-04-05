

# Class qqmusic::QRCodeLoginEvent



[**ClassList**](annotated.md) **>** [**qqmusic**](namespaceqqmusic.md) **>** [**QRCodeLoginEvent**](classqqmusic_1_1QRCodeLoginEvent.md)



_二维码登陆事件_ 

* `#include <login.h>`

















## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Status**](#enum-status)  <br>_二维码状态_  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QRCodeLoginEvent**](#function-qrcodeloginevent-12) ([**Status**](classqqmusic_1_1QRCodeLoginEvent.md#enum-status) stat) <br>_从状态构造_  |
|   | [**QRCodeLoginEvent**](#function-qrcodeloginevent-22) (unsigned int code) <br>_从Http请求返回的状态码构造_  |
|  std::string\_view | [**name**](#function-name) () const<br>_获取当前状态的名字_  |
|  bool | [**operator!=**](#function-operator) (const [**QRCodeLoginEvent**](classqqmusic_1_1QRCodeLoginEvent.md) & e) <br>_状态是否不相同_  |
|  bool | [**operator!=**](#function-operator_1) ([**Status**](classqqmusic_1_1QRCodeLoginEvent.md#enum-status) stat) <br>_和枚举直接比较是否不相同_  |
|  bool | [**operator==**](#function-operator_2) (const [**QRCodeLoginEvent**](classqqmusic_1_1QRCodeLoginEvent.md) & e) <br>_状态是否相同_  |
|  bool | [**operator==**](#function-operator_3) ([**Status**](classqqmusic_1_1QRCodeLoginEvent.md#enum-status) stat) <br>_和枚举直接比较是否相同_  |




























## Public Types Documentation




### enum Status 

_二维码状态_ 
```C++
enum qqmusic::QRCodeLoginEvent::Status {
    DONE,
    SCAN,
    CONF,
    REFUSE,
    TIMEOUT,
    OTHER
};
```




<hr>
## Public Functions Documentation




### function QRCodeLoginEvent [1/2]

_从状态构造_ 
```C++
inline qqmusic::QRCodeLoginEvent::QRCodeLoginEvent (
    Status stat
) 
```




<hr>



### function QRCodeLoginEvent [2/2]

_从Http请求返回的状态码构造_ 
```C++
inline qqmusic::QRCodeLoginEvent::QRCodeLoginEvent (
    unsigned int code
) 
```




<hr>



### function name 

_获取当前状态的名字_ 
```C++
inline std::string_view qqmusic::QRCodeLoginEvent::name () const
```




<hr>



### function operator!= 

_状态是否不相同_ 
```C++
inline bool qqmusic::QRCodeLoginEvent::operator!= (
    const QRCodeLoginEvent & e
) 
```




<hr>



### function operator!= 

_和枚举直接比较是否不相同_ 
```C++
inline bool qqmusic::QRCodeLoginEvent::operator!= (
    Status stat
) 
```




<hr>



### function operator== 

_状态是否相同_ 
```C++
inline bool qqmusic::QRCodeLoginEvent::operator== (
    const QRCodeLoginEvent & e
) 
```




<hr>



### function operator== 

_和枚举直接比较是否相同_ 
```C++
inline bool qqmusic::QRCodeLoginEvent::operator== (
    Status stat
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/qqmusic/login.h`

