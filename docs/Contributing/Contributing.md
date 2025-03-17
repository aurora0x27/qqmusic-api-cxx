---
title: 贡献指南
---

感谢您对本项目感兴趣, 请遵循以下规范提交贡献

## 预先配置

- 编译器: gcc >= 13, clang >= 19, Visual Studio 2022

- [vcpkg](https://vcpkg.io/en/) 和 CMake >= 3.20, 或 xmake

!!! warning "About Xmake"

    Xmake 目前只能在Linux平台成功编译出可执行文件, Windows和MacOS尚未通过测试

- [pre-commit](https://pre-commit.com/)

- [mkdocs](https://squidfunk.github.io/mkdocs-material/)

## 配置环境

- ***Step1:*** 在github上fork一份代码

- ***Step2:*** 下载并安装pre-commit钩子

```bash
# download your forked repo from github
git clone --recursive git@github.com:<your username>/qqmusic-api-cxx.git
cd qqmusic-api-cxx

# install pre-commit hooks
pre-commit install --all
```

- ***Step3:*** 编译配置

```bash
#                If you want to add a demo program,     You will need this if you
#                    please set this to `Demo`                use clangd
#                                       \                     /
cmake -B build -DQQMUSIC_API_BUILD_TYPE=Test -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug
```

出现类似这样的图形即配置成功:

```
+---------------QQMUSIC API CXX---------------+
|                                             |
|              QQMUSIC BUILD TYPE             |
|                                             |
|    DEMO                           OFF       |
|                                             |
|    SHARED LIBRARY                 OFF       |
|                                             |
|    TEST                           ON        |
|                                             |
+---------------------------------------------+
```

编译

```
cmake --build build --parallel 8
```

!!! note "About xmake"

    推荐使用CMake, 但是目前缺少人手做xmake方面的适配qwq

    ```bash
    $ xmake config -m [debug|release] -y && xmake build [test|demo|qmapi]
    ```

## 关于提交和PR

提交规范请遵循[Conventional Commits](https://www.conventionalcommits.org/zh-hans/v1.0.0/)规范

!!! note Pull Request

    请向***dev***分支提交PR, 所有向main分支提交的PR将被忽略

## 文档

依赖[Mkdocs Material](https://squidfunk.github.io/mkdocs-material/)
