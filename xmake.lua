add_rules("mode.debug", "mode.release")
set_languages("c++20")

if #os.dirs("lib/*") > 0 then
    includes("lib/**/xmake.lua")
end

local sharedPackages = { "spdlog", "sqlitecpp", "libcurl", "nlohmann_json", "thread-pool" }
local releasePackages = {}
local debugPackages = {}

add_requires(table.join(releasePackages, debugPackages, sharedPackages))
add_packages(sharedPackages)

set_defaultmode("debug")
set_license("GPL-2.0")
set_kind("binary")
set_optimize("smallest")
add_files("src/*.cpp")
add_includedirs("include")

set_warnings("more")

target("birdseye")
    add_packages(releasePackages)
    add_defines("SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE")
    set_configdir("$(buildir)/$(plat)/$(arch)/$(mode)/third_party")
    --add_configfiles("third_party/*", {onlycopy = true})
target_end()

-- @code
--    -- add debug and release modes
--    add_rules("mode.debug", "mode.release")
--
--    -- set warning all as error
--    set_warnings("all", "error")
--
--    -- add link libraries and search directories
--    add_links("tbox")
--    add_linkdirs("/usr/local/lib", "/usr/lib")
--
--    -- add system link libraries
--    add_syslinks("z", "pthread")
--
--    -- add compilation and link flags
--    add_cxflags("-stdnolib", "-fno-strict-aliasing")
--    add_ldflags("-L/usr/local/lib", "-lpthread", {force = true})
--
-- @endcode
--

