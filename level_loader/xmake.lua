add_rules("mode.debug", "mode.release")

-- set_warnings("all", "error")

set_languages("c99")

add_requires("raylib")

target("level_loader")
    set_kind("binary")

	add_packages("raylib")

    add_files("src/*.c")
    add_headerfiles("src/libs/*.h")

	set_rundir("$(projectdir)")

