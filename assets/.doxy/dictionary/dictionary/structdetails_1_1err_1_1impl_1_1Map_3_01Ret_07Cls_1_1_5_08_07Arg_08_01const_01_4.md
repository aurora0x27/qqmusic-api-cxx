

# Struct details::err::impl::Map&lt; Ret(Cls::\*)(Arg) const &gt;

**template &lt;typename Ret, typename Cls, typename Arg&gt;**



[**ClassList**](annotated.md) **>** [**details**](namespacedetails.md) **>** [**err**](namespacedetails_1_1err.md) **>** [**impl**](namespacedetails_1_1err_1_1impl.md) **>** [**Map&lt; Ret(Cls::\*)(Arg) const &gt;**](structdetails_1_1err_1_1impl_1_1Map_3_01Ret_07Cls_1_1_5_08_07Arg_08_01const_01_4.md)












































## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**Result**](structResult.md)&lt; T, Ret &gt; | [**map**](#function-map-12) (const [**Result**](structResult.md)&lt; T, E &gt; & result, Func func) <br> |
|  [**Result**](structResult.md)&lt; void, Ret &gt; | [**map**](#function-map-22) (const [**Result**](structResult.md)&lt; void, E &gt; & result, Func func) <br> |


























## Public Static Functions Documentation




### function map [1/2]

```C++
template<typename T, typename E, typename Func>
static inline Result < T, Ret > details::err::impl::Map< Ret(Cls::*)(Arg) const >::map (
    const Result < T, E > & result,
    Func func
) 
```




<hr>



### function map [2/2]

```C++
template<typename E, typename Func>
static inline Result < void, Ret > details::err::impl::Map< Ret(Cls::*)(Arg) const >::map (
    const Result < void, E > & result,
    Func func
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/qqmusic/details/result.h`

