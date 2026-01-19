#include "builders.h"
#include "deps.h"
#include "helper.h"
#include "console.h"
#include "project.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char** argv) {

	Arena arena = {0};

	Array commands = array_new(ConsoleCommand,
		COMMAND_EXACT("help", 0, "", "display this help message"),
		COMMAND_EXACT("init", 1, "[PATH]", "initialize a new project"),
		COMMAND_EXACT("run", 1, "", "run the project"),
		COMMAND_MIN("build", 1, "[PLATFORMS]", "build the project"),
	);

	ConsoleResult result = console_parse(&arena, &commands, argc, argv);

	if (!result.initialized) {
		return 0;
	}

	if (string_cmp_lit(&result.command, "init")) {

		if (len(&result.args) == 1) {

			String *path = array_get_ptr(String, &result.args, 0);
			project_init(&arena, lit(path));
		}

	} else if (string_cmp_lit(&result.command, "build")) {

		Project proj = project_load(&arena);

		if (proj.initialized) {

			String *target = array_get_ptr(String, &result.args, 0);

			// Make sure build dir exists.
			if (!directory_exists(lit(&proj.directories.build))) {
				directory_make(lit(&proj.directories.build));
				printf("Created build directory\n");
			}

			if (!dep_check(&arena, target)) {

				int downloaded = dep_get(&arena, target);
				printf("DOWNLOADED: %d\n", downloaded);
			}

			build_love(&arena, &proj);
			build_win64(&arena, &proj);
		}


	} else if (string_cmp_lit(&result.command, "run")) {

		String *path = array_get_ptr(String, &result.args, 0);
		printf("PATH: %s\n", lit(path));
		Project proj = project_load(&arena);
		String love_cmd = cat(&arena, "love ", lit(&proj.directories.src));

		system(lit(&love_cmd));
	}

	arena_free(&arena);

	return 0;
}
