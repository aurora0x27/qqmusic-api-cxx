

# Struct details::Storage

**template &lt;typename T, typename E&gt;**



[**ClassList**](annotated.md) **>** [**details**](namespacedetails.md) **>** [**Storage**](structdetails_1_1Storage.md)






















## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::aligned\_storage&lt; Size, Align &gt;::type | [**type**](#typedef-type)  <br> |




## Public Attributes

| Type | Name |
| ---: | :--- |
|  bool | [**initialized\_**](#variable-initialized_)  <br> |
|  type | [**storage\_**](#variable-storage_)  <br> |


## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  constexpr size\_t | [**Align**](#variable-align)   = `sizeof(T) &gt; sizeof(E) ? alignof(T) : alignof(E)`<br> |
|  constexpr size\_t | [**Size**](#variable-size)   = `sizeof(T) &gt; sizeof(E) ? sizeof(T) : sizeof(E)`<br> |














## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Storage**](#function-storage) () <br> |
|  void | [**construct**](#function-construct-12) ([**types::Ok**](structtypes_1_1Ok.md)&lt; T &gt; ok) <br> |
|  void | [**construct**](#function-construct-22) ([**types::Err**](structtypes_1_1Err.md)&lt; E &gt; err) <br> |
|  void | [**destroy**](#function-destroy-12) ([**ok\_tag**](structdetails_1_1ok__tag.md)) <br> |
|  void | [**destroy**](#function-destroy-22) ([**err\_tag**](structdetails_1_1err__tag.md)) <br> |
|  const U & | [**get**](#function-get-12) () const<br> |
|  U & | [**get**](#function-get-22) () <br> |
|  void | [**rawConstruct**](#function-rawconstruct) (U && val) <br> |




























## Public Types Documentation




### typedef type 

```C++
typedef std::aligned_storage<Size,Align>::type details::Storage< T, E >::type;
```




<hr>
## Public Attributes Documentation




### variable initialized\_ 

```C++
bool details::Storage< T, E >::initialized_;
```




<hr>



### variable storage\_ 

```C++
type details::Storage< T, E >::storage_;
```




<hr>
## Public Static Attributes Documentation




### variable Align 

```C++
constexpr size_t details::Storage< T, E >::Align;
```




<hr>



### variable Size 

```C++
constexpr size_t details::Storage< T, E >::Size;
```




<hr>
## Public Functions Documentation




### function Storage 

```C++
inline details::Storage::Storage () 
```




<hr>



### function construct [1/2]

```C++
inline void details::Storage::construct (
    types::Ok < T > ok
) 
```




<hr>



### function construct [2/2]

```C++
inline void details::Storage::construct (
    types::Err < E > err
) 
```




<hr>



### function destroy [1/2]

```C++
inline void details::Storage::destroy (
    ok_tag
) 
```




<hr>



### function destroy [2/2]

```C++
inline void details::Storage::destroy (
    err_tag
) 
```




<hr>



### function get [1/2]

```C++
template<typename U>
inline const U & details::Storage::get () const
```




<hr>



### function get [2/2]

```C++
template<typename U>
inline U & details::Storage::get () 
```




<hr>



### function rawConstruct 

```C++
template<typename U>
inline void details::Storage::rawConstruct (
    U && val
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/qqmusic/details/result.h`

