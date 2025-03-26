set_xmakever("2.9.7")
set_project("qqmusic_api_cxx")

set_allowedplats("windows", "linux", "macosx")
set_allowedmodes("debug", "release")
set_policy("build.warning", true)

add_rules("mode.release", "mode.debug")
set_languages("c++20")

add_includedirs("include/")

function vcpkg(name, configs)
    if not configs then
        configs = {external = true}
    end

    -- Must match a release commit from https://github.com/microsoft/vcpkg/
    local baseline = "23b33f5a010e3d67132fa3c34ab6cd0009bb9296"

    configs["baseline"] = baseline

    -- Will cause trouble
    configs["default_registries"] = {
        kind = "git",
        baseline = baseline,
        repository = "https://github.com/microsoft/vcpkg"
    }

    configs["registries"] = {
        {
            kind = "artifact",
            location = "https://github.com/microsoft/vcpkg-ce-catalog/archive/refs/heads/main.zip",
            name = "microsoft"
        }
    }

    add_requires("vcpkg::" .. name, {
        alias = name,
        configs = configs,
    })
end

vcpkg("boost-asio")
vcpkg("boost-beast")
vcpkg("boost-uuid")
vcpkg("boost-url")
vcpkg("botan")
vcpkg("zlib")
vcpkg("openssl")
vcpkg("nlohmann-json")

if is_mode("debug") then
    add_rules("plugin.compile_commands.autoupdate", {outputdir="./build/"})
end

if is_plat("linux") then
    add_defines("PLATFORM_LINUX")
elseif is_plat("macosx") then
    add_defines("PLATFORM_APPLE")
    add_values("cmake.CMAKE_OSX_DEPLOYMENT_TARGET", "11.0")
elseif is_plat("windows") then
    add_defines("PLATFORM_WINDOWS")
    add_defines("_WIN32_WINNT=0xA00")
    if is_toolchain("msvc") then
        add_defines("/bigobj")
    end
    add_rules("utils.symbols.export_all")
end

target("qmapi")
    set_kind("shared")
    add_files("src/*.cc", 
              "src/utils/*.cc", 
              "src/details/*.cc", 
              "src/crypto/*.cc")
    add_packages(
        "boost-asio",
        "boost-beast",
        "boost-uuid",
        "boost-url",
        "zlib",
        "nlohmann-json",
        "botan",
        "openssl"
    )

target("test")
    set_kind("binary")
    add_files("test/src/*.cc")
    add_deps("qmapi")
    add_packages(
        "gtest[main]",
        "boost-asio",
        "boost-beast",
        "boost-uuid",
        "boost-url",
        "zlib",
        "nlohmann-json",
        "botan",
        "openssl"
    )
    add_includedirs("test/include")

target("demo")
    set_kind("phony")
    includes("demo/xmake.lua")
    add_deps("qrc-decoder", "qmdown")
    add_packages(
        "boost-asio",
        "boost-beast",
        "boost-uuid",
        "boost-url",
        "zlib",
        "nlohmann-json",
        "botan",
        "openssl"
    )
