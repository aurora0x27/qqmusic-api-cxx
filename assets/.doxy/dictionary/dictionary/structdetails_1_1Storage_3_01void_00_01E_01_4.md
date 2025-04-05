

# Struct details::Storage&lt; void, E &gt;

**template &lt;typename E&gt;**



[**ClassList**](annotated.md) **>** [**details**](namespacedetails.md) **>** [**Storage&lt; void, E &gt;**](structdetails_1_1Storage_3_01void_00_01E_01_4.md)






















## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::aligned\_storage&lt; sizeof(E), alignof(E)&gt;::type | [**type**](#typedef-type)  <br> |




## Public Attributes

| Type | Name |
| ---: | :--- |
|  bool | [**initialized\_**](#variable-initialized_)  <br> |
|  type | [**storage\_**](#variable-storage_)  <br> |
















## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**construct**](#function-construct-12) ([**types::Ok**](structtypes_1_1Ok.md)&lt; void &gt;) <br> |
|  void | [**construct**](#function-construct-22) ([**types::Err**](structtypes_1_1Err.md)&lt; E &gt; err) <br> |
|  void | [**destroy**](#function-destroy-12) ([**ok\_tag**](structdetails_1_1ok__tag.md)) <br> |
|  void | [**destroy**](#function-destroy-22) ([**err\_tag**](structdetails_1_1err__tag.md)) <br> |
|  const U & | [**get**](#function-get-12) () const<br> |
|  U & | [**get**](#function-get-22) () <br> |
|  void | [**rawConstruct**](#function-rawconstruct) (U && val) <br> |




























## Public Types Documentation




### typedef type 

```C++
typedef std::aligned_storage<sizeof(E),alignof(E)>::type details::Storage< void, E >::type;
```




<hr>
## Public Attributes Documentation




### variable initialized\_ 

```C++
bool details::Storage< void, E >::initialized_;
```




<hr>



### variable storage\_ 

```C++
type details::Storage< void, E >::storage_;
```




<hr>
## Public Functions Documentation




### function construct [1/2]

```C++
inline void details::Storage< void, E >::construct (
    types::Ok < void >
) 
```




<hr>



### function construct [2/2]

```C++
inline void details::Storage< void, E >::construct (
    types::Err < E > err
) 
```




<hr>



### function destroy [1/2]

```C++
inline void details::Storage< void, E >::destroy (
    ok_tag
) 
```




<hr>



### function destroy [2/2]

```C++
inline void details::Storage< void, E >::destroy (
    err_tag
) 
```




<hr>



### function get [1/2]

```C++
template<typename U>
inline const U & details::Storage< void, E >::get () const
```




<hr>



### function get [2/2]

```C++
template<typename U>
inline U & details::Storage< void, E >::get () 
```




<hr>



### function rawConstruct 

```C++
template<typename U>
inline void details::Storage< void, E >::rawConstruct (
    U && val
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/qqmusic/details/result.h`

