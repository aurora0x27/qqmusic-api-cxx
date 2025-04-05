

# Class qqmusic::utils::SessionGuard



[**ClassList**](annotated.md) **>** [**qqmusic**](namespaceqqmusic.md) **>** [**utils**](namespaceqqmusic_1_1utils.md) **>** [**SessionGuard**](classqqmusic_1_1utils_1_1SessionGuard.md)



_一个_ `RAII` _风格的Session切换器_

* `#include <session.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**SessionGuard**](#function-sessionguard-12) ([**SessionManager**](classqqmusic_1_1utils_1_1SessionManager.md) & sm, [**Session**](classqqmusic_1_1utils_1_1Session.md) s) <br> |
|   | [**SessionGuard**](#function-sessionguard-22) ([**SessionManager**](classqqmusic_1_1utils_1_1SessionManager.md) & sm, [**details::NetworkContext**](structqqmusic_1_1details_1_1NetworkContext.md) ctx) <br> |
|   | [**~SessionGuard**](#function-sessionguard) () <br> |




























## Public Functions Documentation




### function SessionGuard [1/2]

```C++
inline qqmusic::utils::SessionGuard::SessionGuard (
    SessionManager & sm,
    Session s
) 
```




<hr>



### function SessionGuard [2/2]

```C++
inline qqmusic::utils::SessionGuard::SessionGuard (
    SessionManager & sm,
    details::NetworkContext ctx
) 
```




<hr>



### function ~SessionGuard 

```C++
inline qqmusic::utils::SessionGuard::~SessionGuard () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/qqmusic/utils/session.h`

