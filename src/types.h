#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float  f32;
typedef double f64;

typedef size_t    usize;
typedef ptrdiff_t isize;
typedef usize uptr;

#define ARENA_BLOCK_SIZE (16*1024)
#define ARENA_MAX_ALIGN _Alignof(max_align_t)
#define ARENA_ALIGN_UP(x, n) (((x) + (n) - 1) & ~((n) - 1))

typedef struct ArenaBlock {
	struct ArenaBlock *next;
	u8 *first;
	u8 *ptr;
	u8 *end;
} ArenaBlock;

typedef struct {
	ArenaBlock *blocks;
	ArenaBlock *current;
} Arena;

union ArenaMaxAlign {

};

void* arena_alloc(Arena *arena, usize size);
void arena_dealloc(Arena *arena);
void arena_free(Arena *arena);

typedef struct {
	i32 *items;
	i32 len;
	i32 capacity;
} Int32Array;

typedef struct {
	char *chars;
	i32 len;
} String;

String string_new(char *str);
i32 _string_len(char *chars);

#endif // !TYPES_H
