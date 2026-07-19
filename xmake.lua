add_requires("nlohmann_json", {system = false})

-- include subprojects
includes("lib/commonlibsse")
--includes("extern/styyx-utils/xmake.lua")

-- set project constants
set_project("divine-restriction")
set_version("0.0.0")
set_license("GPL-3.0")
set_languages("c++23")
set_warnings("allextra")

-- add common rules
add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")

-- define targets
target("Divine-Restriction")
    add_rules("commonlibsse.plugin", {
        name = "Divine-Restriction",
        author = "NHK-512",
        description = "Throughout the Heavens and Earth, I alone am the Restricted one."
    })

    -- add src files
    add_files("src/**.cpp")
    add_headerfiles("src/**.h")
    add_includedirs("src")
    set_pcxxheader("src/pch.h")
    add_packages("nlohmann_json")
