#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

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

typedef union {
    long long ll;
    long double ld;
    void *p;
} max_align;

#if defined(_MSC_VER)
	#include <intrin.h>
	#define TRAP() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
	#include <signal.h>
	#define TRAP() __builtin_trap()
#endif

#define error(message) ({			\
	printf("ERROR: %s\n", message);	\
	TRAP();							\
})

#define ARENA_BLOCK_SIZE (16*1024)
#define ARENA_MAX_ALIGN _Alignof(max_align)
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

void* arena_alloc(Arena *arena, usize size);
void arena_dealloc(Arena *arena);
void arena_free(Arena *arena);

typedef struct {
	void *items;
	usize length;
	usize capacity;
	usize item_size;
} Array;

Array _array_new(void *items, usize capacity, usize item_size);
Array _array_make(Arena *arena, usize capacity, usize item_size);
void* _array_get(Array *array, usize index);
void _array_push(Array *array, void* item);
void _array_set(Array *array, void *item, usize index);

#define array_new(type, ...) ({											\
	usize capacity = sizeof((type[]){ __VA_ARGS__ }) / sizeof(type);	\
	_array_new((type[]){ __VA_ARGS__ }, capacity, sizeof(type));		\
})

#define array_make(arena, type, capacity) \
	(_array_make(arena, capacity, sizeof(type)))

#define array_get(type, array, index) \
	(*(type*)_array_get(array, index))

#define array_get_ptr(type, array, index) \
	((type*)_array_get(array, index))

#define array_push(array, value) ({		\
	__typeof__(value) tmp = value;		\
	_array_push(array, &tmp);			\
})

#define array_set(array, value, index) ({    \
	__typeof__(value) tmp = value;		\
	_array_set(array, &tmp, index);		 \
})

typedef struct {
	char *chars;
	usize length;
	usize capacity;
} String;

String string_new(char *str);
String string_make(Arena *arena, usize capacity);
String string_copy(Arena *arena, String *original);
void string_write(String *string, char *literal);
char string_get(String *string, usize index);
bool string_cmp(String *a, String *b);
usize _string_len(char *chars);

#define len(obj)\
    ((obj)->length)

#define str(string) \
    ((string)->chars)

#endif // !TYPES_H
