

# Struct details::Or::impl::Else&lt; Result&lt; T, F &gt;(void)&gt;

**template &lt;typename T, typename F&gt;**



[**ClassList**](annotated.md) **>** [**details**](namespacedetails.md) **>** [**Or**](namespacedetails_1_1Or.md) **>** [**impl**](namespacedetails_1_1Or_1_1impl.md) **>** [**Else&lt; Result&lt; T, F &gt;(void)&gt;**](structdetails_1_1Or_1_1impl_1_1Else_3_01Result_3_01T_00_01F_01_4_07void_08_4.md)












































## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**Result**](structResult.md)&lt; T, F &gt; | [**orElse**](#function-orelse-12) (const [**Result**](structResult.md)&lt; T, E &gt; & result, Func func) <br> |
|  [**Result**](structResult.md)&lt; void, F &gt; | [**orElse**](#function-orelse-22) (const [**Result**](structResult.md)&lt; void, E &gt; & result, Func func) <br> |


























## Public Static Functions Documentation




### function orElse [1/2]

```C++
template<typename E, typename Func>
static inline Result < T, F > details::Or::impl::Else< Result< T, F >(void)>::orElse (
    const Result < T, E > & result,
    Func func
) 
```




<hr>



### function orElse [2/2]

```C++
template<typename E, typename Func>
static inline Result < void, F > details::Or::impl::Else< Result< T, F >(void)>::orElse (
    const Result < void, E > & result,
    Func func
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/qqmusic/details/result.h`

