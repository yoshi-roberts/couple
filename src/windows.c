#include "builders.h"
#include "helper.h"

static int on_extract_entry(const char *filename, void *arg) {
    static int i = 0;
    int n = *(int *)arg;
    printf("Extracted: %s (%d of %d)\n", filename, ++i, n);

    return 0;
}

void build_win64(Arena *arena, Project *proj) {

	String zip_path = str("deps/win64");

	printf("%s\n", lit(&zip_path));

	int arg = 2;
	zip_extract(lit(&zip_path), "build/win64", on_extract_entry, &arg);


	// FILE *exe_fptr = fopen("build/love.exe", "r");
	//
	// if (!file.fptr) {
	// 	error("Could not open file.");
	// }
	//
	// // Read contents
	//
	// fseek(file.fptr, 0, SEEK_END); 
	// usize size = ftell(file.fptr);
	// fseek(file.fptr, 0, SEEK_SET);
	//
	// file.contents = string_make(arena, size);
	// fread(file.contents.chars, size, 1, file.fptr);
	//
	// fclose(file.fptr);
}
