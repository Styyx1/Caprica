-- set minimum xmake version
set_xmakever("3.1.1")
includes("@builtin/xpack")
-- set project
set_project("caprica")
set_languages("c++23")
set_license("MIT")
set_version("0.3.0")

-- require packages
add_requires("boost", { configs = { filesystem = true, program_options = true, container = true } })
add_requires("pugixml")
set_allowedmodes("release", "debug", "releasedbg")
set_defaultmode("release")

option("tests", { default = false, description = "Build tests" })

if has_config("tests") then
    add_requires("catch2")
end

target("caprica", function()
    set_kind("$(kind)")
        -- for appimage
    add_rpathdirs("$ORIGIN/../lib")
    add_packages("pugixml", { public = true })

        -- adding as package gives weird linker errors i can't figure out
    add_syslinks("boost_filesystem", "boost_program_options", "stdc++fs")

    add_files("caprica/**/**.cpp")

    if is_kind("binary") then
        add_files("caprica/**.cpp")
    end

    add_includedirs("caprica", { public = true })
    add_headerfiles("caprica/(**.h)")
    add_includedirs("/usr/include")

    add_cxxflags("cl::/Zc:inline", "cl::/bigobj")

end)

if has_config("tests") then
    target("caprica_tests", function()
        set_kind("binary")
        set_default(false)
        set_group("test")
        add_deps("caprica")
        add_packages("catch2")
        add_files("tests/**.cpp")
        
    end)
end

xpack("caprica")
        set_formats("appimage")
        set_title("caprica")
        add_targets("caprica")
        add_installfiles("/usr/lib/libboost_filesystem.so.*", {
            prefixdir = "lib"
        })
        add_installfiles("/usr/lib/libboost_program_options.so.*", {
            prefixdir = "lib"
        })
        add_installfiles("/usr/lib/libboost_container.so.*", {
            prefixdir = "lib"
        })
        on_load(function (package)
            package:set("iconfile", path.join(os.scriptdir(), "assets/caprica.png"))
    end)
