---
title: 项目架构
---

我们通过一个Api函数介绍项目的整体架构

```cpp

// 使用通用异步任务`qqmusic::Task<T>`返回协程task
// 使用通用返回结果`qqmusic::Result<T>`包装nlohmann_json提供的json
Task<Result<nlohmann::json>> get_album_detail(std::string_view album_mid) {

    // 通过SessionManager单例获取session实例
    auto session = utils::SessionManager::get_instance().get_session();

    // 通过session实例化Api类, 同时传入必要的信息
    auto api = details::Api(session, "music.musichallAlbum.AlbumInfoServer", "GetAlbumDetail");

    // 构造json格式的请求数据表单
    nlohmann::json params = {{"albumMId", album_mid}};

    // 构造请求
    auto req_param_res = co_await api.prepare_request(params);
    if (req_param_res.isErr()) {
        // 这里返回的是Result<>, 需要检查request是否构造成功
        co_return Err(utils::Exception(
            utils::Exception::Kind(req_param_res.unwrapErr().get_error_enum()),
            std::format("[get_album_detail] -- Error occurred when preparing request: `{}`",
                        req_param_res.unwrapErr().what())));
    }
    // *这里构造的请求不包含cookie信息, 如果要包含cookie, 请查阅qqmusic::utils::CookieJar的文档
    // 并手动添加cookie
    // `req.set(boost::beast::http::field::cookie, "cookie string");`


    // 发送请求, 获取结果
    auto url = req_param_res.unwrap().url;
    auto req = req_param_res.unwrap().req;
    auto response_res = co_await session.perform_request(url, req);
    if (response_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::NetworkError,
            std::format("[get_album_detail] -- Error occurred when performing request: `{}`",
                        response_res.unwrapErr().what())));
    }

    // 如果结果无误, 就进行解析. 
    // 返回的结果是原始的boost::beast::http::response<boost::beast::http::dynamic_body>
    // 如果是二进制的媒体数据, 可以通过utils::to_buffer函数转换成通用buffer
    // 一般的json格式一般直接返回json对象
    auto response_json_res = api.parse_response(utils::to_buffer(response_res.unwrap()));
    if (response_json_res.isErr()) {
        co_return Err(utils::Exception(
            utils::Exception::DataDestroy,
            std::format("[get_album_detail] -- Error occurred when parsing reponse: `{}`",
                        response_json_res.unwrapErr().what())));
    }
    auto response_json = response_json_res.unwrap();
    co_return Ok(response_json);
}
```

其中`Session`是负责管理请求上下文, 并发起请求的类, 内部有每个实例各自的上下文拷贝和指向全局上下文的引用. 所有的请求都通过`Session::perform_request`函数发送

`Api`类负责构造请求, 省去手动拼接参数的工作. 多数Api都可以自动拼接参数, 部分情况下需要手动构造请求.
