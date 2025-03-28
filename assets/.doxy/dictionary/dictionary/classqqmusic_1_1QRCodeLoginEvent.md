

# Class qqmusic::QRCodeLoginEvent



[**ClassList**](annotated.md) **>** [**qqmusic**](namespaceqqmusic.md) **>** [**QRCodeLoginEvent**](classqqmusic_1_1QRCodeLoginEvent.md)






















## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Status**](#enum-status)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QRCodeLoginEvent**](#function-qrcodeloginevent-12) (Status stat) <br> |
|   | [**QRCodeLoginEvent**](#function-qrcodeloginevent-22) (unsigned int code) <br> |
|  std::string\_view | [**name**](#function-name) () const<br> |
|  bool | [**operator!=**](#function-operator) (const [**QRCodeLoginEvent**](classqqmusic_1_1QRCodeLoginEvent.md) & e) <br> |
|  bool | [**operator!=**](#function-operator_1) (Status stat) <br> |
|  bool | [**operator==**](#function-operator_2) (const [**QRCodeLoginEvent**](classqqmusic_1_1QRCodeLoginEvent.md) & e) <br> |
|  bool | [**operator==**](#function-operator_3) (Status stat) <br> |




























## Public Types Documentation




### enum Status 

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

```C++
inline qqmusic::QRCodeLoginEvent::QRCodeLoginEvent (
    Status stat
) 
```




<hr>



### function QRCodeLoginEvent [2/2]

```C++
inline qqmusic::QRCodeLoginEvent::QRCodeLoginEvent (
    unsigned int code
) 
```




<hr>



### function name 

```C++
inline std::string_view qqmusic::QRCodeLoginEvent::name () const
```




<hr>



### function operator!= 

```C++
inline bool qqmusic::QRCodeLoginEvent::operator!= (
    const QRCodeLoginEvent & e
) 
```




<hr>



### function operator!= 

```C++
inline bool qqmusic::QRCodeLoginEvent::operator!= (
    Status stat
) 
```




<hr>



### function operator== 

```C++
inline bool qqmusic::QRCodeLoginEvent::operator== (
    const QRCodeLoginEvent & e
) 
```




<hr>



### function operator== 

```C++
inline bool qqmusic::QRCodeLoginEvent::operator== (
    Status stat
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/qqmusic/login.h`

