

# File details/result.h



[**FileList**](files.md) **>** [**details**](dir_6e6556f41d7f85330fbda245682a6a39.md) **>** [**result.h**](details_2result_8h.md)

[Go to the source code of this file](details_2result_8h_source.md)



* `#include <functional>`
* `#include <iostream>`
* `#include <type_traits>`













## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**concepts**](namespaceconcepts.md) <br> |
| namespace | [**details**](namespacedetails.md) <br> |
| namespace | [**And**](namespacedetails_1_1And.md) <br> |
| namespace | [**impl**](namespacedetails_1_1And_1_1impl.md) <br> |
| namespace | [**Or**](namespacedetails_1_1Or.md) <br> |
| namespace | [**impl**](namespacedetails_1_1Or_1_1impl.md) <br> |
| namespace | [**Other**](namespacedetails_1_1Other.md) <br> |
| namespace | [**impl**](namespacedetails_1_1Other_1_1impl.md) <br> |
| namespace | [**err**](namespacedetails_1_1err.md) <br> |
| namespace | [**impl**](namespacedetails_1_1err_1_1impl.md) <br> |
| namespace | [**impl**](namespacedetails_1_1impl.md) <br> |
| namespace | [**ok**](namespacedetails_1_1ok.md) <br> |
| namespace | [**impl**](namespacedetails_1_1ok_1_1impl.md) <br> |
| namespace | [**types**](namespacetypes.md) <br> |


## Classes

| Type | Name |
| ---: | :--- |
| struct | [**Result**](structResult.md) &lt;typename T, typename E&gt;<br> |
| struct | [**EqualityComparable**](structconcepts_1_1EqualityComparable.md) &lt;typename T, typename&gt;<br> |
| struct | [**EqualityComparable&lt; T, typename std::enable\_if&lt; true, typename details::void\_t&lt; decltype(std::declval&lt; T &gt;()==std::declval&lt; T &gt;())&gt;::type &gt;::type &gt;**](structconcepts_1_1EqualityComparable_3_01T_00_01typename_01std_1_1enable__if_3_01true_00_01typenb4b0200ec94bdc3bb35c4a1595f66806.md) &lt;typename T&gt;<br> |
| struct | [**Then**](structdetails_1_1And_1_1Then.md) &lt;typename Func&gt;<br> |
| struct | [**Then&lt; Ret(\*)(Args...)&gt;**](structdetails_1_1And_1_1Then_3_01Ret_07_5_08_07Args_8_8_8_08_4.md) &lt;typename Ret, Args&gt;<br> |
| struct | [**Then&lt; Ret(Cls::\*)(Args...) const &gt;**](structdetails_1_1And_1_1Then_3_01Ret_07Cls_1_1_5_08_07Args_8_8_8_08_01const_01_4.md) &lt;typename Ret, typename Cls, Args&gt;<br> |
| struct | [**Then&lt; Ret(Cls::\*)(Args...)&gt;**](structdetails_1_1And_1_1Then_3_01Ret_07Cls_1_1_5_08_07Args_8_8_8_08_4.md) &lt;typename Ret, typename Cls, Args&gt;<br> |
| struct | [**Then&lt; Ret(\*)(Args...)&gt;**](structdetails_1_1And_1_1impl_1_1Then_3_01Ret_07_5_08_07Args_8_8_8_08_4.md) &lt;typename Ret, Args&gt;<br> |
| struct | [**Then&lt; Ret(Arg)&gt;**](structdetails_1_1And_1_1impl_1_1Then_3_01Ret_07Arg_08_4.md) &lt;typename Ret, typename Arg&gt;<br> |
| struct | [**Then&lt; Ret(Cls::\*)(Args...) const &gt;**](structdetails_1_1And_1_1impl_1_1Then_3_01Ret_07Cls_1_1_5_08_07Args_8_8_8_08_01const_01_4.md) &lt;typename Ret, typename Cls, Args&gt;<br> |
| struct | [**Then&lt; Ret(Cls::\*)(Args...)&gt;**](structdetails_1_1And_1_1impl_1_1Then_3_01Ret_07Cls_1_1_5_08_07Args_8_8_8_08_4.md) &lt;typename Ret, typename Cls, Args&gt;<br> |
| struct | [**Then&lt; Ret(void)&gt;**](structdetails_1_1And_1_1impl_1_1Then_3_01Ret_07void_08_4.md) &lt;typename Ret&gt;<br> |
| struct | [**Constructor**](structdetails_1_1Constructor.md) &lt;typename T, typename E&gt;<br> |
| struct | [**Constructor&lt; void, E &gt;**](structdetails_1_1Constructor_3_01void_00_01E_01_4.md) &lt;typename E&gt;<br> |
| struct | [**IsResult**](structdetails_1_1IsResult.md) &lt;typename R&gt;<br> |
| struct | [**IsResult&lt; Result&lt; T, E &gt; &gt;**](structdetails_1_1IsResult_3_01Result_3_01T_00_01E_01_4_01_4.md) &lt;typename T, typename E&gt;<br> |
| struct | [**Else**](structdetails_1_1Or_1_1Else.md) &lt;typename Func&gt;<br> |
| struct | [**Else&lt; Ret(\*)(Args...)&gt;**](structdetails_1_1Or_1_1Else_3_01Ret_07_5_08_07Args_8_8_8_08_4.md) &lt;typename Ret, Args&gt;<br> |
| struct | [**Else&lt; Ret(Cls::\*)(Args...) const &gt;**](structdetails_1_1Or_1_1Else_3_01Ret_07Cls_1_1_5_08_07Args_8_8_8_08_01const_01_4.md) &lt;typename Ret, typename Cls, Args&gt;<br> |
| struct | [**Else&lt; Ret(Cls::\*)(Args...)&gt;**](structdetails_1_1Or_1_1Else_3_01Ret_07Cls_1_1_5_08_07Args_8_8_8_08_4.md) &lt;typename Ret, typename Cls, Args&gt;<br> |
| struct | [**Else&lt; Result&lt; T, F &gt;(Arg)&gt;**](structdetails_1_1Or_1_1impl_1_1Else_3_01Result_3_01T_00_01F_01_4_07Arg_08_4.md) &lt;typename T, typename F, typename Arg&gt;<br> |
| struct | [**Else&lt; Result&lt; T, F &gt;(void)&gt;**](structdetails_1_1Or_1_1impl_1_1Else_3_01Result_3_01T_00_01F_01_4_07void_08_4.md) &lt;typename T, typename F&gt;<br> |
| struct | [**Else&lt; Ret(\*)(Args...)&gt;**](structdetails_1_1Or_1_1impl_1_1Else_3_01Ret_07_5_08_07Args_8_8_8_08_4.md) &lt;typename Ret, Args&gt;<br> |
| struct | [**Else&lt; Ret(Cls::\*)(Args...) const &gt;**](structdetails_1_1Or_1_1impl_1_1Else_3_01Ret_07Cls_1_1_5_08_07Args_8_8_8_08_01const_01_4.md) &lt;typename Ret, typename Cls, Args&gt;<br> |
| struct | [**Else&lt; Ret(Cls::\*)(Args...)&gt;**](structdetails_1_1Or_1_1impl_1_1Else_3_01Ret_07Cls_1_1_5_08_07Args_8_8_8_08_4.md) &lt;typename Ret, typename Cls, Args&gt;<br> |
| struct | [**Wise**](structdetails_1_1Other_1_1Wise.md) &lt;typename Func&gt;<br> |
| struct | [**Wise&lt; Ret(\*)(Args...)&gt;**](structdetails_1_1Other_1_1Wise_3_01Ret_07_5_08_07Args_8_8_8_08_4.md) &lt;typename Ret, Args&gt;<br> |
| struct | [**Wise&lt; Ret(Cls::\*)(Args...) const &gt;**](structdetails_1_1Other_1_1Wise_3_01Ret_07Cls_1_1_5_08_07Args_8_8_8_08_01const_01_4.md) &lt;typename Ret, typename Cls, Args&gt;<br> |
| struct | [**Wise&lt; Ret(Cls::\*)(Args...)&gt;**](structdetails_1_1Other_1_1Wise_3_01Ret_07Cls_1_1_5_08_07Args_8_8_8_08_4.md) &lt;typename Ret, typename Cls, Args&gt;<br> |
| struct | [**Wise&lt; Ret(\*)(Args...)&gt;**](structdetails_1_1Other_1_1impl_1_1Wise_3_01Ret_07_5_08_07Args_8_8_8_08_4.md) &lt;typename Ret, Args&gt;<br> |
| struct | [**Wise&lt; Ret(Arg)&gt;**](structdetails_1_1Other_1_1impl_1_1Wise_3_01Ret_07Arg_08_4.md) &lt;typename Ret, typename Arg&gt;<br> |
| struct | [**Wise&lt; Ret(Cls::\*)(Args...) const &gt;**](structdetails_1_1Other_1_1impl_1_1Wise_3_01Ret_07Cls_1_1_5_08_07Args_8_8_8_08_01const_01_4.md) &lt;typename Ret, typename Cls, Args&gt;<br> |
| struct | [**Wise&lt; Ret(Cls::\*)(Args...)&gt;**](structdetails_1_1Other_1_1impl_1_1Wise_3_01Ret_07Cls_1_1_5_08_07Args_8_8_8_08_4.md) &lt;typename Ret, typename Cls, Args&gt;<br> |
| struct | [**ResultErrType**](structdetails_1_1ResultErrType.md) &lt;typename R&gt;<br> |
| struct | [**ResultErrType&lt; Result&lt; T, E &gt; &gt;**](structdetails_1_1ResultErrType_3_01Result_3_01T_00_01E_01_4_01_4.md) &lt;typename T, typename E&gt;<br> |
| struct | [**ResultOkType**](structdetails_1_1ResultOkType.md) &lt;typename R&gt;<br> |
| struct | [**ResultOkType&lt; Result&lt; T, E &gt; &gt;**](structdetails_1_1ResultOkType_3_01Result_3_01T_00_01E_01_4_01_4.md) &lt;typename T, typename E&gt;<br> |
| struct | [**Storage**](structdetails_1_1Storage.md) &lt;typename T, typename E&gt;<br> |
| struct | [**Storage&lt; void, E &gt;**](structdetails_1_1Storage_3_01void_00_01E_01_4.md) &lt;typename E&gt;<br> |
| struct | [**Map**](structdetails_1_1err_1_1Map.md) &lt;typename Func&gt;<br> |
| struct | [**Map&lt; Ret(Cls::\*)(Arg) const &gt;**](structdetails_1_1err_1_1impl_1_1Map_3_01Ret_07Cls_1_1_5_08_07Arg_08_01const_01_4.md) &lt;typename Ret, typename Cls, typename Arg&gt;<br> |
| struct | [**err\_tag**](structdetails_1_1err__tag.md) <br> |
| struct | [**result\_of&lt; Ret(Args...)&gt;**](structdetails_1_1impl_1_1result__of_3_01Ret_07Args_8_8_8_08_4.md) &lt;typename Ret, Args&gt;<br> |
| struct | [**result\_of&lt; Ret(Cls::\*)(Args...)&gt;**](structdetails_1_1impl_1_1result__of_3_01Ret_07Cls_1_1_5_08_07Args_8_8_8_08_4.md) &lt;typename Ret, typename Cls, Args&gt;<br> |
| struct | [**Map**](structdetails_1_1ok_1_1Map.md) &lt;typename Func&gt;<br> |
| struct | [**Map&lt; Ret(\*)(Args...)&gt;**](structdetails_1_1ok_1_1Map_3_01Ret_07_5_08_07Args_8_8_8_08_4.md) &lt;typename Ret, Args&gt;<br> |
| struct | [**Map&lt; Ret(Cls::\*)(Args...) const &gt;**](structdetails_1_1ok_1_1Map_3_01Ret_07Cls_1_1_5_08_07Args_8_8_8_08_01const_01_4.md) &lt;typename Ret, typename Cls, Args&gt;<br> |
| struct | [**Map&lt; Ret(Cls::\*)(Args...)&gt;**](structdetails_1_1ok_1_1Map_3_01Ret_07Cls_1_1_5_08_07Args_8_8_8_08_4.md) &lt;typename Ret, typename Cls, Args&gt;<br> |
| struct | [**Map&lt; std::function&lt; Ret(Args...)&gt; &gt;**](structdetails_1_1ok_1_1Map_3_01std_1_1function_3_01Ret_07Args_8_8_8_08_4_01_4.md) &lt;typename Ret, Args&gt;<br> |
| struct | [**Map&lt; Result&lt; U, E &gt;(Arg)&gt;**](structdetails_1_1ok_1_1impl_1_1Map_3_01Result_3_01U_00_01E_01_4_07Arg_08_4.md) &lt;typename U, typename E, typename Arg&gt;<br> |
| struct | [**Map&lt; Result&lt; U, E &gt;(void)&gt;**](structdetails_1_1ok_1_1impl_1_1Map_3_01Result_3_01U_00_01E_01_4_07void_08_4.md) &lt;typename U, typename E&gt;<br> |
| struct | [**Map&lt; Ret(Arg)&gt;**](structdetails_1_1ok_1_1impl_1_1Map_3_01Ret_07Arg_08_4.md) &lt;typename Ret, typename Arg&gt;<br> |
| struct | [**Map&lt; Ret(Cls::\*)(Arg) const &gt;**](structdetails_1_1ok_1_1impl_1_1Map_3_01Ret_07Cls_1_1_5_08_07Arg_08_01const_01_4.md) &lt;typename Ret, typename Cls, typename Arg&gt;<br> |
| struct | [**Map&lt; Ret(Cls::\*)(Arg)&gt;**](structdetails_1_1ok_1_1impl_1_1Map_3_01Ret_07Cls_1_1_5_08_07Arg_08_4.md) &lt;typename Ret, typename Cls, typename Arg&gt;<br> |
| struct | [**Map&lt; Ret(void)&gt;**](structdetails_1_1ok_1_1impl_1_1Map_3_01Ret_07void_08_4.md) &lt;typename Ret&gt;<br> |
| struct | [**Map&lt; void(Arg)&gt;**](structdetails_1_1ok_1_1impl_1_1Map_3_01void_07Arg_08_4.md) &lt;typename Arg&gt;<br> |
| struct | [**Map&lt; void(void)&gt;**](structdetails_1_1ok_1_1impl_1_1Map_3_01void_07void_08_4.md) &lt;&gt;<br> |
| struct | [**ok\_tag**](structdetails_1_1ok__tag.md) <br> |
| struct | [**result\_of**](structdetails_1_1result__of.md) &lt;typename Func&gt;<br> |
| struct | [**result\_of&lt; Ret(\*)(Args...)&gt;**](structdetails_1_1result__of_3_01Ret_07_5_08_07Args_8_8_8_08_4.md) &lt;typename Ret, Args&gt;<br> |
| struct | [**result\_of&lt; Ret(Cls::\*)(Args...) const &gt;**](structdetails_1_1result__of_3_01Ret_07Cls_1_1_5_08_07Args_8_8_8_08_01const_01_4.md) &lt;typename Ret, typename Cls, Args&gt;<br> |
| struct | [**void\_t**](structdetails_1_1void__t.md) &lt;typename...&gt;<br> |
| struct | [**Err**](structtypes_1_1Err.md) &lt;typename E&gt;<br> |
| struct | [**Ok**](structtypes_1_1Ok.md) &lt;typename T&gt;<br> |
| struct | [**Ok&lt; void &gt;**](structtypes_1_1Ok_3_01void_01_4.md) &lt;&gt;<br> |






















## Public Functions

| Type | Name |
| ---: | :--- |
|  [**types::Err**](structtypes_1_1Err.md)&lt; CleanE &gt; | [**Err**](#function-err) (E && val) <br> |
|  [**types::Ok**](structtypes_1_1Ok.md)&lt; CleanT &gt; | [**Ok**](#function-ok) (T && val) <br> |
|  [**types::Ok**](structtypes_1_1Ok.md)&lt; void &gt; | [**Ok**](#function-ok) () <br> |
|  bool | [**operator==**](#function-operator) (const [**Result**](structResult.md)&lt; T, E &gt; & lhs, const [**Result**](structResult.md)&lt; T, E &gt; & rhs) <br> |
|  bool | [**operator==**](#function-operator_1) (const [**Result**](structResult.md)&lt; T, E &gt; & lhs, [**types::Ok**](structtypes_1_1Ok.md)&lt; T &gt; ok) <br> |
|  bool | [**operator==**](#function-operator_2) (const [**Result**](structResult.md)&lt; void, E &gt; & lhs, [**types::Ok**](structtypes_1_1Ok.md)&lt; void &gt;) <br> |
|  bool | [**operator==**](#function-operator_3) (const [**Result**](structResult.md)&lt; T, E &gt; & lhs, [**types::Err**](structtypes_1_1Err.md)&lt; E &gt; err) <br> |



























## Macros

| Type | Name |
| ---: | :--- |
| define  | [**TRY**](details_2result_8h.md#define-try) (...) `/* multi line expression */`<br> |

## Public Functions Documentation




### function Err 

```C++
template<typename E, typename CleanE>
types::Err < CleanE > Err (
    E && val
) 
```




<hr>



### function Ok 

```C++
template<typename T, typename CleanT>
types::Ok < CleanT > Ok (
    T && val
) 
```




<hr>



### function Ok 

```C++
inline types::Ok < void > Ok () 
```




<hr>



### function operator== 

```C++
template<typename T, typename E>
bool operator== (
    const Result < T, E > & lhs,
    const Result < T, E > & rhs
) 
```




<hr>



### function operator== 

```C++
template<typename T, typename E>
bool operator== (
    const Result < T, E > & lhs,
    types::Ok < T > ok
) 
```




<hr>



### function operator== 

```C++
template<typename E>
bool operator== (
    const Result < void, E > & lhs,
    types::Ok < void >
) 
```




<hr>



### function operator== 

```C++
template<typename T, typename E>
bool operator== (
    const Result < T, E > & lhs,
    types::Err < E > err
) 
```




<hr>
## Macro Definition Documentation





### define TRY 

```C++
#define TRY (
    ...
) `/* multi line expression */`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/qqmusic/details/result.h`

