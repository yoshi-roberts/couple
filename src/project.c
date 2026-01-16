#include "project.h"
#include "../lib/tomlc17.h"
#include "helper.h"

void config_new(literal file_path) {
	File conf_file = file_write(file_path, CONFIG_TEMPLATE);
}

void project_init(Arena *arena, literal path) {

	// TODO: Verify path

	String config_path = cat(arena, path, "/couple.toml");
	String src_path = cat(arena, path, "/src");
	String main_path = cat(arena, path, "/src/main.lua");

	config_new(lit(&config_path));
	directory_make(lit(&src_path));

	File main_file = file_write(lit(&main_path), MAIN_TEMPLATE);
}

Project project_load(Arena *arena) {

	Project proj = {0};
	proj.initialized = false;

	// Check that the current dir is a valid project.
	if (!file_exists("couple.toml")) {
		printf("The current directory is not a valid project.\n");
		return proj;
	}

	// proj.path = str(path);

	// String config_path = string_cat(arena, lit(&proj.path), "/couple.toml");
	// String config_path = cat(arena, lit(&proj.path), "/couple.toml");
	String config_path = str("couple.toml");

	toml_result_t result = toml_parse_file_ex(lit(&config_path));

	if (!result.ok) {
		printf("%s\n", result.errmsg);
	}

	toml_datum_t package_name = toml_seek(result.toptab, "package.name");
	toml_datum_t package_version = toml_seek(result.toptab, "package.version");

	// if (package_name.type != TOML_STRING) {
	// 	error("missing or invalid 'server.host' property in config");
	// }

	proj.package.name = str(package_name.u.s);
	proj.package.version = str(package_version.u.s);

	toml_datum_t src_dir = toml_seek(result.toptab, "directories.src");
	toml_datum_t build_dir = toml_seek(result.toptab, "directories.build");

	// String full_src_path = cat(arena, lit(&proj.path), "/", src_dir.u.s);
	// String full_build_path = cat(arena, lit(&proj.path), "/", build_dir.u.s);

	proj.directories.src = str(src_dir.u.s);
	proj.directories.build = str(build_dir.u.s);

	toml_free(result);

	proj.initialized = true;
	return proj;
}
