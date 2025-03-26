---
title: Common
---

一组工具函数

::: doxy.dictionary.namespace.function
    namespace: qqmusic::utils
    name: resp2buf

::: doxy.dictionary.namespace.function
    namespace: qqmusic::utils
    name: std::string sign(const nlohmann::json &params)

::: doxy.dictionary.namespace.function
    namespace: qqmusic::utils
    name: hex2buf

### ***enum qrc_type***

- `cloud`

- `local`

::: doxy.dictionary.namespace.function
    namespace: qqmusic::utils
    name: qrc_decode

!!! note 关于选项

    `cloud`和`local`的区别: `cloud`直接处理, 而`local`需要进行qmc1_decryt之后
    去掉前11字节再处理

::: doxy.dictionary.namespace.function
    namespace: qqmusic::utils
    name: hash33

::: doxy.dictionary.namespace.function
    namespace: qqmusic::utils
    name: get_search_id
