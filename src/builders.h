#ifndef BUILDERS_H
#define BUILDERS_H

#include "project.h"
#include "../lib/zip/src/zip.h"

void build_love(Arena *arena, Project *proj);
void _zip_walk(Arena *arena, struct zip_t *zip, String path);

#endif // !BUILDERS_H
