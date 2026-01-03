#include <stdio.h>
#include <stdlib.h>
#include "console.h"
#include "types.h"

void console_show_help(Array *commands) {

	printf("Commands:\n");

	for (int i = 0; i < len(commands); ++i) {
		console_command *cmd = array_get_ptr(console_command, commands, i);
		printf("	%-10s%s\n", str(&cmd->name), str(&cmd->description));
	}
}

console_result console_parse(Arena *arena, Array *commands, int argc, const char **argv) {

	if (argc == 1) {
		console_show_help(commands);
		exit(0);
	}

	// const char *cmd_name = argv[1];
	String cmd_name = string_new((char*)argv[1]); 

	for (int i = 0; i < len(commands); i++) {

		console_command *cmd = array_get_ptr(console_command, commands, i);

		if (string_cmp(&cmd_name, &cmd->name)) {

			// Get args
			Array args = array_make(arena, String, argc - 2);

			for (int j = 2; j < argc; j++) {

				String arg = string_new((char*)argv[j]);
				array_push(&args, arg);
			}

			return (console_result) {
				.command = cmd_name,
				.args = args
			};
		}
	}

	printf("'%s' is not a valid command.\n", str(&cmd_name));
	exit(0);
}
