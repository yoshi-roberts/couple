#include "console.h"
#include <string.h>
#include <stdio.h>

void console_show_help(console_command *commands) {

	printf("Commands:\n");

	console_command cmd = commands[0];

	int i = 0;
	while (cmd.type != COMMAND_TYPE_END) {

		printf("	%-10s%s\n", cmd.name, cmd.description);

		i++;
		cmd = commands[i];
	}
}

void console_parse(console_command *commands, int argc, const char **argv) {

	if (argc == 1) {
		console_show_help(commands);
		return;
	}

	console_command cmd = commands[0];
	const char *cmd_name = argv[1];

	int i = 0;
	while (cmd.type != COMMAND_TYPE_END) {

		if (strcmp(cmd_name, cmd.name) == 0) {
			// Handle command
			printf("%s\n", cmd.name);

			return;
		}

		i++;
		cmd = commands[i];
	}

	printf("'%s' is not a valid command.\n", cmd_name);
}
