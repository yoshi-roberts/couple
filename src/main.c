#include "types.h"
#include "console.h"

int main(int argc, const char** argv) {

	Arena arena = {0};

	Array commands = array_new(console_command,
		COMMAND_STRING("help", 0, "", "display this help message"),
		COMMAND_STRING("new", 1, "[PATH]", "create a new project"),
		COMMAND_STRING("run", 0, "", "run the project"),
		COMMAND_STRING("build", 3, "[TARGETS]", "build the project"),
	);

	console_result result = console_parse(&arena, &commands, argc, argv);

	arena_free(&arena);

	return 0;
}
