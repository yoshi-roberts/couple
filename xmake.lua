add_rules("mode.debug", "mode.release")

target("couple")
set_languages("c99")
set_kind("binary")

add_files("lib/argparse/argparse.c")
add_files("src/*.c")
