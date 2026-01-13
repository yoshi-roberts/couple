#include <stdio.h>
#include <stdlib.h>
#include "console.h"

void console_show_help(Array *commands) {

	printf("\nUsage: couple <command> [arguments]\n");
	printf("\nCommands:\n\n");

	for (usize i = 0; i < len(commands); ++i) {

		ConsoleCommand *cmd = array_get_ptr(ConsoleCommand, commands, i);

		printf("	%-10s%-10s%s\n", lit(&cmd->name), lit(&cmd->usage), lit(&cmd->description));
	}
}

ConsoleResult console_parse(Arena *arena, Array *commands, int argc, const char **argv) {

	ConsoleResult result = {0};

	if (argc == 1) {
		console_show_help(commands);
		return result;
	}

	String cmd_name = string_new((char*)argv[1]); 

	for (usize i = 0; i < len(commands); i++) {

		ConsoleCommand *cmd = array_get_ptr(ConsoleCommand, commands, i);

		if (string_cmp_lit(&cmd_name, "help")) {

			console_show_help(commands);
			return result;

		} else if (string_cmp(&cmd_name, &cmd->name)) {

			// Get args
			Array args = array_make(arena, String, argc - 2);

			for (int j = 2; j < argc; j++) {

				String arg = string_new((char*)argv[j]);
				array_push(&args, arg);
			}

			if (len(&args) != cmd->arg_count) {
				printf("\nInvalid number of arguments.\n");
				printf("Use the help command to see usage.\n");
				return result;
			}

			result.command = cmd_name;
			result.args = args;
			result.initialized = true;

			return result;
		}
	}

	printf("'%s' is not a valid command.\n", lit(&cmd_name));
	return result;
}
