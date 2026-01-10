#include "types.h"
#include "console.h"
#include "project.h"
#include <stdio.h>

int main(int argc, const char** argv) {

	Arena arena = {0};

	Array commands = array_new(ConsoleCommand,
		COMMAND_EXACT("help", 0, "", "display this help message"),
		COMMAND_EXACT("init", 1, "[PATH]", "initialize a new project"),
		COMMAND_EXACT("run", 0, "", "run the project"),
		COMMAND_MIN("build", 0, "[TARGETS]", "build the project"),
	);

	ConsoleResult result = console_parse(&arena, &commands, argc, argv);

	printf("PATH: %s\n", argv[0]);

	if (!result.initialized) {
		return 0;
	}

	if (string_cmp_lit(&result.command, "init")) {

		if (len(&result.args) == 1) {

			String *path = array_get_ptr(String, &result.args, 0);
			project_init(&arena, lit(path));
		}

	} else if (string_cmp_lit(&result.command, "build")) {

		Project proj = project_load(&arena, argv[0]);
	}

	arena_free(&arena);

	return 0;
}
