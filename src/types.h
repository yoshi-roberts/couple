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

typedef const char* literal;

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

#ifdef _WIN32
    #include <direct.h>
    #define mkdir(path, mode) _mkdir(path)
#else
    #include <sys/stat.h>
    #include <sys/types.h>
#endif

// static void error(const char *msg, const char *msg1) {
//   fprintf(stderr, "ERROR: %s%s\n", msg, msg1 ? msg1 : "");
//   exit(1);
// }

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

String string_new(literal string_lit);
String string_make(Arena *arena, usize capacity);
String string_copy(Arena *arena, String *original);
void string_write(String *string, char *string_lit);
char string_get(String *string, usize index);
bool string_cmp(String *a, String *b);
bool string_cmp_lit(String *a, literal b);
String string_cat(Arena *arena, literal a, literal b);
usize _string_len(const char *chars);

#define len(obj)\
    ((obj)->length)

#define lit(string) \
    ((string)->chars)

#define str(_lit) \
    (string_new(_lit))

typedef struct {
	FILE *fptr;
	String path;
	String contents;
} File;

File file_read(Arena *arena, const char *file_path);
File file_write(literal file_path, literal contents);

int directory_make(const char *path);

/*
-- EXAMPLES --

Arena arena = {0};

Array array = array_make(&arena, i32, 6);
Array array2 = array_new(i32, 1, 3, 5, 2, 7, 5, 9);

array_push(&array, 123);
array_push(&array, 24);

i32 val = array_get(i32, &array, 0);
i32 val2 = array_get(i32, &array, 1);
i32 val3 = array_get(i32, &array2, 1);
printf("%d\n", val);
printf("%d\n", val2);
printf("%d\n", val3);

String str_static = string_new("Hello, world!");
String str_copy = string_copy(&arena, &str_static);

printf("%s | %lu\n", str_static.chars, str_static.length);
printf("%s | %lu\n", str_copy.chars, str_copy.length);

String strb = string_make(&arena, 32);
string_write(&strb, "Written text.");

printf("%s | %lu\n", strb.chars, strb.length);
*/

#endif // !TYPES_H
