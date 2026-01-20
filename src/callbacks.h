#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "helper.h"

void callback_init(Arena *arena, void *data);
void callback_run(Arena *arena, void *data);
void callback_build(Arena *arena, void *data);

#endif // !CALLBACKS_H
