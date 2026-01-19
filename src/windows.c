#include "builders.h"
#include "deps.h"
#include "helper.h"

static int on_extract_entry(const char *filename, void *arg) {
    static int i = 0;
    int n = *(int *)arg;
    printf("Extracted: %s (%d of %d)\n", filename, ++i, n);

    return 0;
}

void build_win64(Arena *arena, Project *proj, Dependency *dep) {

	int arg = 2;
	zip_extract(lit(&dep->full_path), lit(&proj->directories.build), on_extract_entry, &arg);

	String exe_path = cat(
		arena, 
		lit(&proj->directories.build),
		"/",
		lit(&dep->file_name),
		"/love.exe"
	);

	String love_path = cat(
		arena, 
		lit(&proj->directories.build),
		"/",
		lit(&proj->package.name),
		".love"
	);

	printf("EXE PATH: %s\n", lit(&exe_path));

	String new_path = cat(
		arena, 
		lit(&proj->directories.build),
		"/",
		lit(&dep->file_name),
		"/",
		lit(&proj->package.name),
		".exe"
	);

	printf("NEW EXE PATH: %s\n", lit(&new_path));

	FILE *exe = fopen(lit(&exe_path), "ab");   // append binary
	FILE *love = fopen(lit(&love_path), "rb");

	if (!exe || !love) {
		perror("fopen");
	}

	char buffer[8192];
	size_t n;

	while ((n = fread(buffer, 1, sizeof(buffer), love)) > 0) {
		fwrite(buffer, 1, n, exe);
	}

	fclose(love);
	fclose(exe);

	// FILE *exe_fptr = fopen(lit(&exe_path), "rb");
	//    FILE *love_fptr = fopen(lit(&love_path), "rb");
	//    FILE *new_fptr = fopen(lit(&new_path), "wb");
	//
	//    // Check if the files were opened successfully
	//    if (exe_fptr == NULL || love_fptr == NULL || new_fptr == NULL) {
	//        printf("Error opening files!\n");
	//    }
	//
	//    // Copy contents of the first file to the third file
	//    char ch;
	//    while ((ch = fgetc(exe_fptr)) != EOF) {
	//        fputc(ch, new_fptr);
	//    }
	//
	//    // Copy contents of the second file to the third file
	//    while ((ch = fgetc(love_fptr)) != EOF) {
	//        fputc(ch, new_fptr);
	//    }
	//
	//    fclose(exe_fptr);
	//    fclose(love_fptr);
	//    fclose(new_fptr);
}
