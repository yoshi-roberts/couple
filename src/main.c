#include <stdio.h>

#include "types.h"
#include "console.h"

int main(int argc, const char** argv) {

	Arena arena = {0};

	Array array = array_make(&arena, i32, 6);
	Array array2 = array_new(i32, 1, 3, 5, 2, 7, 5, 9);

	array_push(&array, 123);
	array_push(&array, 24);

	i32 val = array_get(i32, &array, 0);
	i32 val2 = array_get(i32, &array, 1);
	i32 val3 = array_get(i32, &array2, 1);
	printf("%d\n", val);
	printf("%d\n", val2);
	printf("%d\n", val3);

	String str_static = string_new("Hello, world!");
	String str_copy = string_copy(&arena, &str_static);

	printf("%s | %lu\n", str_static.chars, str_static.length);
	printf("%s | %lu\n", str_copy.chars, str_copy.length);

	String strb = string_make(&arena, 32);
	string_write(&strb, "Written text.");

	printf("%s | %lu\n", strb.chars, strb.length);

	const char *path = NULL;

	Array commands = array_new(console_command,
		COMMAND_STRING("new", "create a new project"),
		COMMAND_STRING("run", "run the project"),
		COMMAND_STRING("build", "build the project"),
	);

	console_result result = console_parse(&arena, &commands, argc, argv);

	arena_free(&arena);

	return 0;
}
