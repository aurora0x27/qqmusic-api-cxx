

# Namespace details



[**Namespace List**](namespaces.md) **>** [**details**](namespacedetails.md)


















## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**And**](namespacedetails_1_1And.md) <br> |
| namespace | [**Or**](namespacedetails_1_1Or.md) <br> |
| namespace | [**Other**](namespacedetails_1_1Other.md) <br> |
| namespace | [**err**](namespacedetails_1_1err.md) <br> |
| namespace | [**impl**](namespacedetails_1_1impl.md) <br> |
| namespace | [**ok**](namespacedetails_1_1ok.md) <br> |


## Classes

| Type | Name |
| ---: | :--- |
| struct | [**Constructor**](structdetails_1_1Constructor.md) &lt;typename T, typename E&gt;<br> |
| struct | [**Constructor&lt; void, E &gt;**](structdetails_1_1Constructor_3_01void_00_01E_01_4.md) &lt;typename E&gt;<br> |
| struct | [**IsResult**](structdetails_1_1IsResult.md) &lt;typename R&gt;<br> |
| struct | [**IsResult&lt; Result&lt; T, E &gt; &gt;**](structdetails_1_1IsResult_3_01Result_3_01T_00_01E_01_4_01_4.md) &lt;typename T, typename E&gt;<br> |
| struct | [**ResultErrType**](structdetails_1_1ResultErrType.md) &lt;typename R&gt;<br> |
| struct | [**ResultErrType&lt; Result&lt; T, E &gt; &gt;**](structdetails_1_1ResultErrType_3_01Result_3_01T_00_01E_01_4_01_4.md) &lt;typename T, typename E&gt;<br> |
| struct | [**ResultOkType**](structdetails_1_1ResultOkType.md) &lt;typename R&gt;<br> |
| struct | [**ResultOkType&lt; Result&lt; T, E &gt; &gt;**](structdetails_1_1ResultOkType_3_01Result_3_01T_00_01E_01_4_01_4.md) &lt;typename T, typename E&gt;<br> |
| struct | [**Storage**](structdetails_1_1Storage.md) &lt;typename T, typename E&gt;<br> |
| struct | [**Storage&lt; void, E &gt;**](structdetails_1_1Storage_3_01void_00_01E_01_4.md) &lt;typename E&gt;<br> |
| struct | [**err\_tag**](structdetails_1_1err__tag.md) <br> |
| struct | [**ok\_tag**](structdetails_1_1ok__tag.md) <br> |
| struct | [**result\_of**](structdetails_1_1result__of.md) &lt;typename Func&gt;<br> |
| struct | [**result\_of&lt; Ret(\*)(Args...)&gt;**](structdetails_1_1result__of_3_01Ret_07_5_08_07Args_8_8_8_08_4.md) &lt;typename Ret, Args&gt;<br> |
| struct | [**result\_of&lt; Ret(Cls::\*)(Args...) const &gt;**](structdetails_1_1result__of_3_01Ret_07Cls_1_1_5_08_07Args_8_8_8_08_01const_01_4.md) &lt;typename Ret, typename Cls, Args&gt;<br> |
| struct | [**void\_t**](structdetails_1_1void__t.md) &lt;typename...&gt;<br> |






















## Public Functions

| Type | Name |
| ---: | :--- |
|  Ret | [**map**](#function-map) (const [**Result**](structResult.md)&lt; T, E &gt; & result, Func func) <br> |
|  Ret | [**mapError**](#function-maperror) (const [**Result**](structResult.md)&lt; T, E &gt; & result, Func func) <br> |
|  Ret | [**orElse**](#function-orelse) (const [**Result**](structResult.md)&lt; T, E &gt; & result, Func func) <br> |
|  [**Result**](structResult.md)&lt; T, E &gt; | [**otherwise**](#function-otherwise) (const [**Result**](structResult.md)&lt; T, E &gt; & result, Func func) <br> |
|  [**Result**](structResult.md)&lt; T, E &gt; | [**then**](#function-then) (const [**Result**](structResult.md)&lt; T, E &gt; & result, Func func) <br> |




























## Public Functions Documentation




### function map 

```C++
template<typename T, typename E, typename Func, typename Ret>
Ret details::map (
    const Result < T, E > & result,
    Func func
) 
```




<hr>



### function mapError 

```C++
template<typename T, typename E, typename Func, typename Ret>
Ret details::mapError (
    const Result < T, E > & result,
    Func func
) 
```




<hr>



### function orElse 

```C++
template<typename T, typename E, typename Func, typename Ret>
Ret details::orElse (
    const Result < T, E > & result,
    Func func
) 
```




<hr>



### function otherwise 

```C++
template<typename T, typename E, typename Func>
Result < T, E > details::otherwise (
    const Result < T, E > & result,
    Func func
) 
```




<hr>



### function then 

```C++
template<typename T, typename E, typename Func>
Result < T, E > details::then (
    const Result < T, E > & result,
    Func func
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/qqmusic/details/result.h`

