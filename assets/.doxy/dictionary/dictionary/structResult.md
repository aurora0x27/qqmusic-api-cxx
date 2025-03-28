

# Struct Result

**template &lt;typename T, typename E&gt;**



[**ClassList**](annotated.md) **>** [**Result**](structResult.md)






















## Public Types

| Type | Name |
| ---: | :--- |
| typedef [**details::Storage**](structdetails_1_1Storage.md)&lt; T, E &gt; | [**storage\_type**](#typedef-storage_type)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Result**](#function-result-14) ([**types::Ok**](structtypes_1_1Ok.md)&lt; T &gt; ok) <br> |
|   | [**Result**](#function-result-24) ([**types::Err**](structtypes_1_1Err.md)&lt; E &gt; err) <br> |
|   | [**Result**](#function-result-34) ([**Result**](structResult.md) && other) <br> |
|   | [**Result**](#function-result-44) (const [**Result**](structResult.md) & other) <br> |
|  T | [**expect**](#function-expect) (const char \* str) const<br> |
|  bool | [**isErr**](#function-iserr) () const<br> |
|  bool | [**isOk**](#function-isok) () const<br> |
|  Ret | [**map**](#function-map) (Func func) const<br> |
|  Ret | [**mapError**](#function-maperror) (Func func) const<br> |
|  Ret | [**orElse**](#function-orelse) (Func func) const<br> |
|  [**Result**](structResult.md)&lt; T, E &gt; | [**otherwise**](#function-otherwise) (Func func) const<br> |
|  [**storage\_type**](structdetails_1_1Storage.md) & | [**storage**](#function-storage-12) () <br> |
|  const [**storage\_type**](structdetails_1_1Storage.md) & | [**storage**](#function-storage-22) () const<br> |
|  [**Result**](structResult.md)&lt; T, E &gt; | [**then**](#function-then) (Func func) const<br> |
|  std::enable\_if&lt;!std::is\_same&lt; U, void &gt;::value, U &gt;::type | [**unwrap**](#function-unwrap) () const<br> |
|  E | [**unwrapErr**](#function-unwraperr) () const<br> |
|  std::enable\_if&lt;!std::is\_same&lt; U, void &gt;::value, U &gt;::type | [**unwrapOr**](#function-unwrapor) (const U & defaultValue) const<br> |
|   | [**~Result**](#function-result) () <br> |




























## Public Types Documentation




### typedef storage\_type 

```C++
typedef details::Storage<T, E> Result< T, E >::storage_type;
```




<hr>
## Public Functions Documentation




### function Result [1/4]

```C++
inline Result::Result (
    types::Ok < T > ok
) 
```




<hr>



### function Result [2/4]

```C++
inline Result::Result (
    types::Err < E > err
) 
```




<hr>



### function Result [3/4]

```C++
inline Result::Result (
    Result && other
) 
```




<hr>



### function Result [4/4]

```C++
inline Result::Result (
    const Result & other
) 
```




<hr>



### function expect 

```C++
inline T Result::expect (
    const char * str
) const
```




<hr>



### function isErr 

```C++
inline bool Result::isErr () const
```




<hr>



### function isOk 

```C++
inline bool Result::isOk () const
```




<hr>



### function map 

```C++
template<typename Func, typename Ret>
inline Ret Result::map (
    Func func
) const
```




<hr>



### function mapError 

```C++
template<typename Func, typename Ret>
inline Ret Result::mapError (
    Func func
) const
```




<hr>



### function orElse 

```C++
template<typename Func, typename Ret>
inline Ret Result::orElse (
    Func func
) const
```




<hr>



### function otherwise 

```C++
template<typename Func>
inline Result < T, E > Result::otherwise (
    Func func
) const
```




<hr>



### function storage [1/2]

```C++
inline storage_type & Result::storage () 
```




<hr>



### function storage [2/2]

```C++
inline const storage_type & Result::storage () const
```




<hr>



### function then 

```C++
template<typename Func>
inline Result < T, E > Result::then (
    Func func
) const
```




<hr>



### function unwrap 

```C++
template<typename U>
inline std::enable_if<!std::is_same< U, void >::value, U >::type Result::unwrap () const
```




<hr>



### function unwrapErr 

```C++
inline E Result::unwrapErr () const
```




<hr>



### function unwrapOr 

```C++
template<typename U>
inline std::enable_if<!std::is_same< U, void >::value, U >::type Result::unwrapOr (
    const U & defaultValue
) const
```




<hr>



### function ~Result 

```C++
inline Result::~Result () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/qqmusic/details/result.h`

