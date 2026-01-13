#include "builders.h"
#include "helper.h"

void _zip_walk(Arena *arena, struct zip_t *zip, String path) {

	DIR *dir;
	struct dirent *entry;
	String fullpath = string_make(arena, 4096);
	struct stat s;

	printf("ZIP DIR: %s\n", lit(&path));

	dir = opendir(lit(&path));

	// assert(dir);
	if (!dir) {
		printf("Could not open dir\n");
		return;
	}

	while ((entry = readdir(dir))) {

		// skip "." and ".."

		if (!strcmp(entry->d_name, ".\0") || !strcmp(entry->d_name, "..\0")) {
			continue;
		}

		String combined = cat(arena, lit(&path), "/", entry->d_name);
		string_write(&fullpath, lit(&combined));
		
		stat(lit(&fullpath), &s);

		if (S_ISDIR(s.st_mode)) {

			_zip_walk(arena, zip, fullpath);

		} else {

			zip_entry_open(zip, lit(&fullpath));
			zip_entry_fwrite(zip, lit(&fullpath));
			zip_entry_close(zip);
		}
	}

	closedir(dir);
}

void build_love(Arena *arena, Project *proj) {

	String zip_path = cat(arena, lit(&proj->directories.build), "/", lit(&proj->package.name), ".love");

	struct zip_t *zip = zip_open(lit(&zip_path), ZIP_DEFAULT_COMPRESSION_LEVEL, 'w');

	_zip_walk(arena, zip, proj->directories.src);

	printf("Created ZIP\n");
	zip_close(zip);
}
