add_rules("mode.debug", "mode.release")

package("zip")
add_deps("cmake")
set_sourcedir(path.join(os.scriptdir(), "lib/zip"))
set_policy("package.install_always", true)
on_install(function(package)
	local configs = {}
	table.insert(configs, "-DCMAKE_BUILD_TYPE=" .. (package:debug() and "Debug" or "Release"))
	table.insert(configs, "-DBUILD_SHARED_LIBS=" .. (package:config("shared") and "ON" or "OFF"))
	import("package.tools.cmake").install(package, configs)
end)
-- on_test(function (package)
--     assert(package:has_cfuncs("add", {includes = "foo.h"}))
-- end)
package_end()

add_requires("zip", "libcurl")

target("couple")
set_languages("c99")
set_kind("binary")

add_packages("zip", "libcurl")
add_files("lib/tomlc17.c")
add_files("src/*.c")
