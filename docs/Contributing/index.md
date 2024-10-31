# 开发者注意事项 

## 编码规范

主要采用[linux kernel](https://docs.kernel.org/translations/zh_CN/process/coding-style.html)的规范,
但是统一使用4个空格作为缩进, 注释使用统一的单行注释.

## 开发环境

依赖项:

- gcc/g++12及以上或clang/clang++18及以上的编译器

- [vcpkg](https://vcpkg.io/en/)包管理器

- [mkdocs](https://squidfunk.github.io/mkdocs-material/)文档生成器

使用`git`下载仓库后, `cmake -B build -DQQMUSIC_API_BUILD_TYPE=DEMO && cmake --build build`, 此时会构建与api库动态链接的
可执行文件, 可以利用`demo/main.cc`进行调试(该文件在`.gitignore`中, 更改不会被提交, 专门用于调试)


关于curl库, 请参考[everything curl](https://everything.curl.dev/)文档

## api接口规范

- 所有的入参和返回参数当作函数参数, 返回值需要设置成`qqmusic/error.h`中的`error`枚举类,
代表报错信息, 函数原型举例:

```cpp
qqmusic::result get_song_list(std::string url, std::string& res_json);
```

## 提交规范

从`issue`新建分支时请基于`dev`分支构建. 一个分支对应一个`issue`, 只完成`issue`规定的任务.
分支名称遵循一下规则:

```
branch_type/module/issue
```

1. **主分支(Main Branch)**
    – 主分支通常是master或者main，用于存储稳定、可发布的代码

2. **开发分支(Develop Branch)**
    – 必须是可编译的代码, 其他开发分支从`dev`分支派生

3. **特性分支(Feature Branch)**
    – 从主分支创建的特性分支命名通常以feat/开头, 后接特性的简短描述, 例如: `feat/ui/new-login-page`

4. **修复分支(Bugfix Branch)**
    – 从主分支创建的修复分支命名通常以fix/开头, 后接修复的问题的简短描述, 例如: `bugfix/backend/fix-login-issue`

5. **热修复分支(Hotfix Branch)**
    – 从主分支创建的热修复分支命名通常以hfix/开头, 后接修复的问题的简短描述, 例如: `hfix/security/fix-security-issue`

编写代码的过程中尽量多`commit`, 便于回滚, 并且时常更新`dev`分支的内容保持最新, 可以使用以下命令

```
git pull --rebase origin dev
```

任务完成后需要合并进入`dev`分支, 并关闭`issue`. 

提交规范: [约定式提交规范](https://www.conventionalcommits.org/zh-hans/v1.0.0/)

请在各人的分支中进行操作, 不要自行合并, 进行第一次代码提交后就可以向`dev`分支提出合并请求.

## 文档编写规范

- 每个头文件对应`docs/Api/main_module`或`docs/Api/utils`下的一个markdown文件

- 在文档文件开头应该描述该头文件中的类或函数作用, 以及对应的头文件相对项目根目录的路径

- 一级标题是对头文件内容概括

- 二级标题是类或函数名称, 前面使用加粗斜体格式标注***func***或***class***

- 三级标题是类的成员函数或重要成员变量名称

- 函数接口或类的成员函数需要介绍功能, 参数和返回值含义

***参考示例:[utils/buffer](../Api/utils/buffer.md)***
