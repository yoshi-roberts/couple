#ifndef BUILDERS_H
#define BUILDERS_H

#include "deps.h"
#include "project.h"
#include "../lib/zip/src/zip.h"

#define SQUASHFS_OFFSET 193728

void build_love(Arena *arena, Project *proj);
void _zip_walk(Arena *arena, struct zip_t *zip, String path, String relative_path);
void build_win64(Arena *arena, Project *proj, Dependency *dep);

#endif // !BUILDERS_H
