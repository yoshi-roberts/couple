#ifndef DEPS_H
#define DEPS_H

#include "helper.h"
#include <curl/curl.h>

#define DEP_BASE_URL "https://github.com/love2d/love/releases/download/"

typedef enum {
	WINDOWS_LATEST,
	LINUX_LATEST,
	INVALID_DEP
} Dependency; 

int _download_dep(Arena *arena, literal url);
bool dep_check(Arena *arena, String *plat);
int dep_get(Arena *arena, String *plat);
Dependency plat_to_dep(String *plat);

#endif // !DEPS_H
