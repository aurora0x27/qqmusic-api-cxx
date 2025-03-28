

# Class qqmusic::utils::SessionManager



[**ClassList**](annotated.md) **>** [**qqmusic**](namespaceqqmusic.md) **>** [**utils**](namespaceqqmusic_1_1utils.md) **>** [**SessionManager**](classqqmusic_1_1utils_1_1SessionManager.md)



_全局单例的Session管理器, 负责维护全局上下文栈, 并负责_ `session` _实例的生成_

* `#include <session.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**SessionManager**](#function-sessionmanager-12) (const [**SessionManager**](classqqmusic_1_1utils_1_1SessionManager.md) &) = delete<br> |
|  [**Session**](classqqmusic_1_1utils_1_1Session.md) | [**get\_session**](#function-get_session) () <br>_获取一个_ `session` _实例_ |
|  [**SessionManager**](classqqmusic_1_1utils_1_1SessionManager.md) | [**operator=**](#function-operator) (const [**SessionManager**](classqqmusic_1_1utils_1_1SessionManager.md) &) = delete<br> |
|  void | [**pop\_context**](#function-pop_context) () <br>_上下文栈弹出栈顶元素_  |
|  void | [**push\_context**](#function-push_context) ([**qqmusic::details::NetworkContext**](structqqmusic_1_1details_1_1NetworkContext.md) && context) <br>_将指定的上下文压栈_  |
|  void | [**set\_context**](#function-set_context) (const [**qqmusic::details::NetworkContext**](structqqmusic_1_1details_1_1NetworkContext.md) & context) <br>_替换上下文栈栈顶的上下文_  |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**SessionManager**](classqqmusic_1_1utils_1_1SessionManager.md) & | [**get\_instance**](#function-get_instance) () <br>_获取Session管理器实例_  |


























## Public Functions Documentation




### function SessionManager [1/2]

```C++
qqmusic::utils::SessionManager::SessionManager (
    const SessionManager &
) = delete
```




<hr>



### function get\_session 

_获取一个_ `session` _实例_
```C++
Session qqmusic::utils::SessionManager::get_session () 
```




<hr>



### function operator= 

```C++
SessionManager qqmusic::utils::SessionManager::operator= (
    const SessionManager &
) = delete
```




<hr>



### function pop\_context 

_上下文栈弹出栈顶元素_ 
```C++
void qqmusic::utils::SessionManager::pop_context () 
```




<hr>



### function push\_context 

_将指定的上下文压栈_ 
```C++
void qqmusic::utils::SessionManager::push_context (
    qqmusic::details::NetworkContext && context
) 
```




<hr>



### function set\_context 

_替换上下文栈栈顶的上下文_ 
```C++
void qqmusic::utils::SessionManager::set_context (
    const qqmusic::details::NetworkContext & context
) 
```




<hr>
## Public Static Functions Documentation




### function get\_instance 

_获取Session管理器实例_ 
```C++
static SessionManager & qqmusic::utils::SessionManager::get_instance () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/qqmusic/utils/session.h`

