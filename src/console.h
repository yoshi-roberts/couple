#ifndef CONSOLE_H
#define CONSOLE_H

typedef enum {
	COMMAND_TYPE_STRING,
	COMMAND_TYPE_END
} console_command_type;

typedef struct {
	const char *name;
	const char *description;
	console_command_type type;
} console_command;

#define COMMAND_STRING(_name, _desc) \
	((console_command){ \
		.name = (_name), .description = (_desc), .type = (COMMAND_TYPE_STRING) \
	})
#define COMMAND_END() \
	((console_command){ .type = (COMMAND_TYPE_END) })

void console_parse(console_command *commands, int argc, const char **argv);
void console_show_help(console_command *commands);

#endif // !CONSOLE_H
