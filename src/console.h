#ifndef CONSOLE_H
#define CONSOLE_H

#include "types.h"

typedef enum {
	COMMAND_TYPE_STRING,
	COMMAND_TYPE_END
} console_command_type;

typedef struct {
	String name;
	String usage;
	String description;
	usize arg_count;
	console_command_type type;
} console_command;

typedef struct {
	String command;
	Array args;
} console_result;

#define COMMAND_STRING(_name, _arg_count, _usage, _desc) \
	((console_command){                          \
		.name = (_name),                         \
        .usage = (string_new(_usage)),           \
		.arg_count = (_arg_count),               \
		.description = (_desc),                  \
		.type = (COMMAND_TYPE_STRING)            \
	})
#define COMMAND_END() \
	((console_command){ .type = (COMMAND_TYPE_END) })

console_result console_parse(Arena *arena, Array *commands, int argc, const char **argv);
void console_show_help(Array *commands);

#endif // !CONSOLE_H
