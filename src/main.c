#include "types.h"
#include <stdio.h>

int main( int argc, const char** argv ) {

	Arena arena = {0};

	Array array = array_empty(&arena, i32, 6);
	Array array2 = array_new(i32, 1, 3, 5, 2, 7, 5, 9);

	array_push(&array, 123);
	array_push(&array, 24);

	i32 val = array_get(i32, &array, 0);
	i32 val2 = array_get(i32, &array, 1);
	i32 val3 = array_get(i32, &array2, 1);
	printf("%d\n", val);
	printf("%d\n", val2);
	printf("%d\n", val3);

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
