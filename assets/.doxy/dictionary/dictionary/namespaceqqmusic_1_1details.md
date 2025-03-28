

# Namespace qqmusic::details



[**Namespace List**](namespaces.md) **>** [**qqmusic**](namespaceqqmusic.md) **>** [**details**](namespaceqqmusic_1_1details.md)



[More...](#detailed-description)
















## Classes

| Type | Name |
| ---: | :--- |
| class | [**Api**](classqqmusic_1_1details_1_1Api.md) <br>_Api类, 简化请求报文组装和返回报文解析过程_  |
| struct | [**ApiConfig**](structqqmusic_1_1details_1_1ApiConfig.md) <br>_Api相关常量_  |
| struct | [**NetworkContext**](structqqmusic_1_1details_1_1NetworkContext.md) <br>_打包的上下文_  |
| struct | [**RequestParam**](structqqmusic_1_1details_1_1RequestParam.md) <br>_包装请求所需的参数_  |


## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**tripledes\_crypt\_mode**](#enum-tripledes_crypt_mode)  <br> |
| typedef std::vector&lt; std::vector&lt; std::vector&lt; uint32\_t &gt; &gt; &gt; | [**tripledes\_key\_schedule**](#typedef-tripledes_key_schedule)  <br> |




## Public Attributes

| Type | Name |
| ---: | :--- |
|  const char | [**VERSION**](#variable-version)   = `"13.2.5.8"`<br>_Api版本号_  |
|  const uint64\_t | [**VERSION\_CODE**](#variable-version_code)   = `13020508`<br>_版本代码_  |
















## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**tripledes\_crypt**](#function-tripledes_crypt) ([**utils::buffer**](classqqmusic_1_1utils_1_1buffer.md) & buf\_in, [**utils::buffer**](classqqmusic_1_1utils_1_1buffer.md) & buf\_out, tripledes\_key\_schedule key\_schedule) <br> |
|  tripledes\_key\_schedule | [**tripledes\_key\_setup**](#function-tripledes_key_setup) (const uint8\_t \* key\_head, size\_t key\_size, tripledes\_crypt\_mode mode) <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  uint32\_t | [**bitnum**](#function-bitnum) (const uint8\_t \* head, size\_t size, uint32\_t b, uint32\_t c) <br> |
|  uint32\_t | [**bitnum\_intl**](#function-bitnum_intl) (uint32\_t a, uint32\_t b, uint32\_t c) <br> |
|  uint32\_t | [**bitnum\_intr**](#function-bitnum_intr) (uint32\_t a, uint32\_t b, uint32\_t c) <br> |
|  void | [**crypt**](#function-crypt) ([**utils::buffer**](classqqmusic_1_1utils_1_1buffer.md) & buf, std::vector&lt; std::vector&lt; uint32\_t &gt; &gt; key) <br> |
|  void | [**initial\_permutation**](#function-initial_permutation) (uint32\_t \* a, uint32\_t \* b, [**utils::buffer**](classqqmusic_1_1utils_1_1buffer.md) & buf) <br> |
|  void | [**inverse\_permutation**](#function-inverse_permutation) (uint32\_t a, uint32\_t b, [**utils::buffer**](classqqmusic_1_1utils_1_1buffer.md) & buf) <br> |
|  std::vector&lt; std::vector&lt; uint32\_t &gt; &gt; | [**key\_schedule**](#function-key_schedule) (const uint8\_t \* key\_head, size\_t key\_size, tripledes\_crypt\_mode mode) <br> |
|  uint32\_t | [**sbox\_bit**](#function-sbox_bit) (uint32\_t a) <br> |
|  uint32\_t | [**transform**](#function-transform) (uint32\_t state, std::vector&lt; uint32\_t &gt; key) <br> |


























## Detailed Description


----------------------------qqmusic/details/tripledes.h------------------------------------ 


### 3des decrypt functions, for qrc-decoder.




    
## Public Types Documentation




### enum tripledes\_crypt\_mode 

```C++
enum qqmusic::details::tripledes_crypt_mode {
    encrypt,
    decrypt
};
```




<hr>



### typedef tripledes\_key\_schedule 

```C++
using qqmusic::details::tripledes_key_schedule = typedef std::vector<std::vector<std::vector<uint32_t> >>;
```




<hr>
## Public Attributes Documentation




### variable VERSION 

_Api版本号_ 
```C++
const char qqmusic::details::VERSION[];
```




<hr>



### variable VERSION\_CODE 

_版本代码_ 
```C++
const uint64_t qqmusic::details::VERSION_CODE;
```




<hr>
## Public Functions Documentation




### function tripledes\_crypt 

```C++
void qqmusic::details::tripledes_crypt (
    utils::buffer & buf_in,
    utils::buffer & buf_out,
    tripledes_key_schedule key_schedule
) 
```




<hr>



### function tripledes\_key\_setup 

```C++
tripledes_key_schedule qqmusic::details::tripledes_key_setup (
    const uint8_t * key_head,
    size_t key_size,
    tripledes_crypt_mode mode
) 
```




<hr>
## Public Static Functions Documentation




### function bitnum 

```C++
static uint32_t qqmusic::details::bitnum (
    const uint8_t * head,
    size_t size,
    uint32_t b,
    uint32_t c
) 
```




<hr>



### function bitnum\_intl 

```C++
static uint32_t qqmusic::details::bitnum_intl (
    uint32_t a,
    uint32_t b,
    uint32_t c
) 
```




<hr>



### function bitnum\_intr 

```C++
static uint32_t qqmusic::details::bitnum_intr (
    uint32_t a,
    uint32_t b,
    uint32_t c
) 
```




<hr>



### function crypt 

```C++
static void qqmusic::details::crypt (
    utils::buffer & buf,
    std::vector< std::vector< uint32_t > > key
) 
```




<hr>



### function initial\_permutation 

```C++
static void qqmusic::details::initial_permutation (
    uint32_t * a,
    uint32_t * b,
    utils::buffer & buf
) 
```




<hr>



### function inverse\_permutation 

```C++
static void qqmusic::details::inverse_permutation (
    uint32_t a,
    uint32_t b,
    utils::buffer & buf
) 
```




<hr>



### function key\_schedule 

```C++
static std::vector< std::vector< uint32_t > > qqmusic::details::key_schedule (
    const uint8_t * key_head,
    size_t key_size,
    tripledes_crypt_mode mode
) 
```




<hr>



### function sbox\_bit 

```C++
static uint32_t qqmusic::details::sbox_bit (
    uint32_t a
) 
```




<hr>



### function transform 

```C++
static uint32_t qqmusic::details::transform (
    uint32_t state,
    std::vector< uint32_t > key
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/details/api.cc`

