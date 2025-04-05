

# File qimei.cc



[**FileList**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**utils**](dir_313caf1132e152dd9b58bea13a4052ca.md) **>** [**qimei.cc**](qimei_8cc.md)

[Go to the source code of this file](qimei_8cc_source.md)



* `#include <boost/beast.hpp>`
* `#include <boost/beast/http/impl/read.hpp>`
* `#include <boost/beast/http/impl/verb.ipp>`
* `#include <boost/beast/http/message.hpp>`
* `#include <boost/beast/http/string_body.hpp>`
* `#include <boost/beast/http/verb.hpp>`
* `#include <boost/url.hpp>`
* `#include <boost/uuid/detail/md5.hpp>`
* `#include <botan/asn1_obj.h>`
* `#include <botan/auto_rng.h>`
* `#include <botan/base64.h>`
* `#include <botan/block_cipher.h>`
* `#include <botan/cipher_mode.h>`
* `#include <botan/data_src.h>`
* `#include <botan/hex.h>`
* `#include <botan/pem.h>`
* `#include <botan/pubkey.h>`
* `#include <botan/rsa.h>`
* `#include <botan/secmem.h>`
* `#include <botan/x509_key.h>`
* `#include <chrono>`
* `#include <ctime>`
* `#include <format>`
* `#include <nlohmann/json.hpp>`
* `#include <qqmusic/result.h>`
* `#include <qqmusic/utils/buffer.h>`
* `#include <qqmusic/utils/device.h>`
* `#include <qqmusic/utils/qimei.h>`
* `#include <set>`
* `#include <string>`
* `#include <vector>`























## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const char | [**APP\_KEY**](#variable-app_key)   = `"0AND0HD6FE4HY80F"`<br> |
|  const char | [**PUBLIC\_KEY**](#variable-public_key)   = `/* multi line expression */`<br> |
|  const char | [**SECRET**](#variable-secret)   = `"ZdJqM15EeO2zWc08"`<br> |
















## Public Static Functions

| Type | Name |
| ---: | :--- |
|  qqmusic::Result&lt; [**qqmusic::utils::buffer**](classqqmusic_1_1utils_1_1buffer.md) &gt; | [**aes\_encrypt**](#function-aes_encrypt) ([**qqmusic::utils::buffer**](classqqmusic_1_1utils_1_1buffer.md) & key, [**qqmusic::utils::buffer**](classqqmusic_1_1utils_1_1buffer.md) & buf) <br> |
|  nlohmann::json | [**load\_rand\_payload**](#function-load_rand_payload) ([**qqmusic::utils::Device**](structqqmusic_1_1utils_1_1Device.md) & device, std::string\_view version) <br> |
|  std::string | [**random\_beacon\_id**](#function-random_beacon_id) () <br> |
|  qqmusic::Result&lt; [**qqmusic::utils::buffer**](classqqmusic_1_1utils_1_1buffer.md) &gt; | [**rsa\_encrypt**](#function-rsa_encrypt) ([**qqmusic::utils::buffer**](classqqmusic_1_1utils_1_1buffer.md) & buf) <br> |


























## Public Static Attributes Documentation




### variable APP\_KEY 

```C++
const char APP_KEY[];
```




<hr>



### variable PUBLIC\_KEY 

```C++
const char PUBLIC_KEY[];
```




<hr>



### variable SECRET 

```C++
const char SECRET[];
```




<hr>
## Public Static Functions Documentation




### function aes\_encrypt 

```C++
static qqmusic::Result< qqmusic::utils::buffer > aes_encrypt (
    qqmusic::utils::buffer & key,
    qqmusic::utils::buffer & buf
) 
```




<hr>



### function load\_rand\_payload 

```C++
static nlohmann::json load_rand_payload (
    qqmusic::utils::Device & device,
    std::string_view version
) 
```




<hr>



### function random\_beacon\_id 

```C++
static std::string random_beacon_id () 
```




<hr>



### function rsa\_encrypt 

```C++
static qqmusic::Result< qqmusic::utils::buffer > rsa_encrypt (
    qqmusic::utils::buffer & buf
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/utils/qimei.cc`

