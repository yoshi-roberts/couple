#include <stdio.h>
#include "console.h"

int main( int argc, const char** argv ) {

	console_command commands[] = {
		COMMAND_STRING("new", "Create a new project"),
		COMMAND_END()
	};

	console_parse(commands, argc, argv);

	return 0;
}
