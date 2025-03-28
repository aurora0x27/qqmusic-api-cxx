

# Struct details::Constructor

**template &lt;typename T, typename E&gt;**



[**ClassList**](annotated.md) **>** [**details**](namespacedetails.md) **>** [**Constructor**](structdetails_1_1Constructor.md)












































## Public Static Functions

| Type | Name |
| ---: | :--- |
|  void | [**copy**](#function-copy-12) (const [**Storage**](structdetails_1_1Storage.md)&lt; T, E &gt; & src, [**Storage**](structdetails_1_1Storage.md)&lt; T, E &gt; & dst, [**ok\_tag**](structdetails_1_1ok__tag.md)) <br> |
|  void | [**copy**](#function-copy-22) (const [**Storage**](structdetails_1_1Storage.md)&lt; T, E &gt; & src, [**Storage**](structdetails_1_1Storage.md)&lt; T, E &gt; & dst, [**err\_tag**](structdetails_1_1err__tag.md)) <br> |
|  void | [**move**](#function-move-12) ([**Storage**](structdetails_1_1Storage.md)&lt; T, E &gt; && src, [**Storage**](structdetails_1_1Storage.md)&lt; T, E &gt; & dst, [**ok\_tag**](structdetails_1_1ok__tag.md)) <br> |
|  void | [**move**](#function-move-22) ([**Storage**](structdetails_1_1Storage.md)&lt; T, E &gt; && src, [**Storage**](structdetails_1_1Storage.md)&lt; T, E &gt; & dst, [**err\_tag**](structdetails_1_1err__tag.md)) <br> |


























## Public Static Functions Documentation




### function copy [1/2]

```C++
static inline void details::Constructor::copy (
    const Storage < T, E > & src,
    Storage < T, E > & dst,
    ok_tag
) 
```




<hr>



### function copy [2/2]

```C++
static inline void details::Constructor::copy (
    const Storage < T, E > & src,
    Storage < T, E > & dst,
    err_tag
) 
```




<hr>



### function move [1/2]

```C++
static inline void details::Constructor::move (
    Storage < T, E > && src,
    Storage < T, E > & dst,
    ok_tag
) 
```




<hr>



### function move [2/2]

```C++
static inline void details::Constructor::move (
    Storage < T, E > && src,
    Storage < T, E > & dst,
    err_tag
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/qqmusic/details/result.h`

