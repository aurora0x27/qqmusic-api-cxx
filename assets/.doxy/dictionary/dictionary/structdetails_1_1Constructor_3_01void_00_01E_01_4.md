

# Struct details::Constructor&lt; void, E &gt;

**template &lt;typename E&gt;**



[**ClassList**](annotated.md) **>** [**details**](namespacedetails.md) **>** [**Constructor&lt; void, E &gt;**](structdetails_1_1Constructor_3_01void_00_01E_01_4.md)












































## Public Static Functions

| Type | Name |
| ---: | :--- |
|  void | [**copy**](#function-copy-12) (const [**Storage**](structdetails_1_1Storage.md)&lt; void, E &gt; & src, [**Storage**](structdetails_1_1Storage.md)&lt; void, E &gt; & dst, [**ok\_tag**](structdetails_1_1ok__tag.md)) <br> |
|  void | [**copy**](#function-copy-22) (const [**Storage**](structdetails_1_1Storage.md)&lt; void, E &gt; & src, [**Storage**](structdetails_1_1Storage.md)&lt; void, E &gt; & dst, [**err\_tag**](structdetails_1_1err__tag.md)) <br> |
|  void | [**move**](#function-move-12) ([**Storage**](structdetails_1_1Storage.md)&lt; void, E &gt; && src, [**Storage**](structdetails_1_1Storage.md)&lt; void, E &gt; & dst, [**ok\_tag**](structdetails_1_1ok__tag.md)) <br> |
|  void | [**move**](#function-move-22) ([**Storage**](structdetails_1_1Storage.md)&lt; void, E &gt; && src, [**Storage**](structdetails_1_1Storage.md)&lt; void, E &gt; & dst, [**err\_tag**](structdetails_1_1err__tag.md)) <br> |


























## Public Static Functions Documentation




### function copy [1/2]

```C++
static inline void details::Constructor< void, E >::copy (
    const Storage < void, E > & src,
    Storage < void, E > & dst,
    ok_tag
) 
```




<hr>



### function copy [2/2]

```C++
static inline void details::Constructor< void, E >::copy (
    const Storage < void, E > & src,
    Storage < void, E > & dst,
    err_tag
) 
```




<hr>



### function move [1/2]

```C++
static inline void details::Constructor< void, E >::move (
    Storage < void, E > && src,
    Storage < void, E > & dst,
    ok_tag
) 
```




<hr>



### function move [2/2]

```C++
static inline void details::Constructor< void, E >::move (
    Storage < void, E > && src,
    Storage < void, E > & dst,
    err_tag
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/qqmusic/details/result.h`

