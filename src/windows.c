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
}
