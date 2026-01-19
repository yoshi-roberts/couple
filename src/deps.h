#ifndef DEPS_H
#define DEPS_H

#include "helper.h"
#include <curl/curl.h>

#define DEP_BASE_URL "https://github.com/love2d/love/releases/download/"

typedef enum {
	PLATFORM_WINDOWS,
	PLATFORM_LINUX,
	PLATFORM_INVALID
} DependencyPlatform; 

typedef enum {
	VERSION_LATEST = 0,
	VERSION_11_5 = 0
} DependencyVersion;

typedef struct {
	DependencyPlatform platform;
	DependencyVersion version;
	String love_version;
	String url;
	String path;
} Dependency;

Dependency dep_init(Arena *arena, String *plat);
int _download_dep(Arena *arena, literal url);
bool dep_check(Arena *arena, Dependency *dep);
int dep_get(Arena *arena, Dependency *dep);
DependencyPlatform str_to_plat(String *plat);
literal plat_to_path(DependencyPlatform plat);

#endif // !DEPS_H
