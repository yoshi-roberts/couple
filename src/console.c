#include "console.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void console_show_help(console_command *commands) {

	printf("Commands:\n");

	for (int i = 0; commands[i].type != COMMAND_TYPE_END; ++i) {
		console_command cmd = commands[0];
		printf("	%-10s%s\n", cmd.name, cmd.description);
	}
}

console_result console_get_result(int argc, const char **argv) {

	console_result result = {};

	int count = argc - 2 + 1;

	result.args = malloc(sizeof(char*) * (count + 1));

	for (int i = 0; i < count; ++i) {
		result.args[i] = argv[2 + i];
	}

	result.args[count] = NULL;
	result.arg_count = count - 1;

	return result;
}

console_result console_parse(console_command *commands, int argc, const char **argv) {

	if (argc == 1) {
		console_show_help(commands);
		exit(0);
	}

	const char *cmd_name = argv[1];

	for (int i = 0; commands[i].type != COMMAND_TYPE_END; ++i) {

		console_command cmd = commands[0];

		if (strcmp(cmd_name, cmd.name) == 0) {

			console_result result = console_get_result(argc, argv);
			result.command_name = cmd_name;

			return result;
		}
	}

	printf("'%s' is not a valid command.\n", cmd_name);
	exit(0);
}
