set_xmakever("3.1.1")
set_project("MusicQueueFix")
set_license("MIT")
set_policy("package.requires_lock", true)

local version = "0.1.0"

add_repositories("bmk https://github.com/gabriel-andreescu/BethesdaModKit.git")
add_addons("bmk 0.3.0")
includes("@addon/bmk/project")
includes("@addon/bmk/native")

-- Dependencies
add_requires("commonlibsse-ng 8.0.1", { system = false })
add_requires("bmk", "devbench-api 2026.09.13", { system = false })

-- Build targets

target("Native", function()
    set_default(false)
    set_basename("MusicQueueFix")
    set_version(version)
    add_rules("@commonlibsse-ng/plugin", {
        author = "GabonZ",
        description = "Fixes music left in the playback queue after loading a save.",
    })
    add_rules("@addon/bmk/skyrim.plugin")
    add_files("$(projectdir)/src/**.cpp")
    add_includedirs("$(projectdir)/src")
    set_pcxxheader("src/PCH.h")
    add_packages("commonlibsse-ng")
    add_rules("@devbench-api/integration")
    add_packages("bmk", "devbench-api")
end)

-- Packages
target("MusicQueueFix", function()
    set_version(version)
    add_rules("@addon/bmk/skyrim.package", {
        targets = {
            "Native",
        },
        nexus = {
            mod_id = "7318624464863",
            file_id = "7998894",
            category = "main",
            primary = true,
            display_name = "Music Queue Fix",
            description = "For SE, AE, GOG and VR.",
        },
    })
    add_installfiles("$(projectdir)/assets/(**)|misc/**")
end)

target("MusicQueueFixReproduction", function()
    set_version("1.0.0")
    add_rules("@addon/bmk/skyrim.package", {
        package_name = "Whiterun Music Reload Reproduction",
        nexus = {
            mod_id = "7318624464863",
            file_id = "7998912",
            category = "miscellaneous",
            display_name = "Whiterun Music Reload Reproduction",
            description = "Two saves and instructions for reproducing the Whiterun battle music bug on Steam AE 1.7.104.",
        },
    })
    add_installfiles("$(projectdir)/assets/misc/reproduction/(**)")
end)
