#include "builders.h"
#include "deps.h"
#include "helper.h"
#include <stdio.h>

static int on_extract_entry(const char *filename, void *arg) {
    static int i = 0;
    int n = *(int *)arg;
    printf("Extracted: %s (%d of %d)\n", filename, ++i, n);

    return 0;
}

void build_win64(Arena *arena, Project *proj, Dependency *dep) {

	int arg = 2;
	zip_extract(lit(&dep->full_path), lit(&proj->directories.build), on_extract_entry, &arg);

	String in_exe_path = cat(
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

	printf("EXE PATH: %s\n", lit(&in_exe_path));

	String out_path = cat(
		arena, 
		lit(&proj->directories.build),
		"/",
		lit(&dep->file_name),
		"/",
		lit(&proj->package.name),
		".exe"
	);

	FILE *in_exe = fopen(lit(&in_exe_path), "rb");
	FILE *out_exe = fopen(lit(&out_path), "wb");
	FILE *love = fopen(lit(&love_path), "rb");

	if (!in_exe || !out_exe || !love) {
		perror("fopen");
	}

	char buffer[8192];
	size_t n;


	// Copy love.exe
	while ((n = fread(buffer, 1, sizeof(buffer), in_exe)) > 0) {
		fwrite(buffer, 1, n, out_exe);
	}

	// Write to new exe
	while ((n = fread(buffer, 1, sizeof(buffer), love)) > 0) {
		fwrite(buffer, 1, n, out_exe);
	}

	fclose(in_exe);
	fclose(love);
	fclose(out_exe);

	// Delete the original exe.
	remove(lit(&in_exe_path));
}
