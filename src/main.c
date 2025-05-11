#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "console.h"

int main( int argc, const char** argv ) {

	console_command commands[] = {
		COMMAND_STRING("new", "Create a new project"),
		COMMAND_END()
	};

	console_result result = console_parse(commands, argc, argv);

	if (strcmp(result.command_name, "new") == 0) {
		if (result.arg_count > 0) {
			printf("%s\n", result.args[0]);
		}
	}

	free(result.args);

	return 0;
}
