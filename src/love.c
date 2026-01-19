#include "builders.h"
#include "helper.h"

void _zip_walk(Arena *arena, struct zip_t *zip, String path, String relative_path) {

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

		String zip_path;

		if (len(&relative_path) <= 0) {
			zip_path = str(entry->d_name);
		} else {
			zip_path = cat(arena, lit(&relative_path), "/", entry->d_name);
		}

		printf("REL PATH: %s\n", lit(&zip_path));
		
		stat(lit(&fullpath), &s);

		if (S_ISDIR(s.st_mode)) {

			_zip_walk(arena, zip, fullpath, zip_path);

		} else {

			zip_entry_open(zip, lit(&zip_path));
			zip_entry_fwrite(zip, lit(&fullpath));
			zip_entry_close(zip);
		}
	}

	closedir(dir);
}

void build_love(Arena *arena, Project *proj) {

	String zip_path = cat(arena, lit(&proj->directories.build), "/", lit(&proj->package.name), ".love");
	printf("ZIP PATH: %s\n", lit(&zip_path));

	struct zip_t *zip = zip_open(lit(&zip_path), ZIP_DEFAULT_COMPRESSION_LEVEL, 'w');

	_zip_walk(arena, zip, proj->directories.src, str(""));

	printf("Created ZIP\n");
	zip_close(zip);
}
