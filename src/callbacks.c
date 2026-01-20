#include "callbacks.h"
#include "builders.h"
#include "console.h"
#include "deps.h"
#include "project.h"
#include <stdlib.h>

void callback_init(Arena *arena, void *data) {

	ConsoleResult *result = (ConsoleResult*)data;

	if (len(&result->args) == 1) {

		String *path = array_get_ptr(String, &result->args, 0);
		project_init(arena, lit(path));
	}
}

void callback_run(Arena *arena, void *data) {

	ConsoleResult *result = (ConsoleResult*)data;

	String *path = array_get_ptr(String, &result->args, 0);
	printf("PATH: %s\n", lit(path));

	Project proj = project_load(arena);
	String love_cmd = cat(arena, "love ", lit(&proj.directories.src));

	u64 cmd_result = system(lit(&love_cmd));
}

void callback_build(Arena *arena, void *data) {

	ConsoleResult *result = (ConsoleResult*)data;

	Project proj = project_load(arena);

	if (!proj.initialized) {
		// TODO: Error message.
		return;
	}


	String *target = array_get_ptr(String, &result->args, 0);

	// Make sure build dir exists.
	if (!directory_exists(lit(&proj.directories.build))) {
		directory_make(lit(&proj.directories.build));
		printf("Created build directory\n");
	}

	Dependency dep = dep_init(arena, target);

	if (!dep_check(arena, &dep)) {

		int downloaded = dep_get(arena, &dep);
		printf("DOWNLOADED: %d\n", downloaded);
	}

	build_love(arena, &proj);

	switch (dep.platform) {

		case PLATFORM_WINDOWS:
			build_win64(arena, &proj, &dep);
			break;

		case PLATFORM_LINUX: break;

		case PLATFORM_INVALID: break;
	}
}
