#include "project.h"
#include "types.h"
#include "../lib/tomlc17.h"

void config_new(literal file_path) {
	File conf_file = file_write(file_path, CONFIG_TEMPLATE);
}

void project_init(Arena *arena, literal path) {

	// TODO: Verify path

	String config_path = string_cat(arena, path, "/couple.toml");
	String src_path = string_cat(arena, path, "/src");
	String main_path = string_cat(arena, path, "/src/main.lua");

	config_new(lit(&config_path));
	directory_make(lit(&src_path));

	File main_file = file_write(lit(&main_path), MAIN_TEMPLATE);
}

Project project_load(Arena *arena, literal path) {

	Project proj = {0};

	proj.path = string_new(path);

	String config_path = string_cat(arena, lit(&proj.path), "/couple.toml");

	printf("Config Path: %s\n", lit(&config_path));

	// toml_result_t result = toml_parse_file_ex(path);
	//
	// if (!result.ok) {
	// 	error(result.errmsg);
	// }
	//
	// toml_datum_t package_name = toml_seek(result.toptab, "package.name");
	// toml_datum_t package_author = toml_seek(result.toptab, "package.author");
	// toml_datum_t package_version = toml_seek(result.toptab, "package.version");
	//
	// if (package_name.type != TOML_STRING) {
	// 	error("missing or invalid 'server.host' property in config");
	// }
	//
	// proj.package.name = string_new(package_name.u.s);
	// proj.package.author = string_new(package_author.u.s);
	// proj.package.version = string_new(package_version.u.s);
	//
	// toml_free(result);

	return proj;
}
