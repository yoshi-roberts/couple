#include "helper.h"
#include "console.h"
#include "callbacks.h"
#include <stdlib.h>

int main(int argc, const char** argv) {

	Arena arena = {0};

	Array commands = array_new(ConsoleCommand,
		COMMAND_EXACT("help", 0, NULL, "", "display this help message"),
		COMMAND_EXACT("init", 1, callback_init, "[PATH]", "initialize a new project"),
		COMMAND_EXACT("run", 1, callback_run, "", "run the project"),
		COMMAND_MIN("build", 1, callback_build, "[PLATFORMS]", "build the project"),
	);

	ConsoleResult result = console_parse(&arena, &commands, argc, argv);

	if (!result.initialized) {
		return 0;
	}

	result.callback(&arena, &result);

	arena_free(&arena);

	return 0;
}
