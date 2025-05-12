#include "types.h"
#include <stdio.h>

int main( int argc, const char** argv ) {

	Arena arena = {0};

	const char *str = arena_alloc(&arena, sizeof(char) * 32);
	sprintf(str, "Hello World!");
	printf("%s\n", str);

	arena_free(&arena);

	// console_command commands[] = {
	// 	COMMAND_STRING("new", "Create a new project"),
	// 	COMMAND_END()
	// };
	//
	// console_result result = console_parse(commands, argc, argv);
	//
	// if (strcmp(result.command_name, "new") == 0) {
	// 	if (result.arg_count > 0) {
	// 		printf("%s\n", result.args[0]);
	// 	}
	// }
	//
	// free(result.args);

	return 0;
}
