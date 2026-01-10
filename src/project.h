#ifndef CONFIG_H
#define CONFIG_H

#include "types.h"

/*

[package]
name = "Name"
author = "Author"
version = "0.1.0"

[directories]
src = "src"
build = "build"

*/

#define CONFIG_TEMPLATE \
"[package]\n"           \
"name = \"Name\"\n"     \
"author = \"Author\"\n" \
"version = \"0.1.0\"\n"   \
"\n"                    \
"[directories]\n"       \
"src = \"src\"\n"       \
"build = \"build\"\n"   \

#define MAIN_TEMPLATE                 \
"\nfunction love.load()\n\nend\n"     \
"\nfunction love.update(dt)\n\nend\n" \
"\nfunction love.draw()\n\nend\n"     \


typedef struct {
	String name;
	String version;
	String author;
} PackageConfig;

typedef struct {
	String path;
	PackageConfig package;
} Project;

void config_new(literal file_path);
void project_init(Arena *arena, literal path);
Project project_load(Arena *arena, literal path);

#endif // !CONFIG_H
